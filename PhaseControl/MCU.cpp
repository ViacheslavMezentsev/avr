///////////////////////////////////////////////////////////
//  MCU.cpp
//  Implementation of the Class CMCU
//  Created on:      14-июл-2011 11:34:33
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "MCU.h"


// -=[ Пользовательские типы ]=-


// Перечисление видов работы компаратора.
enum EnACMode { acmFirstMeasurement = 0, acmWorkMode };

// Режимы управления фазой за время одного периода.
enum EnPCMode { pcmDisabled = 0, pcmEnabledUp, pcmEnabledDown };

// -=[ Внешние ссылки ]=-

extern FIFO( 16 ) uart_rx_fifo;

// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

// Текущий режим работы аналогового компаратора.
EnACMode ACMode = acmFirstMeasurement;

// Состояния КА для формирования импульса управления симистором.
EnPCMode PCMode = pcmDisabled;

// Номер отсчёта измерения.
uint8_t SamplePoint = 0;

// Вычисляемое значение половины периода сетевого напряжения в отсчётах счётчика
// таймера.
uint16_t HalfPeriod = 0;

// Длительность импульса запуска симистора, мкс.
// Очень короткие импульсы могут не захватываться триггерами простых
// цифровых осциллографов, поэтому реализована возможность регулировки их
// длительности в целях отладки.
uint16_t PulseWidth = 1000U;

// Программный счётчик для формирования событий через определённый временной
// интервал.
uint16_t Counter10ms = 0;

// Массив для хранения значений измеренных временных интервалов для отсчётов
// сетевого напряжения. Эти значения используются в алгоритме определения
// перечечения нуля.
volatile uint16_t PointValues[3] = { 0U, 0U, 0U };


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * Главный (основной) поток программы.
 */
HRESULT CMCU::MainThreadProcedure(){

    // Разрешаем прерывания.
    __enable_interrupt();

    CConsole::CursorOff();
    CConsole::SetColor( clLightGray, clBlack );

    CConsole::ClearScreen();

    // Выводим версию программы и дату сборки.
    CConsole::MoveTo( 1, 24 );
    CConsole::WriteString( CVersion::GetVersionString() );

    CConsole::PutChar( ' ' );
    CConsole::PutChar( '(' );
    CConsole::WriteString( CVersion::GetBuildDateString(), CConsole::cp1251 );
    CConsole::PutChar( ')' );

    CConsole::MoveTo( 1, 25 );
    CConsole::WriteString( SPSTR( "Двигатель: Выключен" ), CConsole::cp1251 );

    do {


    } while ( true );

    // Все проверки прошли успешно, объект в рабочем состоянии.
    return NO_ERROR;

}


/**
 * Инициализация всей периферии
 */
void CMCU::Initialization(){

    // Инициализация после сброса.

    // Запрещаем все прерывания.
    __disable_interrupt();

    // Устанавливаем начальный режим работы аналогового компаратора.
    ACMode = acmFirstMeasurement;

    PCMode = pcmDisabled;

    // Текущая измеряемая точка.
    SamplePoint = 0;

    // Начальное значение счётчика таймера в каждой измеряемой точке.
    PointValues[0] = 0U;
    PointValues[1] = 0U;
    PointValues[2] = 0U;

    // Схема соединений (разводка выводов) [ATtiny2313]
    PortsInit();

    // Настройка таймера/счётчика 0 [ATtiny2313]
    Timer0Init();

    // Настройка таймера/счётчика 1 [ATtiny2313]
    Timer1Init();

    // Настройка внутреннего сторожевого таймера [ATtiny2313]
    //InternalWDTInit();

    // Настройка внутреннего USART [ATtiny2313]
    USARTInit();

    // Настройка аналогового компаратора [ATtiny2313]
    AnalogComparatorInit();

    // Настройка последовательного интерфейса USI [ATtiny2313]
    //USIInit();

    // Настройка работы с внешними прерываниями [ATtiny2313]
    ExternalInterruptsInit();

    // Настройка работы с прерываниями от выводов [ATtiny2313]
    //PinChangeInterruptsInit();

    // Настройка управляющих регистров контроллера [ATtiny2313]
    ControlRegistersInit();

}


/**
 * Настройка управляющих регистров контроллера.
 */
void CMCU::ControlRegistersInit(){

    uint8_t temp;

    // MCU Control Register
    // [ Регистр управления микроконтроллером ][ATtiny2313]
    //          00000000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
    //          ||||||+-- 1, rw, ISC01: _|
    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
    //          ||||+---- 3, rw, ISC11: _|
    //          |||+----- 4, rw, SM0:      - Sleep Mode Select Bits 0
    //          ||+------ 5, rw, SE:       - Sleep Enable
    //          |+------- 6, rw, SM1:      - Sleep Mode Select Bits 1
    //          +-------- 7, rw, PUD:      - Pull-up disable
    // Примечание:
    // Эти конструкции сохраняют младшую тетраду регистра MCUCR от случайного
    // изменения
    //temp &= ( 1 << SM1 ) | ( 1 << SM0 );
    MCUCR &= ~( ( 1 << SE ) | ( 1 << SM1 ) | ( 1 << SM0 ) );
    MCUCR |= temp;


    // Timer/Counter Interrupt Mask Register
    // [ Регистр маски прерываний от таймеров/счётчиков ][ATtiny2313]
    //           00000000 - Initial Value
    TIMSK = BIN8(00000010); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, OCIE0A: - Timer/Counter0 Output Compare Match A Interrupt Enable
    //           ||||||+-- 1, rw, TOIE0:  - Timer/Counter0 Overflow Interrupt Enable
    //           |||||+--- 2, rw, OCIE0B: - OCIE0B: Timer/Counter0 Output Compare Match B Interrupt Enable
    //           ||||+---- 3, rw, ICIE1:  - Timer/Counter1, Input Capture Interrupt Enable
    //           |||+----- 4, r: 0
    //           ||+------ 5, rw, OCIE1B: - Timer/Counter1, Output Compare B Match Interrupt Enable
    //           |+------- 6, rw, OCIE1A: - Timer/Counter1, Output Compare A Match Interrupt Enable
    //           +-------- 7, rw, TOIE1:  - Timer/Counter1, Overflow Interrupt Enable
    // Примечание:


    // Timer/Counter Interrupt Flag Register
    // [ Регистр флагов прерываний таймеров/счётчиков ][ATtiny2313]
    //          00000000 - Initial Value
    //TIFR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, OCF0A: - Output Compare Flag 0 A
    //          ||||||+-- 1, rw, TOV0:  - Timer/Counter0 Overflow Flag
    //          |||||+--- 2, rw, OCF0B: - Output Compare Flag 0 B
    //          ||||+---- 3, rw, ICF1:  - Timer/Counter1, Input Capture Flag
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, OCF1B: - Timer/Counter1, Output Compare B Match Flag
    //          |+------- 6, rw, OCF1A: - Timer/Counter1, Output Compare A Match Flag
    //          +-------- 7, rw, TOV1:  - Timer/Counter1, Overflow Flag
    // Примечание:


}


/**
 * -=[ Схема соединений (разводка выводов) MCU ATtiny2313 ]=-
 */
void CMCU::PortsInit(){

    // Параметры проекта:
    // Имя проекта: Шаблон для ATtiny2313
    // Файл схемы (URI): file:///
    // Хеш файла схемы (MD5):
    // Код:
    // Дата: 16.04.2013
    // Координаты разработчика:
    //  mailto:unihomelab@ya.ru
    //  skype: viacheslavmezentsev

//                 [ATtiny2313]
//
//
//
//                            PDIP/SOIC
//                        +--------------+
//       (~RESET/dW) PA2 -|  1        20 |- VCC
//             (RXD) PD0 -|  2        19 |- PB7 (UCSK/SCL/PCINT7)
//             (TXD) PD1 -|  3        18 |- PB6 (MISO/DO/PCINT6)
//           (XTAL2) PA1 -|  4        17 |- PB5 (MOSI/DI/SDA/PCINT5)
//           (XTAL1) PA0 -|  5        16 |- PB4 (OC1B/PCINT4)
//  (CKOUT/XCK/INT0) PD2 -|  6        15 |- PB3 (OC1A/PCINT3)
//            (INT1) PD3 -|  7        14 |- PB2 (OC0A/PCINT2)
//              (T0) PD4 -|  8        13 |- PB1 (AIN1/PCINT1)
//         (OC0B/T1) PD5 -|  9        12 |- PB0 (AIN0/PCINT0)
//                   GND -| 10        11 |- PD6 (ICP)
//                        +--------------+
//
//
//
//
//
//


    // Table. Port Pin Configurations
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //| DDxn|PORTxn|   PUD    |  I/O   | Pull-up |   Comment                                  |
    //|     |      |(in SFIOR)|        |         |                                            |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|  0  |  0   |    X     | Input  |   No    | Tri-state (Hi-Z)                           |
    //|  0  |  1   |    0     | Input  |   Yes   | Pxn will source current if ext. pulled low.|
    //|  0  |  1   |    1     | Input  |   No    | Tri-state (Hi-Z)                           |
    //|  1  |  0   |    X     | Output |   No    | Output Low (Sink)                          |
    //|  1  |  1   |    X     | Output |   No    | Output High (Source)                       |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    // Если же бит PUD установлен в 1, подтягивающие
    // резисторы отключаются от всех выводов микроконтроллера.

    //sbi( MCUCR, PUD );
    cbi( MCUCR, PUD );  // Начальное значение бита.

    // Настройка портов: A, B, D. Начальная инициализация уровней.

    // Port A Data Direction Register
    // [ Регистр направления порта A ][ATtiny2313]
    //          00000000 - Initial Value
    DDRA = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDA0: (XTAL1)     -
    //          ||||||+-- 1, rw, DDA1: (XTAL2)     -
    //          |||||+--- 2, rw, DDA2: (RESET, dW) -
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, r: 0
    //          |+------- 6, r: 0
    //          +-------- 7, r: 0
    // Примечание:


    // Port A Data Register
    // [ Регистр данных порта A ][ATtiny2313]
    //           00000000 - Initial Value
    PORTA = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTA0: (XTAL1)     -
    //           ||||||+-- 1, rw, PORTA1: (XTAL2)     -
    //           |||||+--- 2, rw, PORTA2: (RESET, dW) -
    //           ||||+---- 3, r: 0
    //           |||+----- 4, r: 0
    //           ||+------ 5, r: 0
    //           |+------- 6, r: 0
    //           +-------- 7, r: 0
    // Примечание:


    // Port B Data Direction Register
    // [ Регистр направления порта B ][ATtiny2313]
    //          00000000 - Initial Value
    DDRB = BIN8(00001000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDB0: (AIN0/PCINT0)     -
    //          ||||||+-- 1, rw, DDB1: (AIN1/PCINT1)     -
    //          |||||+--- 2, rw, DDB2: (OC0A/PCINT2)     -
    //          ||||+---- 3, rw, DDB3: (OC1A/PCINT3)     - Управление симистором.
    //          |||+----- 4, rw, DDB4: (OC1B/PCINT4)     -
    //          ||+------ 5, rw, DDB5: (DI/SDA/PCINT5)   -
    //          |+------- 6, rw, DDB6: (DO/PCINT6)       -
    //          +-------- 7, rw, DDB7: (USCK/SCL/PCINT7) -
    // Примечание:


    // Port B Data Register
    // [ Регистр данных порта B ][ATtiny2313]
    //           00000000 - Initial Value
    PORTB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTB0: (AIN0/PCINT0)     -
    //           ||||||+-- 1, rw, PORTB1: (AIN1/PCINT1)     -
    //           |||||+--- 2, rw, PORTB2: (OC0A/PCINT2)     -
    //           ||||+---- 3, rw, PORTB3: (OC1A/PCINT3)     - Управление симистором.
    //           |||+----- 4, rw, PORTB4: (OC1B/PCINT4)     -
    //           ||+------ 5, rw, PORTB5: (DI/SDA/PCINT5)   -
    //           |+------- 6, rw, PORTB6: (DO/PCINT6)       -
    //           +-------- 7, rw, PORTB7: (USCK/SCL/PCINT7) -
    // Примечание:


    // Port D Data Direction Register
    // [ Регистр направления порта D ][ATtiny2313]
    //          00000000 - Initial Value
    DDRD = BIN8(00000010); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDD0: (RXD)            - RXD
    //          ||||||+-- 1, rw, DDD1: (TXD)            - TXD
    //          |||||+--- 2, rw, DDD2: (INT0/XCK/CKOUT) -
    //          ||||+---- 3, rw, DDD3: (INT1)           -
    //          |||+----- 4, rw, DDD4: (T0)             -
    //          ||+------ 5, rw, DDD5: (OC0B/T1)        -
    //          |+------- 6, rw, DDD6: (ICP)            -
    //          +-------- 7, r: 0
    // Примечание:


    // Port D Data Register
    // [ Регистр данных порта D ][ATtiny2313]
    //           00000000 - Initial Value
    PORTD = BIN8(00000001); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTD0: (RXD)            - RXD
    //           ||||||+-- 1, rw, PORTD1: (TXD)            - TXD
    //           |||||+--- 2, rw, PORTD2: (INT0/XCK/CKOUT) -
    //           ||||+---- 3, rw, PORTD3: (INT1)           -
    //           |||+----- 4, rw, PORTD4: (T0)             -
    //           ||+------ 5, rw, PORTD5: (OC0B/T1)        -
    //           |+------- 6, rw, PORTD6: (ICP)            -
    //           +-------- 7, r: 0
    // Примечание:

}


/**
 * Настройка таймера/счётчика 0
 */
void CMCU::Timer0Init(){

    // [ATtiny2313] Table. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     ¦
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source. (Таймер/счетчик остановлен)                    |
    // | 0  | 0  | 1  | clkT0S/(No prescaling)                                          |
    // | 0  | 1  | 0  | clkT0S/8    (From prescaler)                                    |
    // | 0  | 1  | 1  | clkT0S/64   (From prescaler)                                    |
    // | 1  | 0  | 0  | clkT0S/256  (From prescaler)                                    |
    // | 1  | 0  | 1  | clkT0S/1024 (From prescaler)                                    |
    // | 1  | 1  | 0  | External clock source on T0 pin. Clock on falling edge.         |
    // | 1  | 1  | 1  | External clock source on T0 pin. Clock on rising edge.          |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter 0 Control Register B
    // [ Регистр управления B Таймером/Счётчиком 0 ][ATtiny2313]
    //            00000000 - Initial Value
    TCCR0B = BIN8(00000011); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, CS00:  -+ - Управление тактовым сигналом
    //            ||||||+-- 1, rw, CS01:   |
    //            |||||+--- 2, rw, CS02:  _|
    //            ||||+---- 3, rw, WGM02:    - Waveform Generation Mode
    //            |||+----- 4, r: 0
    //            ||+------ 5, r: 0
    //            |+------- 6, rw, FOC0B:    - Force Output Compare B
    //            +-------- 7, rw, FOC0A:    - Force Output Compare A
    // Примечание:

    // Устанавливаем значения для счётного регистра
    TCNT0 = 0xFF - F_CPU / ( 64UL * 1000UL );

    // Timer/Counter 0 Output Compare Register A
    //OCR0A = 0x00;

    // Timer/Counter 0 Output Compare Register B
    //OCR0B = 0x00;

    // Timer/Counter 0 Control Register A
    // [ Регистр управления A Таймером/Счётчиком 0 ][ATtiny2313]
    //            00000000 - Initial Value
    //TCCR0A = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, WGM00:  -+ - Waveform Generation Mode
    //            ||||||+-- 1, rw, WGM01:  _|
    //            |||||+--- 2, r: 0
    //            ||||+---- 3, r: 0
    //            |||+----- 4, rw, COM0B0: -+ - Режим работы канала сравнения B
    //            ||+------ 5, rw, COM0B1: _|
    //            |+------- 6, rw, COM0A0: -+ - Режим работы канала сравнения A
    //            +-------- 7, rw, COM0A1: _|
    // Примечание:


}


/**
 * Настройка таймера/счётчика 1
 */
void CMCU::Timer1Init(){

    // [ATtiny2313] Table. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     ¦
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source. (Таймер/счетчик остановлен)                    |
    // | 0  | 0  | 1  | clkI/O/1    (No prescaling)                                     |
    // | 0  | 1  | 0  | clkI/O/8    (From prescaler)                                    |
    // | 0  | 1  | 1  | clkI/O/64   (From prescaler)                                    |
    // | 1  | 0  | 0  | clkI/O/256  (From prescaler)                                    |
    // | 1  | 0  | 1  | clkI/O/1024 (From prescaler)                                    |
    // | 1  | 1  | 0  | Вывод Тn, счет осуществляется по спадающему фронту импульсов    |
    // | 1  | 1  | 1  | Вывод Тn, счет осуществляется по нарастающему фронту импульсов  |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter1 Control Register B
    // [ Регистр управления B Таймером/Счётчиком 1 ][ATtiny2313]
    //            00000000 - Initial Value
    TCCR1B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, CS10:  -+ - Управление тактовым сигналом
    //            ||||||+-- 1, rw, CS11:   |
    //            |||||+--- 2, rw, CS12:  _|
    //            ||||+---- 3, rw, WGM12: -+ - Режим работы таймера/счетчика
    //            |||+----- 4, rw, WGM13: _|
    //            ||+------ 5, r: 0          - зарезервирован, должен быть установлен в 0 при записи
    //            |+------- 6, rw, ICES1:    - Выбор активного фронта сигнала захвата
    //            +-------- 7, rw, ICNC1:    - Управление схемой подавления помех блока захвата
    // Примечание:

    // Устанавливаем значения для счётных регистров
    TCNT1H = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue ) >> 8
    TCNT1L = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue )
    /*
    OCR1AH = 0x00;
    OCR1AL = 0x39;

    OCR1BH = 0x00;
    OCR1BL = 0x39;

    OCR1CH = 0x00;
    OCR1CL = 0x39;

    ICR1H  = 0x00;
    ICR1L  = 0x39;
    */
    // Timer/Counter1 Control Register A
    // [ Регистр управления A Таймером/Счётчиком 1 ][ATtiny2313]
    //            00000000 - Initial Value
    TCCR1A = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, WGM10:  -+ - Режим работы таймера/счетчика
    //            ||||||+-- 1, rw, WGM11:  _|
    //            |||||+--- 2, r: 0
    //            ||||+---- 3, r: 0
    //            |||+----- 4, rw, COM1B0: -+ - Режим работы канала сравнения B
    //            ||+------ 5, rw, COM1B1: _|
    //            |+------- 6, rw, COM1A0: -+ - Режим работы канала сравнения A
    //            +-------- 7, rw, COM1A1: _|
    // Примечание:


    // Timer/Counter1 Control Register C
    // [ Регистр управления C Таймером/Счётчиком 1 ][ATtiny2313]
    //            00000000 - Initial Value
    //TCCR1C = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, r: 0
    //            ||||||+-- 1, r: 0
    //            |||||+--- 2, r: 0
    //            ||||+---- 3, r: 0
    //            |||+----- 4, r: 0
    //            ||+------ 5, r: 0
    //            |+------- 6, rw, FOC1B: - Force Output Compare for Channel B
    //            +-------- 7, rw, FOC1A: - Force Output Compare for Channel A
    // Примечание:

}


/**
 * Настройка последовательного интерфейса USI
 */
void CMCU::USIInit(){

    // TWI Bit Rate Register
    //TWBR = 1; // TODO: Написать формулу для настройки скорости

    // TWI Control Register
    // [ Регистр управления TWI ][ATmega16]
    //          00000000 - Initial Value
    //TWCR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, TWIE:  - TWI Interrupt Enable
    //          ||||||+-- 1, r:         - reserved (will always read as zero)
    //          |||||+--- 2, rw, TWEN:  - TWI Enable Bit
    //          ||||+---- 3, r,  TWWC:  - TWI Write Collision Flag
    //          |||+----- 4, rw, TWSTO: - TWI STOP Condition Bit
    //          ||+------ 5, rw, TWSTA: - TWI START Condition Bit
    //          |+------- 6, rw, TWEA:  - TWI Enable Acknowledge Bit
    //          +-------- 7, rw, TWINT: - TWI Interrupt Flag
    // Примечание:

    // TWI Status Register
    // [ TWI статус регистр ][ATmega16]
    //          11111000 - Initial Value
    //TWSR = BIN8(11111000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, TWPS0: -+ - TWI Prescaler Bits
    //          ||||||+-- 1, rw, TWPS1: _|
    //          |||||+--- 2, r:            - reserved (will always read as zero)
    //          ||||+---- 3, r, TWS3:   -+ - TWI Status
    //          |||+----- 4, r, TWS4:    |
    //          ||+------ 5, r, TWS5:    |
    //          |+------- 6, r, TWS6:    |
    //          +-------- 7, r, TWS7:   _|
    // Примечание:

    // TWI (Slave) Address Register
    // Иметь в виду, что регистр содержит бит TWGCE (TWI General Call Recognition Enable Bit)
    //TWAR = 0xFE;

}


/**
 * Настройка внутреннего USART
 */
void CMCU::USARTInit(){

    // USART Control and Status Register A
    // [ Регистр управления UCSRA ][ATtiny2313]
    //           00100000 - Initial Value
    UCSRA = BIN8(00100000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, MPCM: - Multi-processor Communication Mode
    //           ||||||+-- 1, rw, U2X:  - Double the USART Transmission Speed
    //           |||||+--- 2, r, UPE:   - USART Parity Error
    //           ||||+---- 3, r, DOR:   - Data OverRun
    //           |||+----- 4, r, FE:    - Frame Error
    //           ||+------ 5, r, UDRE:  - USART Data Register Empty
    //           |+------- 6, rw, TXC:  - USART Transmit Complete
    //           +-------- 7, r, RXC:   - USART Receive Complete
    // Примечание:

    UCSRB = 0x00; // отключаем, пока настраиваем скорость

    // Определение BAUD см. в файле: "Configuration.h"
    UBRRL = ( uint8_t ) ( F_CPU / ( 16UL * BAUD ) - 1UL ); // устанавливаем скорость
    UBRRH = ( uint8_t ) ( ( F_CPU / ( 16UL * BAUD ) - 1UL ) >> 8 );

    // USART Control and Status Register B
    // [ Регистр управления UCSRB ][ATtiny2313]
    //           00000000 - Initial Value
    UCSRB = BIN8(10011000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, TXB8:  - Transmit Data Bit 8
    //           ||||||+-- 1, r,  RXB8:  - Receive Data Bit 8
    //           |||||+--- 2, rw, UCSZ2: - Character Size
    //           ||||+---- 3, rw, TXEN:  - Transmitter Enable
    //           |||+----- 4, rw, RXEN:  - Receiver Enable
    //           ||+------ 5, rw, UDRIE: - USART Data Register Empty Interrupt Enable
    //           |+------- 6, rw, TXCIE: - TX Complete Interrupt Enable
    //           +-------- 7, rw, RXCIE: - RX Complete Interrupt Enable
    // Примечание:

    // USART Control and Status Register C
    // [ Регистр управления UCSRC ][ATtiny2313]
    //           00000110 - Initial Value
    //UCSRC = BIN8(00000110); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, UCPOL:    - Clock Polarity
    //           ||||||+-- 1, rw, UCSZ0: -+ - Character Size
    //           |||||+--- 2, rw, UCSZ1: _|
    //           ||||+---- 3, rw, USBS:     - Stop Bit Select
    //           |||+----- 4, rw, UPM0:  -+ - Parity Mode
    //           ||+------ 5, rw, UPM1:  _|
    //           |+------- 6, rw, UMSEL:    - USART Mode Select
    //           +-------- 7, r: 0
    // Примечание:

}


/**
 * Настройка аналогового компаратора
 */
void CMCU::AnalogComparatorInit(){

    // Table. Analog Comparator Interrupt Mode Select (ACIS1/ACIS0 Settings)
    // +-----+------+----------------------------------------------+
    // |ACIS1|ACIS0 |            Interrupt Mode                    |
    // +-----+------+----------------------------------------------+
    // |  0  |  0   | Comparator Interrupt on Output Toggle.       |
    // |  0  |  1   | Reserved                                     |
    // |  1  |  0   | Comparator Interrupt on Falling Output Edge. |
    // |  1  |  1   | Comparator Interrupt on Rising Output Edge.  |
    // +-----+------+----------------------------------------------+

    // Analog Comparator Control and Status Register
    // [ Регистр управления ACSR ][ATtiny2313]
    //          00?00000 - Initial Value
    ACSR = BIN8(00001000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, ACIS0: -+ - Analog Comparator Interrupt Mode Select
    //          ||||||+-- 1, rw, ACIS1: _|
    //          |||||+--- 2, rw, ACIC:     - Analog Comparator Input Capture Enable
    //          ||||+---- 3, rw, ACIE:     - Analog Comparator Interrupt Enable
    //          |||+----- 4, rw, ACI:      - Analog Comparator Interrupt Flag
    //          ||+------ 5, r, ACO:       - Analog Comparator Output
    //          |+------- 6, rw, ACBG:     - Analog Comparator Bandgap Select
    //          +-------- 7, rw: ACD       - Analog Comparator Disable
    // Примечание:

    // When changing the ACD bit, the Analog Comparator Interrupt must be
    // disabled by clearing the ACIE bit in ACSR. Otherwise an interrupt can
    // occur when the bit is changed.


    // Digital Input Disable Register
    // [ Регистр выключения цифровых входов ][ATtiny2313]
    //          00000000 - Initial Value
    DIDR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, AIN0D: -+ - AIN1, AIN0 Digital Input Disable
    //          ||||||+-- 1, rw, AIN1D: _|
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, r: 0
    //          |+------- 6, r: 0
    //          +-------- 7, r: 0
    // Примечание:

}


/**
 * Настройка внутреннего сторожевого таймера
 */
void CMCU::InternalWDTInit(){

    // [ATmega16] Table. Watchdog Timer Prescale Select
    // +----+----+----+------------------+---------+---------+
    // |WDP2|WDP1|WDP0|OscCycles         | VCC3.0V | VCC5.0V |
    // +----+----+----+------------------+---------+---------+
    // | 0  | 0  | 0  |16K (16,384)      | 14.8 ms | 14.0 ms |
    // | 0  | 0  | 1  |32K (32,768)      | 29.6 ms | 28.1 ms |
    // | 0  | 1  | 0  |64K (65,536)      | 59.1 ms | 56.2 ms |
    // | 0  | 1  | 1  |128K (131,072)    | 0.12 s  | 0.11 s  |
    // | 1  | 0  | 0  |256K (262,144)    | 0.24 s  | 0.22 s  |
    // | 1  | 0  | 1  |512K (524,288)    | 0.47 s  | 0.45 s  |
    // | 1  | 1  | 0  |1,024K (1,048,576)| 0.95 s  | 0.9 s   |
    // | 1  | 1  | 1  |2,048K (2,097,152)| 1.9 s   | 1.8 s   |
    // +----+----+----+------------------+---------+---------+

    // Watchdog Timer ControlRegister
    // [ Регистр управления сторожевого таймера ][ATmega16]
    //           00000000 - Initial Value
    //WDTCR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, WDP0: -+ - Коэффициент деления предделителя сторожевого таймера
    //           ||||||+-- 1, rw, WDP1:  |
    //           |||||+--- 2, rw, WDP2: _|
    //           ||||+---- 3, rw, WDE:     - Разрешение сброса по тайм-ауту сторожевого таймера
    //           |||+----- 4, rw, WDCE:    - Разрешение изменения конфигурации сторожевого таймера
    //           ||+------ 5, r,        -+ - Reserved Bits
    //           |+------- 6, r,         |
    //           +-------- 7, r,        _|
    // Примечание:
    // Включен сторожевой таймер. Далее в коде его необходимо сбрасывать
    // через промежутки времени меньшие периода его генератора с учётом
    // коэффициентов деления.

}


/**
 * Настройка работы с внешними прерываниями
 */
void CMCU::ExternalInterruptsInit(){

    uint8_t temp;

    // Table 48. Interrupt Sense Control
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|ISCn1|ISCn0 | Description                                                              |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|  0  |  0   | The low level of INTn generates an interrupt request.                    |
    //|  0  |  1   | Reserved                                                                 |
    //|  1  |  0   | The falling edge of INTn generates asynchronously an interrupt request.  |
    //|  1  |  1   | The rising edge of INTn generates asynchronously an interrupt request.   |
    //+-----+------+----------+--------+---------+--------------------------------------------+

    // Запрещаем внешние прерывания INT0 и INT1.
    cbi( GIMSK, INT0 );
    cbi( GIMSK, INT1 );

    // Настройка прерываний INT0 и INT1
    // MCU Control Register
    // [ Регистр управления микроконтроллером ][ATtiny2313]
    //          00000000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
    //          ||||||+-- 1, rw, ISC01: _|
    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
    //          ||||+---- 3, rw, ISC11: _|
    //          |||+----- 4, rw, SM0:      - Sleep Mode Select Bits 0
    //          ||+------ 5, rw, SE:       - Sleep Enable
    //          |+------- 6, rw, SM1:      - Sleep Mode Select Bits 1
    //          +-------- 7, rw, PUD:      - Pull-up disable
    // Примечание:
    // Эти конструкции сохраняют биты регистра MCUCR от случайного изменения.
    //temp &= ( 1 << ISC11 ) | ( 1 << ISC01 );
    MCUCR &= ~( ( 1 << ISC11 ) | ( 1 << ISC10 ) | ( 1 << ISC01 ) | ( 1 << ISC00 ) );
    MCUCR |= temp;

    // General Interrupt Mask Register
    // [ Общий регистр управлением прерываниями ][ATtiny2313]
    //          00000000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, r: 0
    //          ||||||+-- 1, r: 0
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, PCIE: - Pin Change Interrupt Enable
    //          |+------- 6, rw, INT0: - External Interrupt Request 0 Enable
    //          +-------- 7, rw, INT1: - External Interrupt Request 1 Enable
    // Примечание:
    // Эти конструкции сохраняют биты регистра GIMSK от случайного изменения.
    //temp &= ( 1 << INT0 ) | ( 1 << INT1 );
    GIMSK &= ~( ( 1 << INT0 ) | ( 1 << INT1 ) );
    GIMSK |= temp;


    // External Interrupt Flag Register
    // [ Регистр флагов внешних прерываний ][ATtiny2313]
    //          00000000 - Initial Value
    //EIFR = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, r: 0
    //          ||||||+-- 1, r: 0
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, PCIF:  - Pin Change Interrupt Flag
    //          |+------- 6, rw, INTF0: - External Interrupt Flag 0
    //          +-------- 7, rw, INTF1: - External Interrupt Flag 1
    // Примечание:

}


/**
 * Настройка работы с прерываниями от выводов
 */
void CMCU::PinChangeInterruptsInit(){

    uint8_t temp;

    // Запрещаем прерывание PCIE.
    cbi( GIMSK, PCIE );

    // General Interrupt Mask Register
    // [ Общий регистр управлением прерываниями ][ATtiny2313]
    //          00000000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, r: 0
    //          ||||||+-- 1, r: 0
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, PCIE: - Pin Change Interrupt Enable
    //          |+------- 6, rw, INT0: - External Interrupt Request 0 Enable
    //          +-------- 7, rw, INT1: - External Interrupt Request 1 Enable
    // Примечание:
    // Эти конструкции сохраняют биты регистра GIMSK от случайного изменения.
    //temp &= ( 1 << PCIE );
    GIMSK &= ~( ( 1 << PCIE ) );
    GIMSK |= temp;


    // Pin Change Mask Register
    // [ Регистр маскирования прерываний от выводов ]
    //           00000000 - Initial Value
    PCMSK = BIN8(00000000);
    //           ||||||||
    //           |||||||+- 0, rw, PCINT0: (PB0) -+ - Pin Change Enable Mask 0-7
    //           ||||||+-- 1, rw, PCINT1: (PB1)  |
    //           |||||+--- 2, rw, PCINT2: (PB2)  |
    //           ||||+---- 3, rw, PCINT3: (PB3)  |
    //           |||+----- 4, rw, PCINT4: (PB4)  |
    //           ||+------ 5, rw, PCINT5: (PB5)  |
    //           |+------- 6, rw, PCINT6: (PB6)  |
    //           +-------- 7, rw, PCINT7: (PB7) _|
    // Примечание:


    // External Interrupt Flag Register
    // [ Регистр флагов внешних прерываний ][ATtiny2313]
    //          00000000 - Initial Value
    //EIFR = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, r: 0
    //          ||||||+-- 1, r: 0
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, PCIF:  - Pin Change Interrupt Flag
    //          |+------- 6, rw, INTF0: - External Interrupt Flag 0
    //          +-------- 7, rw, INTF1: - External Interrupt Flag 1
    // Примечание:

}


// -=[ Обработчики прерываний ]=-

/**
 * External Interrupt Request 0
 */
void CMCU::OnExternalInterruptRequest0(){

}


/**
 * External Interrupt Request 1
 */
void CMCU::OnExternalInterruptRequest1(){

}


/**
 * Timer/Counter1 Capture Event
 */
void CMCU::OnTimerCounter1CaptureEvent(){

}


/**
 * Timer/Counter1 Compare Match A
 */
void CMCU::OnTimerCounter1CompareMatchA(){

    uint16_t temp = 0;

    switch ( PCMode ) {

        case pcmDisabled: {

            temp = HalfPeriod >> 1;

            // Задаём поведение вывода OC1A в режиме CTC.
            TCCR1A = ( 1 << COM1A1 ) | ( 1 << COM1A0 );

            PCMode = pcmEnabledUp;

            break;
        }

        case pcmEnabledUp: {

            // Длительность импульса включения симистора.
            temp = ( PulseWidth * ( F_CPU / 1000000UL ) ) / 8U;

            PCMode = pcmEnabledDown;

            // Задаём поведение вывода OC1A в режиме CTC.
            TCCR1A = ( 1 << COM1A1 );

            break;
        }

        case pcmEnabledDown: {

            temp = ( HalfPeriod >> 1 ) - ( PulseWidth * ( F_CPU / 1000000UL ) ) / 8U;

            PCMode = pcmDisabled;

            // Задаём поведение вывода OC1A в режиме CTC.
            TCCR1A = ( 1 << COM1A1 );

            break;
        }

    }

    OCR1AH = temp >> 8;
    OCR1AL = temp & 0xFF;

}


/**
 * Timer/Counter1 Overflow
 */
void CMCU::OnTimerCounter1Overflow(){

}


/**
 * Timer/Counter0 Overflow
 */
void CMCU::OnTimerCounter0Overflow(){

    Counter10ms++;

    // Восстанавливаем счётчик
    TCNT0 = 0xFF - F_CPU / 64000UL;

    if ( Counter10ms == 10 ) {

        Counter10ms = 0;

        // Формирование события.
        // ...

    }

}


/**
 * USART, Rx Complete
 */
void CMCU::OnUSARTRxComplete( uint8_t data ){

    if ( !FIFO_IS_FULL( uart_rx_fifo ) ) {

        // Добавляем однократную задержку для таймера.
        // Это нужно для того, чтобы таймер не сработал между моментами
        // принятия очередного байта в коротких посылках.
        TCNT0 = 3 * ( BAUD / 10000UL ) * ( 0xFF - F_CPU / 64000UL );

        FIFO_PUSH( uart_rx_fifo, data );
    }

}


/**
 * USART Data Register Empty
 */
void CMCU::OnUSARTDataRegisterEmpty(){

}


/**
 * USART, Tx Complete
 */
void CMCU::OnUSARTTxComplete(){

}


/**
 * Analog Comparator
 */
void CMCU::OnAnalogComparator(){

    switch ( ACMode ) {

        // Режим первого измерения.
        case acmFirstMeasurement: {

            // При первом срабатывании компаратора проверяем положение точки.
            if ( SamplePoint == 0 ) {

                // Если обнаружена вторая точка, то пропускаем её.
                if ( CHECKBIT( ACSR, ACO ) ) return;

                // Отключаем таймер/счётчик 1 перед настройкой.
                TCCR1B = 0x00;

                // Устанавливаем на время - 25 мсек.
                TCNT1H = ( 0xFFFF - ( ( 25UL * ( F_CPU / 1000000UL ) ) / 8UL ) * 1000U ) >> 8;
                TCNT1L = ( 0xFFFF - ( ( 25UL * ( F_CPU / 1000000UL ) ) / 8UL ) * 1000U ) & 0xFF;

                // Сохраняем значение счётчика таймера 1 для текущей точки.
                // Внимание. Порядок считывания байт регистра счётчика важен.
                PointValues[ SamplePoint++ ] = TCNT1L + ( TCNT1H << 8 );

                // Запускаем таймер/счётчик 1.
                TCCR1B |= ( 1 << CS11 );

                // Разрешаем прерывание по переполнению.
                sbi( TIMSK, TOIE1 );

            } else if ( SamplePoint == 1 ) {

                // Сохраняем значение счётчика таймера 1 для текущей точки.
                PointValues[ SamplePoint++ ] = TCNT1L + ( TCNT1H << 8 );

            } else if ( SamplePoint == 2 ) {

                // Сохраняем значение счётчика таймера 1 для текущей точки.
                PointValues[ SamplePoint ] = TCNT1L + ( TCNT1H << 8 );

                SamplePoint = 0;

                // Вычисляем параметры для рабочего режима.
                uint16_t temp = PointValues[0];

                HalfPeriod = ( PointValues[2] - temp ) / 2U;
                
                // Время, оставшееся до ближайшего пересечения нуля.
                temp = ( HalfPeriod +  ( PointValues[1] - temp ) ) / 2U;

                // Перенастраиваем таймер/счётчик 1.
                TCCR1B = 0x00;

                // Обнуляем счётчик.
                TCNT1H = 0;
                TCNT1L = 0;

                // Задаём порог срабатывания.
                OCR1AH = temp >> 8;
                OCR1AL = temp & 0xFF;

                // Задаём поведение вывода OC1A в режиме CTC.
                TCCR1A = ( 1 << COM1A1 );

                // Запускаем таймер/счётчик 1 в режиме CTC.
                TCCR1B |= ( 1 << WGM12 ) | ( 1 << CS11 );

                // Разрешаем прерывание по сравнению.
                sbi( TIMSK, OCIE1A );

                // Переводим компаратор в рабочий режим.
                ACMode = acmWorkMode;

                // Устанавливаем начальное значение для формирователя импульса
                // управления симистором.
                PCMode = pcmDisabled;

            }

            break;

        }

        case acmWorkMode: {

            // Доделать: Корректировка положения точки пересечения нуля в каждом
            // периоде источника.

            //...

            break;
        }

    }

}


/**
 * Pin Change Interrupt
 */
void CMCU::OnPinChangeInterrupt(){

}


/**
 * Timer/Counter1 Compare Match B
 */
void CMCU::OnTimerCounter1CompareMatchB(){

}


/**
 * Timer/Counter0 Compare Match A
 */
void CMCU::OnTimerCounter0CompareMatchA(){

}


/**
 * Timer/Counter0 Compare Match B
 */
void CMCU::OnTimerCounter0CompareMatchB(){

}


/**
 * USI Start Condition
 */
void CMCU::OnUSIStartCondition(){

}


/**
 * USI Overflow
 */
void CMCU::OnUSIOverflow(){

}


/**
 * EEPROM Ready
 */
void CMCU::OnEEPROMReady(){

}


/**
 * Watchdog Timer Overflow
 */
void CMCU::OnWatchdogTimerOverflow(){

}


// -=[ Процедуры обработки прерываний ] [ATtiny2313]=-

/**
* Внимание: При входе в обработчик прерывания флаг IE в SREG
* сбрасывается для запрещения вложенных прерываний.
*
* Для сохранения и восстановления SREG можно использовать
* следующие конструкции:
*
* // Сохраняем SREG
* uint8_t tmp = __save_interrupt();
*
* // ...
*
* // Восстанавливаем SREG
* __restore_interrupt( tmp );
*/


/**
 * External Interrupt Request 0
 */
#ifdef External_Interrupt_Request_0

    #ifdef __GNUC__

        ISR( INT0_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT0_vect
        __interrupt void ExternalInterruptRequest0() {

    #endif

        CMCU::OnExternalInterruptRequest0();

    } // External Interrupt Request 0

#endif


/**
 * External Interrupt Request 1
 */
#ifdef External_Interrupt_Request_1

    #ifdef __GNUC__

        ISR( INT1_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = INT1_vect
        __interrupt void ExternalInterruptRequest1() {

    #endif

        CMCU::OnExternalInterruptRequest1();

    } // External Interrupt Request 1

#endif


/**
 *  Timer/Counter1 Capture Event
 */
#ifdef Timer_Counter1_Capture_Event

    #ifdef __GNUC__

        ISR( TIMER1_CAPT_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_CAPT_vect
        __interrupt void TimerCounter1CaptureEvent() {

    #endif

        CMCU::OnTimerCounter1CaptureEvent();

    } // Timer/Counter1 Capture Event

#endif


/**
 *  Timer/Counter1 Compare Match A
 */
#ifdef Timer_Counter1_Compare_Match_A

    #ifdef __GNUC__

        ISR( TIMER1_COMPA_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_COMPA_vect
        __interrupt void TimerCounter1CompareMatchA() {

    #endif

        CMCU::OnTimerCounter1CompareMatchA();

    } // Timer/Counter1 Compare Match A

#endif


/**
 *  Timer/Counter1 Overflow
 */
#ifdef Timer_Counter1_Overflow

    #ifdef __GNUC__

        ISR( TIMER1_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_OVF1_vect
        __interrupt void TimerCounter1Overflow() {

    #endif

        CMCU::OnTimerCounter1Overflow();

    } // Timer/Counter1 Overflow

#endif


/**
 *  Timer/Counter0 Overflow
 */
#ifdef Timer_Counter0_Overflow

    #ifdef __GNUC__

        ISR( TIMER0_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_OVF0_vect
        __interrupt void TimerCounter0Overflow() {

    #endif

        CMCU::OnTimerCounter0Overflow();

    } // Timer/Counter0 Overflow

#endif


/**
 *  USART, Rx Complete
 */
#ifdef USART_Rx_Complete

    #ifdef __GNUC__

        ISR( USART_RX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_RX_vect
        __interrupt void USARTRxComplete() {

    #endif

        CMCU::OnUSARTRxComplete( UDR );

    } // USART, Rx Complete

#endif


/**
 *  USART Data Register Empty
 */
#ifdef USART_Data_Register_Empty

    #ifdef __GNUC__

        ISR( USART_UDRE_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_UDRE_vect
        __interrupt void USARTDataRegisterEmpty() {

    #endif

        CMCU::OnUSARTDataRegisterEmpty();

    } // USART Data Register Empty

#endif


/**
 *  USART, Tx Complete
 */
#ifdef USART_Tx_Complete

    #ifdef __GNUC__

        ISR( USART_TXC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART0_TX_vect
        __interrupt void USARTTxComplete() {

    #endif

        CMCU::OnUSARTTxComplete();

    } // USART, Tx Complete

#endif


/**
 *  Analog Comparator
 */
#ifdef Analog_Comparator

    #ifdef __GNUC__

        ISR( ANA_COMP_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = ANA_COMP_vect
        __interrupt void AnalogComparator() {

    #endif

        CMCU::OnAnalogComparator();

    } // Analog Comparator

#endif


/**
 *  Pin Change Interrupt
 */
#ifdef Pin_Change_Interrupt

    #ifdef __GNUC__

        ISR( PCINT_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = PCIN1_vect
        __interrupt void PinChangeInterrupt() {

    #endif

        CMCU::OnPinChangeInterrupt();

    } // Pin Change Interrupt

#endif


/**
 *  Timer/Counter1 Compare Match B
 */
#ifdef Timer_Counter1_Compare_Match_B

    #ifdef __GNUC__

        ISR( TIMER1_COMPB_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_COMPB_vect
        __interrupt void TimerCounter1CompareMatchB() {

    #endif

        CMCU::OnTimerCounter1CompareMatchB();

    } // Timer/Counter1 Compare Match B

#endif


/**
 *  Timer/Counter0 Compare Match A
 */
#ifdef Timer_Counter0_Compare_Match_A

    #ifdef __GNUC__

        ISR( TIMER0_COMPA_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_COMPA_vect
        __interrupt void TimerCounter0CompareMatchA() {

    #endif

        CMCU::OnTimerCounter0CompareMatchA();

    } // Timer/Counter0 Compare Match A

#endif


/**
 *  Timer/Counter0 Compare Match B
 */
#ifdef Timer_Counter0_Compare_Match_B

    #ifdef __GNUC__

        ISR( TIMER0_COMPB_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_COMPB_vect
        __interrupt void TimerCounter0CompareMatchB() {

    #endif

        CMCU::OnTimerCounter0CompareMatchB();

    } // Timer/Counter0 Compare Match B

#endif


/**
 *  USI Start Condition
 */
#ifdef USI_Start_Condition

    #ifdef __GNUC__

        ISR( USI_START_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USI_STRT_vect
        __interrupt void USIStartCondition() {

    #endif

        CMCU::OnUSIStartCondition();

    } // USI Start Condition
#endif


/**
 *  USI Overflow
 */
#ifdef USI_Overflow

    #ifdef __GNUC__

        ISR( USI_OVERFLOW_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USI_OVF_vect
        __interrupt void USIOverflow() {

    #endif

        CMCU::OnUSIOverflow();

    } // USI Overflow
#endif


/**
 *  EEPROM Ready
 */
#ifdef EEPROM_Ready

    #ifdef __GNUC__

        ISR( EE_RDY_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = EE_RDY_vect
        __interrupt void EEPROMReady() {

    #endif

        CMCU::OnEEPROMReady();

    } // EEPROM Ready
#endif


/**
 *  Watchdog Timer Overflow
 */
#ifdef Watchdog_Timer_Overflow

    #ifdef __GNUC__

        ISR( WDT_OVERFLOW_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = WDT_vect
        __interrupt void WatchdogTimerOverflow() {

    #endif

        CMCU::OnWatchdogTimerOverflow();

    } // Watchdog Timer Overflow
#endif
