///////////////////////////////////////////////////////////
//  MCU.cpp
//  Implementation of the Class CMCU
//  Created on:      14-июл-2011 11:34:33
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "MCU.h"


// -=[ Пользовательские типы ]=-


// -=[ Внешние ссылки ]=-


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * Главный (основной) поток программы
 */
HRESULT CMCU::MainThreadProcedure(){

    __enable_interrupt();

    do {


    } while ( true );

    // Все проверки прошли успешно, объект в рабочем состоянии
    return NO_ERROR;

}


/**
 * Инициализация всей периферии
 */
void CMCU::Initialization(){

    // Схема соединений (разводка выводов) [ATmega48]
    //PortsInit();
    
    // Настройка таймера/счётчика 0 [ATmega48]
    //Timer0Init();

    // Настройка таймера/счётчика 1 [ATmega48]
    //Timer1Init();
    
    // Настройка таймера/счётчика 2 [ATmega48]
    //Timer2Init();

    // Настройка АЦП [ATmega48]
    //ADCInit();

    // Настройка внутреннего USART [ATmega48]
    //USARTInit();

    // Настройка работы с внешними прерываниями [ATmega48]
    //ExternalInterruptsInit();

    // Настройка управляющих регистров контроллера [ATmega48]
    //ControlRegistersInit();

}


/**
 * Настройка управляющих регистров контроллера
 */
void CMCU::ControlRegistersInit(){

    // MCU Control Register
    // [ Регистр управления микроконтроллером ][ATmega48]
    //           00000000 - Initial Value
    MCUCR = BIN8(00000000);
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, IVCE:   - Interrupt Vector Change Enable
    //           ||||||+-- 1, rw, IVSEL:  - Interrupt Vector Select
    //           |||||+--- 2, r: 0
    //           ||||+---- 3, r: 0
    //           |||+----- 4, rw, PUD:    - Pull-up Disable
    //           ||+------ 5, r: 0
    //           |+------- 6, r: 0
    //           +-------- 7, r: 0
    // Примечание:


    // Special Function IO Register
    // [ Регистр специальных функций ввода/вывода ][ATmega48]
    //           00000000 - Initial Value
    //SFIOR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||	
    //           76543210
    //           |||||||+- 0, rw, PSR321: - Prescaler Reset Timer/Counter3,
    //           |||||||                  - Timer/Counter2, and Timer/Counter1
    //           ||||||+-- 1, rw, PSR0:   - Prescaler Reset Timer/Counter0
    //           |||||+--- 2, rw, PUD:    - Pull-up disable
    //           ||||+---- 3, rw, ACME:   - Analog Comparator Multiplexer Enable
    //           |||+----- 4, r: 0
    //           ||+------ 5, r: 0
    //           |+------- 6, r: 0
    //           +-------- 7, rw, TSM:    - Timer/Counter Synchronization Mode
    // Примечание:

}


/**
 * Настройка таймера/счётчика 0
 */
void CMCU::Timer0Init(){

    // Timer/Counter0 Interrupt Mask Register
    // [ Регистр маски прерываний от таймера/счётчика 0 ][ATmega48]
    //            00000000 - Initial Value
    //TIMSK0 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||	
    //            76543210
    //            |||||||+- 0, rw, TOIE0:   - Timer/Counter0 Overflow Interrupt Enable
    //            ||||||+-- 1, rw, OCIE0A:  - Timer/Counter0 Output Compare Match A Interrupt Enable
    //            |||||+--- 2, rw, OCIE0B:  - Timer/Counter0 Output Compare Match B Interrupt Enable
    //            ||||+---- 3, r: 0
    //            |||+----- 4, r: 0
    //            ||+------ 5, r: 0
    //            |+------- 6, r: 0
    //            +-------- 7, r: 0
    // Примечание:
        
    // [ATmega48] Table. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     ¦
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source (таймер/счетчик остановлен)                     |
    // | 0  | 0  | 1  | clkT0S/(no prescaling)                                          |
    // | 0  | 1  | 0  | clkT0S/8    (from prescaler)                                    |
    // | 0  | 1  | 1  | clkT0S/32   (from prescaler)                                    |
    // | 1  | 0  | 0  | clkT0S/64   (from prescaler)                                    |
    // | 1  | 0  | 1  | clkT0S/128  (from prescaler)                                    |
    // | 1  | 1  | 0  | clkT0S/256  (from prescaler)                                    |
    // | 1  | 1  | 1  | clkT0S/1024 (from prescaler)                                    |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter 0 Control Register A
    // [ Регистр управления A Таймером/Счётчиком 0 ]
    //            00000000 - Initial Value
    //TCCR0A = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, WGM00:  -+ - Waveform generation mode
    //            ||||||+-- 1, rw, WGM01:  _| 
    //            |||||+--- 2, r: 0 
    //            ||||+---- 3, r: 0
    //            |||+----- 4, rw, COM0B0: -+ - Compare match output B mode
    //            ||+------ 5, rw: COM0B1: _| 
    //            |+------- 6, rw, COM0A0: -+ - Compare match output A mode
    //            +-------- 7, rw, COM0A1: _|  
    // Примечание:


    // Timer/Counter 0 Control Register B
    // [ Регистр управления B Таймером/Счётчиком 0 ]
    //            00000000 - Initial Value
    //TCCR0B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, CS00:  -+ - Waveform generation mode
    //            ||||||+-- 1, rw, CS01:   | 
    //            |||||+--- 2, rw, CS02:  _| 
    //            ||||+---- 3, rw, WGM02:    - Waveform generation mode
    //            |||+----- 4, r: 0
    //            ||+------ 5, r: 0
    //            |+------- 6, rw, FOC0B:    - Force output compare B
    //            +-------- 7, rw, FOC0A:    - Force output compare A
    // Примечание:

    // Устанавливаем значения для счётного регистра
    //TCNT0 = 0xFF - F_CPU / 1024000UL;

    // Timer/Counter0 Output Compare Register A
    //OCR0A = 0x00;

    // Timer/Counter0 Output Compare Register B
    //OCR0B = 0x00;

}


/**
 * Настройка таймера/счётчика 1
 */
void CMCU::Timer1Init(){

    // Timer/Counter1 Interrupt Mask Register
    // [ Регистр маски прерываний от таймера/счётчика 1 ][ATmega48]
    //            00000000 - Initial Value
    //TIMSK1 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, TOIE1:   - Timer/Counter1 Overflow Interrupt Enable
    //            ||||||+-- 1, rw, OCIE1A:  - Timer/Counter1 Output Compare Match A Interrupt Enable
    //            |||||+--- 2, rw, OCIE1B:  - Timer/Counter1 Output Compare Match B Interrupt Enable
    //            ||||+---- 3, r: 0         - зарезервирован, должен быть установлен в 0 при записи
    //            |||+----- 4, r: 0         - зарезервирован, должен быть установлен в 0 при записи
    //            ||+------ 5, rw, ICIE1:   - Timer/Counter1, Input Capture Interrupt Enable
    //            |+------- 6, r: 0         - зарезервирован, должен быть установлен в 0 при записи
    //            +-------- 7, r: 0         - зарезервирован, должен быть установлен в 0 при записи
    // Примечание:
    
    // [ATmega48] Table. Clock Select Bit Description
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
    // [ Регистр управления B Таймером/Счётчиком 1 ][ATmega48]
    //            00000000 - Initial Value
    //TCCR1B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
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
    //TCNT1H = 0xFC; //( 0xFFFF - Delay * F_CPU / 8UL ) >> 8
    //TCNT1L = 0x17; //( 0xFFFF - 1000UL * F_CPU / 8UL )
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
    // [ Регистр управления A Таймером/Счётчиком 1 ][ATmega48]
    //            00000000 - Initial Value
    //TCCR1A = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||	
    //            76543210
    //            |||||||+- 0, rw, WGM10:  -+ - Режим работы таймера/счетчика
    //            ||||||+-- 1, rw, WGM11:  _|
    //            |||||+--- 2, rw, COM1C0: -+ - Режим работы канала сравнения C
    //            ||||+---- 3, rw, COM1C1: _|
    //            |||+----- 4, rw, COM1B0: -+ - Режим работы канала сравнения B
    //            ||+------ 5, rw, COM1B1: _|
    //            |+------- 6, rw, COM1A0: -+ - Режим работы канала сравнения A
    //            +-------- 7, rw, COM1A1: _|
    // Примечание: 


    //TCCR1B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
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

}


/**
 * Настройка таймера/счётчика 2
 */
void CMCU::Timer2Init(){
   
    // Timer/Counter2 Control Register A
    // [ Регистр управления A Таймером/Счётчиком 2 ][ATmega48]
    //            00000000 - Initial Value
    //TCCR2A = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||	
    //            76543210
    //            |||||||+- 0, rw, WGM20:  -+ - Waveform Generation Mode
    //            ||||||+-- 1, rw, WGM21:  _|
    //            |||||+--- 2, r: 0
    //            ||||+---- 3, r: 0
    //            |||+----- 4, rw, COM2B0: -+ - Режим работы канала сравнения B
    //            ||+------ 5, rw, COM2B1: _|
    //            |+------- 6, rw, COM2A0: -+ - Режим работы канала сравнения A
    //            +-------- 7, rw, COM2A1: _|
    // Примечание: Установлен режим работы ...
    

    // Timer/Counter2 Control Register B
    // [ Регистр управления B Таймером/Счётчиком 2 ][ATmega48]
    //            00000000 - Initial Value
    //TCCR2B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, CS20:  -+ - Управление тактовым сигналом
    //            ||||||+-- 1, rw, CS21:   | 
    //            |||||+--- 2, rw, CS22:  _|
    //            ||||+---- 3, rw, WGM22:    - Waveform Generation Mode
    //            |||+----- 4, r: 0          - зарезервирован, должен быть установлен в 0 при записи  
    //            ||+------ 5, r: 0          - зарезервирован, должен быть установлен в 0 при записи
    //            |+------- 6, rw, FOC2B:    - Force Output Compare B
    //            +-------- 7, rw, FOC2A:    - Force Output Compare A
    // Примечание: 
    

    // Устанавливаем значения для счётного регистра
    // TODO: написать формулу
    //TCNT2 = 0x00;

    // Timer/Counter2 Output Compare Register A
    //OCR2A = 0x00;
    
    // Timer/Counter2 Output Compare Register B
    //OCR2B = 0x00;

    // Timer/Counter2 Asynchronous Status Register
    // [ Регистр ... ][ATmega48]
    //          00000000 - Initial Value
    //ASSR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||	
    //          76543210
    //          |||||||+- 0, r, TCR2BUB: - Timer/Counter Control Register2 Update Busy
    //          ||||||+-- 1, r, TCR2AUB: - Timer/Counter Control Register2 Update Busy
    //          |||||+--- 2, r, OCR2BUB: - Output Compare Register2 Update Busy
    //          ||||+---- 3, r, OCR2AUB: - Output Compare Register2 Update Busy
    //          |||+----- 4, r, TCN2UB:  - Timer/Counter2 Update Busy
    //          ||+------ 5, rw, AS2:    - Asynchronous Timer/Counter2
    //          |+------- 6, rw, EXCLK:  - Enable External Clock Input
    //          +-------- 7, r: 0        - зарезервирован, должен быть установлен в 0 при записи
    // Примечание: 
    

    // Timer/Counter2 Interrupt Mask Register
    // [ Регистр маски прерываний от таймера/счётчика 2 ][ATmega48]
    //            00000000 - Initial Value
    //TIMSK2 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, TOIE2:     - Timer/Counter2 Overflow Interrupt Enable
    //            ||||||+-- 1, rw, OCIE2A:    - Timer/Counter2 Output Compare Match A Interrupt Enable
    //            |||||+--- 2, rw, OCIE2B:    - Timer/Counter2 Output Compare Match B Interrupt Enable
    //            ||||+---- 3, r: 0        -+ - зарезервированы, должены быть установлены в 0 при записи     
    //            |||+----- 4, r: 0         |
    //            ||+------ 5, r: 0         |
    //            |+------- 6, r: 0         |
    //            +-------- 7, r: 0        _|
    // Примечание:
    

    // Timer/Counter2 Interrupt Flag Register
    // [ Регистр флагов прерываний таймера/счётчика 2 ]
    //           00000000 - Initial Value
    //TIFR2 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||		
    //           76543210
    //           |||||||+- 0, rw, TOV2:  - Timer/Counter2 Overflow Flag
    //           ||||||+-- 1, rw, OCF2A: - Output Compare Flag 2 A
    //           |||||+--- 2, rw, OCF2B: - Output Compare Flag 2 B
    //           ||||+---- 3, r: 0       -+ - зарезервированы, должены быть установлены в 0 при записи
    //           |||+----- 4, r: 0        |
    //           ||+------ 5, r: 0        |
    //           |+------- 6, r: 0        |
    //           +-------- 7, r: 0       _|
    // Примечание:
    
}


/**
 * Настройка внутреннего USART0
 */
void CMCU::USARTInit(){

    // USART0 Control and Status Register A
    // [ Регистр управления USART0A ]
    //            00100000 - Initial Value
    //UCSR0A = BIN8(00100000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||	
    //            76543210
    //            |||||||+- 0, rw, MPCM0: - Multi-Processor Communication Mode
    //            ||||||+-- 1, rw, U2X0:  - Double the USART Transmission Speed
    //            |||||+--- 2, r,  UPE0:  - Parity Error
    //            ||||+---- 3, r,  DOR0:  - Data OverRun
    //            |||+----- 4, r,  FE0:   - Frame Error
    //            ||+------ 5, r,  UDRE0: - USART Data Register Empty
    //            |+------- 6, rw, TXC0:  - USART Transmit Complete
    //            +-------- 7, r,  RXC0:  - USART Receive Complete
    // Примечание:


    //UCSR0B = 0x00; // отключаем, пока настраиваем скорость

    // Определение BAUD см. в файле: "Configuration.h"
    //UBRR0L = F_CPU / ( 16UL * BAUD ) - 1UL; // устанавливаем скорость
    //UBRR0H = ( uint8_t ) ( ( F_CPU / ( 16UL * BAUD ) - 1UL ) >> 8 );

    // USART0 Control and Status Register B
    //            00000000 - Initial Value
    //UCSR0B = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, TXB8n:  - Transmit Data Bit 8 n
    //            ||||||+-- 1, r,  RXB8n:  - Receive Data Bit 8 n
    //            |||||+--- 2, rw, UCSZn2: - Character Size n
    //            ||||+---- 3, rw, TXENn:  - Transmitter Enable n
    //            |||+----- 4, rw, RXENn:  - Receiver Enable n
    //            ||+------ 5, rw, UDRIEn: - USART Data Register Empty Interrupt Enable n
    //            |+------- 6, rw, TXCIEn: - TX Complete Interrupt Enable n
    //            +-------- 7, rw, RXCIEn: - RX Complete Interrupt Enable n
    // Примечание:

    // USART0 Control and Status Register C
    //            00000110 - Initial Value
    //UCSR0C = BIN8(00000110); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, UCPOLn:     - Clock Polarity
    //            ||||||+-- 1, rw, UCSZn0:  -+ - Character Size
    //            |||||+--- 2, rw, UCSZn1:  _|
    //            ||||+---- 3, rw, USBSn:      - Stop Bit Select
    //            |||+----- 4, rw, UPMn0:   -+ - Parity Mode
    //            ||+------ 5, rw, UPMn1:   _|
    //            |+------- 6, rw, UMSELn0: -+ - USART Mode Select
    //            +-------- 7, rw: UMSELn1: _|
    // Примечание: 

}


/**
 * -=[ Схема соединений (разводка выводов) MCU ATmega48 ]=-
 */
void CMCU::PortsInit(){

    // Параметры проекта:
    // Имя проекта: Шаблон для ATmega48
    // Файл схемы (URI): file:///
    // Хеш файла схемы (MD5):
    // Код:
    // Дата: 21.11.2012
    // Координаты разработчика:
    //  mailto:unihomelab@ya.ru
    //  skype: viacheslavmezentsev

//                                 [ATmega48]                       
//
//                                                              
//                                                              
//                                     PDIP                     
//                               +--------------+               
//         (PCINT14/~RESET) PC6 -|  1        28 |- PC5 (ADC5/SCL/PCINT13)
//            (PCINT16/RXD) PD0 -|  2        27 |- PC4 (ADC4/SDA/PCINT12)   
//            (PCINT17/TXD) PD1 -|  3        26 |- PC3 (ADC3/PCINT11)   
//           (PCINT18/INT0) PD2 -|  4        25 |- PC2 (ADC2/PCINT10)   
//      (PCINT19/OC2B/INT1) PD3 -|  5        24 |- PC1 (ADC1/PCINT9)   
//         (PCINT20/XCK/T0) PD4 -|  6        23 |- PC0 (ADC0/PCINT8)  
//                          VCC -|  7        22 |- GND   
//                          GND -|  8        21 |- AREF   
//     (PCINT6/XTAL1/TOSC1) PB6 -|  9        20 |- AVCC         
//     (PCINT7/XTAL2/TOSC2) PB7 -| 10        19 |- PB5 (SCK/PCINT5)          
//        (PCINT21/OC0B/T1) PD5 -| 11        18 |- PB4 (MISO/PCINT4)         
//      (PCINT22/OC0A/AIN0) PD6 -| 12        17 |- PB3 (MOSI/OC2A/PCINT3)  
//           (PCINT23/AIN1) PD7 -| 13        16 |- PB2 (SS/OC1B/PCINT2)  
//       (PCINT0/CLKO/ICP1) PB0 -| 14        15 |- PB1 (OC1A/PCINT1)       
//                               +--------------+               
//                                                              

    // Table. Port Pin Configurations
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //| DDxn|PORTxn|   PUD    |  I/O   | Pull-up |   Comment                                  |
    //|     |      |(in MCUCR)|        |         |                                            |
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

    // Настройка портов B-D. Начальная инициализация уровней

    // Port B Data Direction Register
    // [ Регистр направления порта B ][ATmega48]
    //          00000000 - Initial Value
    DDRB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDB0: - 
    //          ||||||+-- 1, rw, DDB1: - 
    //          |||||+--- 2, rw, DDB2: -
    //          ||||+---- 3, rw, DDB3: -
    //          |||+----- 4, rw, DDB4: -
    //          ||+------ 5, rw, DDB5: -
    //          |+------- 6, rw, DDB6: -
    //          +-------- 7, rw, DDB7: -
    // Примечание:


    // Port C Data Direction Register
    // [ Регистр направления порта C ][ATmega48]
    //          00000000 - Initial Value
    DDRC = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDC0: - 
    //          ||||||+-- 1, rw, DDC1: - 
    //          |||||+--- 2, rw, DDC2: - 
    //          ||||+---- 3, rw, DDC3: - 
    //          |||+----- 4, rw, DDC4: - 
    //          ||+------ 5, rw, DDC5: - 
    //          |+------- 6, rw, DDC6: -
    //          +-------- 7, rw, DDC7: -
    // Примечание:


    // Port D Data Direction Register
    // [ Регистр направления порта D ][ATmega48]
    //          00000000 - Initial Value
    DDRD = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDD0: - 
    //          ||||||+-- 1, rw, DDD1: - 
    //          |||||+--- 2, rw, DDD2: - 
    //          ||||+---- 3, rw, DDD3: - 
    //          |||+----- 4, rw, DDD4: -
    //          ||+------ 5, rw, DDD5: -
    //          |+------- 6, rw, DDD6: -
    //          +-------- 7, rw, DDD7: -
    // Примечание: 


    // Port B Data Register
    // [ Регистр данных порта B ][ATmega48]
    //           00000000 - Initial Value
    PORTB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTB0: (PCINT0/CLKO/ICP1)   - 
    //           ||||||+-- 1, rw, PORTB1: (OC1A/PCINT1)        -
    //           |||||+--- 2, rw, PORTB2: (SS/OC1B/PCINT2)     -
    //           ||||+---- 3, rw, PORTB3: (MOSI/OC2A/PCINT3)   -
    //           |||+----- 4, rw, PORTB4: (MISO/PCINT4)        -
    //           ||+------ 5, rw, PORTB5: (SCK/PCINT5)         -
    //           |+------- 6, rw, PORTB6: (PCINT6/XTAL1/TOSC1) -
    //           +-------- 7, rw, PORTB7: (PCINT7/XTAL2/TOSC2) -
    // Примечание:


    // Port C Data Register
    // [ Регистр данных порта C ][ATmega48]
    //           00000000 - Initial Value
    PORTC = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTC0: (ADC0/PCINT8)      - 
    //           ||||||+-- 1, rw, PORTC1: (ADC1/PCINT9)      - 
    //           |||||+--- 2, rw, PORTC2: (ADC2/PCINT10)     - 
    //           ||||+---- 3, rw, PORTC3: (ADC3/PCINT11)     - 
    //           |||+----- 4, rw, PORTC4: (ADC4/SDA/PCINT12) - 
    //           ||+------ 5, rw, PORTC5: (ADC5/SCL/PCINT13) - 
    //           |+------- 6, rw, PORTC6: (PCINT14/RESET)    - 
    //           +-------- 7, r: 0
    // Примечание:


    // Port D Data Register
    // [ Регистр данных порта D ][ATmega48]
    //           00000000 - Initial Value
    PORTD = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTD0: (PCINT16/RXD)       - 
    //           ||||||+-- 1, rw, PORTD1: (PCINT17/TXD)       - 
    //           |||||+--- 2, rw, PORTD2: (PCINT18/INT0)      - 
    //           ||||+---- 3, rw, PORTD3: (PCINT19/OC2B/INT1) - 
    //           |||+----- 4, rw, PORTD4: (PCINT20/XCK/T0)    -
    //           ||+------ 5, rw, PORTD5: (PCINT21/OC0B/T1)   -
    //           |+------- 6, rw, PORTD6: (PCINT22/OC0A/AIN0) - 
    //           +-------- 7, rw, PORTD7: (PCINT23/AIN1)      -
    // Примечание:

}


/**
 * Настройка работы с внешними прерываниями
 */
void CMCU::ExternalInterruptsInit(){

    // Table. Interrupt Sense Control
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|ISCn1|ISCn0 | Description                                                              |
    //+-----+------+----------+--------+---------+--------------------------------------------+
    //|  0  |  0   | The low level of INTn generates an interrupt request.                    |
    //|  0  |  1   | Any logical change on INTn generates an interrupt request.               |
    //|  1  |  0   | The falling edge of INTn generates asynchronously an interrupt request.  |
    //|  1  |  1   | The rising edge of INTn generates asynchronously an interrupt request.   |
    //+-----+------+----------+--------+---------+--------------------------------------------+

    // External Interrupt Control Register A
    // [ Регистр управления внешними прерываниями ]
    //           00000000 - Initial Value
    //EICRA = BIN8(00000000);
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, ISC00: -+ - External Interrupt 0 Sense Control Bits
    //           ||||||+-- 1, rw, ISC01: _|
    //           |||||+--- 2, rw, ISC10: -+ - External Interrupt 1 Sense Control Bits
    //           ||||+---- 3, rw, ISC11: _|
    //           |||+----- 4, r: 0
    //           ||+------ 5, r: 0
    //           |+------- 6, r: 0
    //           +-------- 7, r: 0
    // When changing the ISCn1/ISCn0 bits, the interrupt must be disabled by clearing its
    // Interrupt Enable bit in the EIMSK Register. Otherwise an interrupt can occur when
    // the bits are changed.
    // Примечание: Начальная настройка обоих прерываний на спад сигнала.


    // External Interrupt Mask Register
    // [ Общий регистр управлением прерываниями ]
    //           00000000 - Initial Value
    //EIMSK = BIN8(00000000);
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, INT0: - External Interrupt Request 0 Enable
    //           ||||||+-- 1, rw, INT1: - External Interrupt Request 1 Enable
    //           |||||+--- 2, r: 0
    //           ||||+---- 3, r: 0
    //           |||+----- 4, r: 0
    //           ||+------ 5, r: 0
    //           |+------- 6, r: 0
    //           +-------- 7, r: 0
    // Примечание:

}


/**
 * Настройка АЦП
 */
void CMCU::ADCInit(){

    // ADC Multiplexer Selection Register – ADMUX
    // [ ADC Multiplexer Selection Register ]
    //           00000000 - Initial Value
    //ADMUX = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||	
    //           76543210
    //           |||||||+- 0, rw, MUX0:  -+ - Analog Channel Selection Bits
    //           ||||||+-- 1, rw, MUX1:   |
    //           |||||+--- 2, rw, MUX2:   |
    //           ||||+---- 3, rw, MUX3:  _|
    //           |||+----- 4, r: 0
    //           ||+------ 5, rw, ADLAR:    - ADC Left Adjust Result
    //           |+------- 6, rw, REFS0: -+ - Reference Selection Bits
    //           +-------- 7, rw, REFS1: _|
    // Примечание: 


    // Digital Input Disable Register 0 – DIDR0
    // [ Digital Input Disable Register 0 ]
    //           00000000 - Initial Value
    //DIDR0 = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //           ||||||||	
    //           76543210
    //           |||||||+- 0, rw, ADC0D:  -+ - ADC5..0 Digital Input Disable
    //           ||||||+-- 1, rw, ADC1D:   |
    //           |||||+--- 2, rw, ADC2D:   |
    //           ||||+---- 3, rw, ADC3D:   |
    //           |||+----- 4, rw, ADC4D:   | 
    //           ||+------ 5, rw, ADC5D:  _| 
    //           |+------- 6, r: 0
    //           +-------- 7, r: 0
    // Примечание: 
    
        
    // ADC Control and Status Register B – ADCSRB
    // [ ADC Control and Status Register B ]
    //            00000000 - Initial Value
    //ADCSRB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||	
    //            76543210
    //            |||||||+- 0, rw, ADTS0: -+ - ADC Auto Trigger Source
    //            ||||||+-- 1, rw, ADTS1:  |
    //            |||||+--- 2, rw, ADTS2: _|
    //            ||||+---- 3, r: 0
    //            |||+----- 4, r: 0
    //            ||+------ 5, r: 0
    //            |+------- 6, rw, ACME:     - (?) 
    //            +-------- 7, r: 0
    // Примечание: 
    
    
    // ADC Control and Status Register A – ADCSRA
    // [ ADC Control and Status Register A ]
    //            00000000 - Initial Value
    //ADCSRA = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||		
    //            76543210
    //            |||||||+- 0, rw, ADPS0: -+ - ADC Prescaler Select Bits
    //            ||||||+-- 1, rw, ADPS1:  |
    //            |||||+--- 2, rw, ADPS2: _|
    //            ||||+---- 3, rw, ADIE:     - ADC Interrupt Enable
    //            |||+----- 4, rw, ADIF:     - ADC Interrupt Flag
    //            ||+------ 5, rw, ADATE:    - ADC Auto Trigger Enable
    //            |+------- 6, rw, ADSC:     - ADC Start Conversion
    //            +-------- 7, rw, ADEN:     - ADC Enable
    // Примечание:
    
}


/**
 * Настройка последовательного интерфейса SPI
 */
void CMCU::SPIInit(){

    // TODO: Переделать для ATmega48.

    // SPI Control Register
    // [ Регистр управления SPI ][ATmega16]
    //          00000000 - Initial Value
    //SPCR = BIN8(0000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, SPR0: -+ - Скорость передачи
    //          ||||||+-- 1, rw, SPR1: _|
    //          |||||+--- 2, rw, CPHA:    - Фаза тактового сигнала
    //          ||||+---- 3, rw, CPOL:    - Полярность тактового сигнала
    //          |||+----- 4, rw, MSTR:    - Выбор режима работы (Master/Slave)
    //          ||+------ 5, rw, DORD:    - Порядок передачи данных
    //          |+------- 6, rw, SPE:     - Включение/выключение SPI
    //          +-------- 7, rw, SPIE:    - Разрешение прерывания от SPI
    // Примечание:


    // SPI Status Register
    // [ Регистр статуса SPI ][ATmega16]
    //          00000000 - Initial Value
    //SPSR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, SPI2X:    - Double SPI Speed Bit
    //          ||||||+-- 1, r, 0       -+ - зарезервированы
    //          |||||+--- 2, r, 0        |
    //          ||||+---- 3, r, 0        | 
    //          |||+----- 4, r, 0        |
    //          ||+------ 5, r, 0       _|
    //          |+------- 6, r, WCOL:      - Write COLlision flag
    //          +-------- 7, r, SPIF:      - SPI Interrupt Flag
    // Примечание:

}


/**
 * Настройка последовательного интерфейса TWI
 */
void CMCU::TWIInit(){

    // TODO: Переделать для ATmega48.

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
    //          ||||+---- 3, r, TWS3:   -+
    //          |||+----- 4, r, TWS4:    |
    //          ||+------ 5, r, TWS5:    | - TWI Status
    //          |+------- 6, r, TWS6:    |
    //          +-------- 7, r, TWS7:   _|
    // Примечание:

    // TWI (Slave) Address Register
    // Иметь в виду, что регистр содержит бит TWGCE (TWI General Call Recognition Enable Bit)
    //TWAR = 0xFE;

}



/**
 * Настройка внутреннего сторожевого таймера
 */
void CMCU::InternalWDTInit(){

    // TODO: Переделать для ATmega48.

    // [ATmega48] Table. Watchdog Timer Prescale Select
    // +----+----+----+----+------------------+---------+
    // |WDP3|WDP2|WDP1|WDP0|OscCycles         | VCC5.0V |
    // +----+----+----+----+------------------+---------+
    // | 0  | 0  | 0  | 0  |    2K    (2048)  | 16 ms   |
    // | 0  | 0  | 0  | 1  |    4K    (4096)  | 32 ms   |
    // | 0  | 0  | 1  | 0  |    8K    (8192)  | 64 ms   |
    // | 0  | 0  | 1  | 1  |   16K   (16384)  | 0.125 s |    
    // | 0  | 1  | 0  | 0  |   32K   (32768)  | 0.25 s  |
    // | 0  | 1  | 0  | 1  |   64K   (65536)  | 0.5 s   |
    // | 0  | 1  | 1  | 0  |  128K  (131072)  | 1.0 s   |
    // | 0  | 1  | 1  | 1  |  256K  (262144)  | 2.0 s   |
    // | 1  | 0  | 0  | 0  |  512K  (524288)  | 4.0 s   |
    // | 1  | 0  | 0  | 1  | 1024K (1048576)  | 8.0 s   |
    // | 1  | 0  | 1  | 0  |------------------+---------+
    // | 1  | 0  | 1  | 1  |                            |    
    // | 1  | 1  | 0  | 0  |                            |
    // | 1  | 1  | 0  | 1  |     Зарезервировано        |
    // | 1  | 1  | 1  | 0  |                            |
    // | 1  | 1  | 1  | 1  |                            |    
    // +----+----+----+----+----------------------------+

    // Watchdog Timer Control Register
    // [ Регистр управления сторожевого таймера ][ATmega48]
    //            00000000 - Initial Value
    //WDTCSR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, WDP0: -+ - Коэффициент деления предделителя сторожевого таймера [0..2]
    //            ||||||+-- 1, rw, WDP1:  |
    //            |||||+--- 2, rw, WDP2: _|
    //            ||||+---- 3, rw, WDE:     - Разрешение сброса по тайм-ауту сторожевого таймера
    //            |||+----- 4, rw, WDCE:    - Разрешение изменения конфигурации сторожевого таймера
    //            ||+------ 5, rw, WDP3:    - Коэффициент деления предделителя сторожевого таймера [3]
    //            |+------- 6, rw, WDIE:    - Watchdog interrupt enable
    //            +-------- 7, rw, WDIF:    - Watchdog interrupt flag
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
 * Pin Change Interrupt Request 0
 */
void CMCU::OnPinChangeInterruptRequest0(){

}


/**
 * Pin Change Interrupt Request 1
 */
void CMCU::OnPinChangeInterruptRequest1(){

}


/**
 * Pin Change Interrupt Request 2
 */
void CMCU::OnPinChangeInterruptRequest2(){

}


/**
 * Pin Change Interrupt Request 2
 */
void CMCU::OnWatchdogTimeoutInterrupt(){

}


/**
 * Timer/Counter2 Compare Match A
 */
void CMCU::OnTimerCounter2CompareMatchA(){

}


/**
 * Timer/Counter2 Compare Match B
 */
void CMCU::OnTimerCounter2CompareMatchB(){

}


/**
 * Timer/Counter2 Overflow
 */
void CMCU::OnTimerCounter2Overflow(){
    
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

}


/**
 * Timer/Counter1 Compare Match B
 */
void CMCU::OnTimerCounter1CompareMatchB(){

}


/**
 * Timer/Counter1 Overflow
 */
void CMCU::OnTimerCounter1Overflow(){
    
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
 * Timer/Counter0 Overflow
 */
void CMCU::OnTimerCounter0Overflow(){

}


/**
 * SPI Serial Transfer Complete
 */
void CMCU::OnSPISerialTransferComplete(){

}


/**
 * USART, Rx Complete
 */
void CMCU::OnUSARTRxComplete( uint8_t data ){
   
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
 * ADC Conversion Complete
 */
void CMCU::OnADCConversionComplete( uint16_t value ) {
        
}


/**
 * EEPROM Ready
 */
void CMCU::OnEEPROMReady(){

}


/**
 * Analog Comparator
 */
void CMCU::OnAnalogComparator(){

}


/**
 * 2-wire Serial Interface
 */
void CMCU::OnTWISerialInterface(){

}


/**
 * Store Program Memory Read
 */
void CMCU::OnStoreProgramMemoryRead(){

}


// -=[ Процедуры обработки прерываний ] [ATmega48]=-

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
 * Pin Change Interrupt Request 0
 */
#ifdef Pin_Change_Interrupt_Request0

    #ifdef __GNUC__

        ISR( PCINT0_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = PCINT0_vect
        __interrupt void PinChangeInterruptRequest0() {

    #endif

        CMCU::OnPinChangeInterruptRequest0();

    } // Pin Change Interrupt Request 0

#endif


/**
 * Pin Change Interrupt Request 1
 */
#ifdef Pin_Change_Interrupt_Request_1

    #ifdef __GNUC__

        ISR( PCINT1_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = PCINT1_vect
        __interrupt void PinChangeInterruptRequest1() {

    #endif

        CMCU::OnPinChangeInterruptRequest1();

    } // Pin Change Interrupt Request 1

#endif


/**
 * Pin Change Interrupt Request 2
 */
#ifdef Pin_Change_Interrupt_Request_2

    #ifdef __GNUC__

        ISR( PCINT2_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = PCINT2_vect
        __interrupt void PinChangeInterruptRequest2() {

    #endif

        CMCU::OnPinChangeInterruptRequest2();

    } // Pin Change Interrupt Request 2

#endif


/**
 *  Watchdog Time-out Interrupt
 */
#ifdef Watchdog_Timeout_Interrupt

    #ifdef __GNUC__

        ISR( WDT_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = WDT_vect
        __interrupt void WatchdogTimeoutInterrupt() {

    #endif

        CMCU::OnWatchdogTimeoutInterrupt();

    } // Watchdog Time-out Interrupt

#endif


/**
 *  Timer/Counter2 Compare Match A
 */
#ifdef Timer_Counter2_Compare_Match_A

    #ifdef __GNUC__

        ISR( TIMER2_COMPA_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER2_COMPA_vect
        __interrupt void TimerCounter2CompareMatchA() {

    #endif

        CMCU::OnTimerCounter2CompareMatchA();

    } // Timer/Counter2 Compare Match A

#endif


/**
 *  Timer/Counter2 Compare Match B
 */
#ifdef Timer_Counter2_Compare_Match_B

    #ifdef __GNUC__

        ISR( TIMER2_COMPB_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER2_COMPB_vect
        __interrupt void TimerCounter2CompareMatchB() {

    #endif

        CMCU::OnTimerCounter2CompareMatchB();

    } // Timer/Counter2 Compare Match B

#endif


/**
 *  Timer/Counter2 Overflow
 */
#ifdef Timer_Counter2_Overflow

    #ifdef __GNUC__

        ISR( TIMER2_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER2_OVF_vect
        __interrupt void TimerCounter2Overflow() {

    #endif

        CMCU::OnTimerCounter2Overflow();

    } // Timer/Counter2 Overflow

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
 *  Timer/Counter1 Overflow
 */
#ifdef Timer_Counter1_Overflow

    #ifdef __GNUC__

        ISR( TIMER1_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER1_OVF_vect
        __interrupt void TimerCounter1Overflow() {

    #endif
               
        CMCU::OnTimerCounter1Overflow();
        
    } // Timer/Counter1 Overflow

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
 *  Timer/Counter0 Overflow
 */
#ifdef Timer_Counter0_Overflow

    #ifdef __GNUC__

        ISR( TIMER0_OVF_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TIMER0_OVF_vect
        __interrupt void TimerCounter0Overflow() {

    #endif

        CMCU::OnTimerCounter0Overflow();

    } // Timer/Counter0 Overflow

#endif


/**
 *  SPI Serial Transfer Complete
 */
#ifdef SPI_Serial_Transfer_Complete

    #ifdef __GNUC__

        ISR( SPI_STC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = SPI_STC_vect
        __interrupt void SPISerialTransferComplete() {

    #endif

        CMCU::OnSPISerialTransferComplete();

    } // SPI Serial Transfer Complete

#endif


/**
 *  USART, Rx Complete
 */
#ifdef USART_Rx_Complete

    #ifdef __GNUC__

        ISR( USART_RX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART_RX_vect
        __interrupt void USARTRxComplete() {

    #endif
        
        CMCU::OnUSARTRxComplete( UDR0 );

    } // USART, Rx Complete

#endif


/**
 *  USART Data Register Empty
 */
#ifdef USART_Data_Register_Empty

    #ifdef __GNUC__

        ISR( USART_UDRE_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART_UDRE_vect
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

        ISR( USART_TX_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = USART_TX_vect
        __interrupt void USARTTxComplete() {

    #endif
        
        CMCU::OnUSARTTxComplete();

    } // USART, Tx Complete

#endif


/**
 *  ADC Conversion Complete
 */
#ifdef ADC_Conversion_Complete

    #ifdef __GNUC__

        ISR( ADC_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = ADC_vect
        __interrupt void ADCConversionComplete() {

    #endif           

        uint16_t temp = ADCL;
        temp += ( ADCH << 8 );

        CMCU::OnADCConversionComplete( temp );
        
    } // ADC Conversion Complete

#endif


/**
 *  EEPROM Ready
 */
#ifdef EEPROM_Ready

    #ifdef __GNUC__

        ISR( EE_READY_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = EE_RDY_vect
        __interrupt void EEPROMReady() {

    #endif

        CMCU::OnEEPROMReady();


    } // EEPROM Ready
#endif


/**
 *  Analog Comparator
 */
#ifdef Analog_Comparator

    #ifdef __GNUC__

        ISR( ANALOG_COMP_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = ANA_COMP_vect
        __interrupt void AnalogComparator() {

    #endif

        CMCU::OnAnalogComparator();

    } // Analog Comparator

#endif


/**
 *  Two-wire Serial Interface
 */
#ifdef TWI_Serial_Interface

    #ifdef __GNUC__

        ISR( TWI_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = TWI_vect
        __interrupt void TWISerialInterface() {

    #endif

        CMCU::OnTWISerialInterface();

    } // Two-wire Serial Interface

#endif


/**
 *  Store Program Memory Read
 */
#ifdef Store_Program_Memory_Read

    #ifdef __GNUC__

        ISR( SPM_READY_vect ) {

    #elif defined( __ICCAVR__ )

        #pragma vector = SPM_READY_vect
        __interrupt void StoreProgramMemoryRead() {

    #endif

        CMCU::OnStoreProgramMemoryRead();

    } // Store Program Memory Read

#endif
