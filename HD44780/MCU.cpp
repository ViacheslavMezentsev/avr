///////////////////////////////////////////////////////////
//  MCU.cpp
//  Implementation of the Class CMCU
//  Created on:      14-���-2011 11:34:33
//  Original author: �����
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "LCD.h"
#include "MCU.h"


// -=[ ������� ������ ]=-


// -=[ ���������� �� ����-������ ]=-

// �������� ��������
FLASHSTR_DECLARE( char, frmSplashString,
#ifdef __GNUC__
"                 HD44780 ���� GCC\n"
#elif defined __ICCAVR__
"                 HD44780 ���� IAR\n"
#endif
"                 �����: uni     " );

// ����������� ���������� �� ������
FLASHSTR_DECLARE( char, frmString,
"���:            \n"
"                " );


// -=[ ���������� � ��� ]=-


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * ������� (��������) ����� ���������.
 */
HRESULT CMCU::MainThreadProcedure(){

    // ��������� ���������������� ����� ��������.
    CLCD::LoadCustomChars();

    // ����� ��������.
	CLCD::WriteString( frmSplashString, 0, 0 );
	
    // �������� ��������.
	uint8_t cnt = 16;
	
	do {
	
	    CLCD::ShiftLeft();
	    _delay_ms( 250 );
	
	} while ( cnt-- );
	
	_delay_ms( 3000UL );

    CLCD::Clear();
    CLCD::Home();

    // ����� ������������ ������. 
    CLCD::WriteString( frmString );

    // ����� ������ � ���� ������.
    CLCD::WriteString( CVersion::GetVersionString(), 0, 5 );
    CLCD::WriteString( CVersion::GetBuildDateString(), 1, 0 );

    CLCD::PutChar( 0, 1, 0 );
    CLCD::PutChar( 1 );
    CLCD::PutChar( 2 );
    CLCD::PutChar( 3 );
    CLCD::PutChar( 4 );
    CLCD::PutChar( 5 );
    CLCD::PutChar( 6 );
    CLCD::PutChar( 7 );

    // ��������� ����������.
    __enable_interrupt();

    do {


    } while ( true );

    // ��� �������� ������ �������, ������ � ������� ���������.
    return NO_ERROR;

}


/**
 * ������������� ���� ���������
 */
void CMCU::Initialization(){

    // ����� ���������� (�������� �������) [ATmega16]
    PortsInit();

    // ��������� ��� [ATmega16]
    //ADCInit();

    // ��������� �������/�������� 0 [ATmega16]
    //Timer0Init();

    // ��������� �������/�������� 1 [ATmega16]
    //Timer1Init();

    // ��������� �������/�������� 2 [ATmega16]
    //Timer2Init();

    // ��������� ����������� ����������� ������� [ATmega16]
    //InternalWDTInit();

    // ��������� ����������� USART [ATmega16]
    //USARTInit();

    // ��������� ����������������� ���������� TWI [ATmega16]
    //TWIInit();

    // ��������� ����������������� ���������� SPI [ATmega16]
    //SPIInit();

    // ��������� ������ � �������� ������������ [ATmega16]
    //ExternalInterruptsInit();

    // ��������� ����������� ��������� ����������� [ATmega16]
    ControlRegistersInit();

}


/**
 * ��������� ����������� ��������� �����������
 */
void CMCU::ControlRegistersInit(){

    uint8_t temp;

    // MCU Control Register
    // [ ������� ���������� ����������������� ][ATmega16]
    //          00000000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
    //          ||||||+-- 1, rw, ISC01: _|
    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
    //          ||||+---- 3, rw, ISC11: _|
    //          |||+----- 4, rw, SM0:   -+ - Sleep Mode Select Bits 0, 1
    //          ||+------ 5, rw, SM1:   _|
    //          |+------- 6, rw, SE:       - Sleep Enable
    //          +-------- 7, rw, SM2:      - Sleep Mode Select Bit 2
    // ����������:
    // ��� ����������� ��������� ������� ������� �������� MCUCR �� ����������
    // ���������
    //temp &= ( 1 << SM2 ) | ( 1 << SM1 ) | ( 1 << SM0 );
    MCUCR &= ~( ( 1 << SE ) | ( 1 << SM2 ) | ( 1 << SM1 ) | ( 1 << SM0 ) );
    MCUCR |= temp;


    // General Interrupt Control Register
    // [ ����� ������� ����������� ������������ ][ATmega16]
    //          00000000 - Initial Value
    //GICR = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, IVCE:  -
    //          ||||||+-- 1, rw, IVSEL: -
    //          |||||+--- 2, r: 0
    //          ||||+---- 3, r: 0
    //          |||+----- 4, r: 0
    //          ||+------ 5, rw, INT2:  - External Interrupt Request 2 Enable
    //          |+------- 6, rw, INT0:  - External Interrupt Request 0 Enable
    //          +-------- 7, rw, INT1:  - External Interrupt Request 1 Enable
    // ����������:


    // Timer/Counter Interrupt Mask Register
    // [ ������� ����� ���������� �� ��������/��������� ][ATmega16]
    //           00000000 - Initial Value
    //TIMSK = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, TOIE0:  - Timer/Counter0 Overflow Interrupt Enable
    //           ||||||+-- 1, rw, OCIE0:  - OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable
    //           |||||+--- 2, rw, TOIE1:  - Timer/Counter1, Overflow Interrupt Enable
    //           ||||+---- 3, rw, OCIE1B: - Timer/Counter1, Output Compare B Match Interrupt Enable
    //           |||+----- 4, rw, OCIE1A: - Timer/Counter1, Output Compare A Match Interrupt Enable
    //           ||+------ 5, rw, TICIE1: - Timer/Counter1, Input Capture Interrupt Enable
    //           |+------- 6, rw, TOIE2:  - Timer/Counter2 Overflow Interrupt Enable
    //           +-------- 7, rw, OCIE2:  - Timer/Counter2 Output Compare Match Interrupt Enable
    // ����������:


    // Timer/Counter Interrupt Flag Register
    // [ ������� ������ ���������� ��������/��������� ][ATmega16]
    //          00000000 - Initial Value
    //TIFR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, TOV0:  - Timer/Counter0 Overflow Flag
    //          ||||||+-- 1, rw, OCF0:  - Output Compare Flag 0
    //          |||||+--- 2, rw, TOV1:  - Timer/Counter1, Overflow Flag
    //          ||||+---- 3, rw, OCF1B: - Timer/Counter1, Output Compare B Match Flag
    //          |||+----- 4, rw, OCF1A: - Timer/Counter1, Output Compare A Match Flag
    //          ||+------ 5, rw, ICF1:  - Timer/Counter1, Input Capture Flag
    //          |+------- 6, rw, TOV2:  - Timer/Counter2 Overflow Flag
    //          +-------- 7, rw, OCF2:  - Output Compare Flag 2
    // ����������:


    // Special Function IO Register
    // [ ������� ����������� ������� �����/������ ][ATmega16]
    //           00000000 - Initial Value
    //SFIOR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
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
    // ����������:


    // ADC Control and Status Register A � ADCSRA
    // [ ADC Control and Status Register A ]
    //            00000000 - Initial Value
    //ADCSRA = BIN8(10001000); // BIN8() �� ������� �� ������ �����������
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
    // ����������:


}


/**
 * ��������� ���
 */
void CMCU::ADCInit(){

    // ADC Multiplexer Selection Register � ADMUX
    // [ ADC Multiplexer Selection Register ]
    //           00000000 - Initial Value
    //ADMUX = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, MUX0:  -+ - Analog Channel Selection Bits
    //           ||||||+-- 1, rw, MUX1:   |
    //           |||||+--- 2, rw, MUX2:   |
    //           ||||+---- 3, rw, MUX3:   |
    //           |||+----- 4, rw: MUX4:  _|
    //           ||+------ 5, rw, ADLAR:    - ADC Left Adjust Result
    //           |+------- 6, rw, REFS0: -+ - Reference Selection Bits
    //           +-------- 7, rw, REFS1: _|
    // ����������: 


    // Digital Input Disable Register 0 � DIDR0
    // [ Digital Input Disable Register 0 ]
    //           00000000 - Initial Value
    //DIDR0 = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
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
    // ����������: �� ����� ��� ADC7 � ADC6, �.�. �� ���.


    // ADC Control and Status Register B � ADCSRB
    // [ ADC Control and Status Register B ]
    //            00000000 - Initial Value
    //ADCSRB = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
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
    // ����������: Free Running mode.

}


/**
 * ��������� �������/�������� 0
 */
void CMCU::Timer0Init(){

    // [ATmega16] Table. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     �
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source. (������/������� ����������)                    |
    // | 0  | 0  | 1  | clkT0S/(No prescaling)                                          |
    // | 0  | 1  | 0  | clkT0S/8    (From prescaler)                                    |
    // | 0  | 1  | 1  | clkT0S/64   (From prescaler)                                    |
    // | 1  | 0  | 0  | clkT0S/256  (From prescaler)                                    |
    // | 1  | 0  | 1  | clkT0S/1024 (From prescaler)                                    |
    // | 1  | 1  | 0  | External clock source on T0 pin. Clock on falling edge.         |
    // | 1  | 1  | 1  | External clock source on T0 pin. Clock on rising edge.          |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter 0 Control Register
    // [ ������� ���������� ��������/��������� 0 ]
    //           00000000 - Initial Value
    //TCCR0 = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, CS00:  -+ - ���������� �������� ��������
    //           ||||||+-- 1, rw, CS01:   |
    //           |||||+--- 2, rw, CS02:  _|
    //           ||||+---- 3, rw, WGM01: -+ - Waveform Generation Mode
    //           |||+----- 4, rw, COM00: -+ - Compare Match Output Mode
    //           ||+------ 5, rw: COM01  _|
    //           |+------- 6, rw, WGM00:    - Waveform Generation Mode
    //           +-------- 7, w, FOC0:      - Force Output Compare
    // ����������:

    // ������������� �������� ��� �������� ��������
    //TCNT0 = 0xFF - F_CPU / 64000UL;

    // Timer/Counter0 Output Compare Register
    //OCR0 = 0x00;

    // Timer/Counter0 Asynchronous Status Register
    // [ ������� ... ][ATmega16]
    //          00000000 - Initial Value
    //ASSR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, r, TCR0UB: - Timer/Counter Control Register0 Update Busy
    //          ||||||+-- 1, r, OCR0UB: - Output Compare Register0 Update Busy
    //          |||||+--- 2, r, TCN0UB: - Timer/Counter0 Update Busy
    //          ||||+---- 3, rw, AS0:   - Asynchronous Timer/Counter0
    //          |||+----- 4, r: 0
    //          ||+------ 5, r: 0
    //          |+------- 6, r: 0
    //          +-------- 7, r: 0
    // ����������:

}


/**
 * ��������� �������/�������� 1
 */
void CMCU::Timer1Init(){

    // [ATmega16] Table. Clock Select Bit Description
    // +----+----+----+-----------------------------------------------------------------+
    // |CSn2|CSn1|CSn0| Description                                                     �
    // +----+----+----+-----------------------------------------------------------------+
    // | 0  | 0  | 0  | No clock source. (������/������� ����������)                    |
    // | 0  | 0  | 1  | clkI/O/1    (No prescaling)                                     |
    // | 0  | 1  | 0  | clkI/O/8    (From prescaler)                                    |
    // | 0  | 1  | 1  | clkI/O/64   (From prescaler)                                    |
    // | 1  | 0  | 0  | clkI/O/256  (From prescaler)                                    |
    // | 1  | 0  | 1  | clkI/O/1024 (From prescaler)                                    |
    // | 1  | 1  | 0  | ����� �n, ���� �������������� �� ���������� ������ ���������    |
    // | 1  | 1  | 1  | ����� �n, ���� �������������� �� ������������ ������ ���������  |
    // +----+----+----+-----------------------------------------------------------------+

    // Timer/Counter1 Control Register B
    // [ ������� ���������� B ��������/��������� 1 ][ATmega16]
    //            00000000 - Initial Value
    //TCCR1B = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, CS10:  -+ - ���������� �������� ��������
    //            ||||||+-- 1, rw, CS11:   |
    //            |||||+--- 2, rw, CS12:  _|
    //            ||||+---- 3, rw, WGM12: -+ - ����� ������ �������/��������
    //            |||+----- 4, rw, WGM13: _|
    //            ||+------ 5, r: 0          - ��������������, ������ ���� ���������� � 0 ��� ������
    //            |+------- 6, rw, ICES1:    - ����� ��������� ������ ������� �������
    //            +-------- 7, rw, ICNC1:    - ���������� ������ ���������� ����� ����� �������
    // ����������:

    // ������������� �������� ��� ������� ���������
    //TCNT1H = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue ) >> 8
    //TCNT1L = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue )
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
    // [ ������� ���������� A ��������/��������� 1 ][ATmega16]
    //            00000000 - Initial Value
    //TCCR1A = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, WGM10:  -+ - ����� ������ �������/��������
    //            ||||||+-- 1, rw, WGM11:  _|
    //            |||||+--- 2, rw, COM1C0: -+ - ����� ������ ������ ��������� C
    //            ||||+---- 3, rw, COM1C1: _|
    //            |||+----- 4, rw, COM1B0: -+ - ����� ������ ������ ��������� B
    //            ||+------ 5, rw, COM1B1: _|
    //            |+------- 6, rw, COM1A0: -+ - ����� ������ ������ ��������� A
    //            +-------- 7, rw, COM1A1: _|
    // ����������: ���������� ����� ������ ...

    //TCCR1B = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //            ||||||||
    //            76543210
    //            |||||||+- 0, rw, CS10:  -+ - ���������� �������� ��������
    //            ||||||+-- 1, rw, CS11:   |
    //            |||||+--- 2, rw, CS12:  _|
    //            ||||+---- 3, rw, WGM12: -+ - ����� ������ �������/��������
    //            |||+----- 4, rw, WGM13: _|
    //            ||+------ 5, r: 0          - ��������������, ������ ���� ���������� � 0 ��� ������
    //            |+------- 6, rw, ICES1:    - ����� ��������� ������ ������� �������
    //            +-------- 7, rw, ICNC1:    - ���������� ������ ���������� ����� ����� �������
    // ����������: ����������� ������� �������� ������� ���������� ������ ...
    // ����� ������: ...

}


/**
 * ��������� �������/�������� 2
 */
void CMCU::Timer2Init(){

    // Timer/Counter2 Control Register
    // [ ������� ���������� ��������/��������� 2 ][ATmega16]
    //           00000000 - Initial Value
    //TCCR2 = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, CS20:  -+ - Clock Select
    //           ||||||+-- 1, rw, CS21:   |
    //           |||||+--- 2, rw, CS22:  _|
    //           ||||+---- 3, rw, WGM21:    - Waveform Generation Mode
    //           |||+----- 4, rw, COM20: -+ - Compare Match Output Mode
    //           ||+------ 5, rw, COM21: _|
    //           |+------- 6, rw, WGM20:    - Waveform Generation Mode
    //           +-------- 7, w,  FOC2:     - Force Output Compare
    // ����������:

    // ������������� �������� ��� �������� ��������
    //TCNT2 =  0xFF - F_CPU / 64000UL;

    // Timer/Counter2 Output Compare Register
    //OCR2 = 0x00;

    // Timer/Counter2 Asynchronous Status Register
    // [ ������� ... ][ATmega16]
    //          00000000 - Initial Value
    //ASSR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, r, TCR2UB: - Timer/Counter Control Register2 Update Busy
    //          ||||||+-- 1, r, OCR2UB: - Output Compare Register2 Update Busy
    //          |||||+--- 2, r, TCN2UB: - Timer/Counter2 Update Busy
    //          ||||+---- 3, rw, AS2:   - Asynchronous Timer/Counter2
    //          |||+----- 4, r: 0
    //          ||+------ 5, r: 0
    //          |+------- 6, r: 0
    //          +-------- 7, r: 0
    // ����������:

}


/**
 * ��������� ����������������� ���������� SPI
 */
void CMCU::SPIInit(){

    // SPI Control Register
    // [ ������� ���������� SPI ][ATmega16]
    //          00000000 - Initial Value
    //SPCR = BIN8(0000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, SPR0: -+ - �������� ��������
    //          ||||||+-- 1, rw, SPR1: _|
    //          |||||+--- 2, rw, CPHA:    - ���� ��������� �������
    //          ||||+---- 3, rw, CPOL:    - ���������� ��������� �������
    //          |||+----- 4, rw, MSTR:    - ����� ������ ������ (Master/Slave)
    //          ||+------ 5, rw, DORD:    - ������� �������� ������
    //          |+------- 6, rw, SPE:     - ���������/���������� SPI
    //          +-------- 7, rw, SPIE:    - ���������� ���������� �� SPI
    // ����������:

    // SPI Status Register
    // [ ������� ������� SPI ][ATmega16]
    //          00000000 - Initial Value
    //SPSR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, SPI2X:    - Double SPI Speed Bit
    //          ||||||+-- 1, r, 0       -+ - ���������������
    //          |||||+--- 2, r, 0        |
    //          ||||+---- 3, r, 0        |
    //          |||+----- 4, r, 0        |
    //          ||+------ 5, r, 0       _|
    //          |+------- 6, r, WCOL:      - Write COLlision flag
    //          +-------- 7, r, SPIF:      - SPI Interrupt Flag
    // ����������:

}


/**
 * ��������� ����������������� ���������� TWI
 */
void CMCU::TWIInit(){

    // TWI Bit Rate Register
    //TWBR = 1; // TODO: �������� ������� ��� ��������� ��������

    // TWI Control Register
    // [ ������� ���������� TWI ][ATmega16]
    //          00000000 - Initial Value
    //TWCR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
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
    // ����������:

    // TWI Status Register
    // [ TWI ������ ������� ][ATmega16]
    //          11111000 - Initial Value
    //TWSR = BIN8(11111000); // BIN8() �� ������� �� ������ �����������
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
    // ����������:

    // TWI (Slave) Address Register
    // ����� � ����, ��� ������� �������� ��� TWGCE (TWI General Call Recognition Enable Bit)
    //TWAR = 0xFE;

}


/**
 * ��������� ����������� USART
 */
void CMCU::USARTInit(){

    // USART Control and Status Register A
    // [ ������� ���������� UCSRA ][ATmega16]
    //           00100000 - Initial Value
    //UCSRA = BIN8(00100000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, MPCM: - Multi-processor Communication Mode
    //           ||||||+-- 1, rw, U2X:  - Double the USART Transmission Speed
    //           |||||+--- 2, r, PE:    - Parity Error
    //           ||||+---- 3, r, DOR:   - Data OverRun
    //           |||+----- 4, r, FE:    - Frame Error
    //           ||+------ 5, r, UDRE:  - USART Data Register Empty
    //           |+------- 6, rw, TXC:  - USART Transmit Complete
    //           +-------- 7, r, RXC:   - USART Receive Complete
    // ����������:

    //UCSRB = 0x00; // ���������, ���� ����������� ��������

    // ����������� BAUD ��. � �����: "Configuration.h"
    //UBRRL = ( uint8_t ) ( F_CPU / ( 16UL * BAUD ) - 1UL ); // ������������� ��������
    //UBRRH = ( uint8_t ) ( ( F_CPU / ( 16UL * BAUD ) - 1UL ) >> 8 );

    // USART Control and Status Register B
    // [ ������� ���������� UCSRB ][ATmega16]
    //           00000000 - Initial Value
    //UCSRB = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
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
    // ����������:

    // USART Control and Status Register C
    // [ ������� ���������� UCSRC ][ATmega16]
    //           10000110 - Initial Value
    //UCSRC = BIN8(10000110); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, UCPOL:    - Clock Polarity
    //           ||||||+-- 1, rw, UCSZ0: -+ - Character Size
    //           |||||+--- 2, rw, UCSZ1: _|
    //           ||||+---- 3, rw, USBS:     - Stop Bit Select
    //           |||+----- 4, rw, UPM0:  -+ - Parity Mode
    //           ||+------ 5, rw, UPM1:  _|
    //           |+------- 6, rw, UMSEL:    - USART Mode Select
    //           +-------- 7, rw: URSEL     - Register Select
    // ����������:

}


/**
 * -=[ ����� ���������� (�������� �������) MCU ATmega16 ]=-
 */
void CMCU::PortsInit(){

    // ��������� �������:
    // ��� �������: ������ ��� ATmega16
    // ���� ����� (URI): file:///
    // ��� ����� ����� (MD5):
    // ���:
    // ����: 17.11.2012
    // ���������� ������������:
    //  mailto:unihomelab@ya.ru
    //  skype: viacheslavmezentsev

//                 [ATmega16]                                                   TQFP/QFN/MLF
//
//                                                                                  +----------[39]- GND
//                                                     (XCK/T0) PB0 -[40]---------+ | +--------[38]- VCC
//                         PDIP                            (T1) PB1 -[41]-------+ | | | +------[37]- PA0 (ADC0)
//                   +--------------+               (AIN0/INT2) PB2 -[42]-----+ | | | | | +----[36]- PA1 (ADC1)
//     (XCK/T0) PB0 -|  1        40 |- PA0 (ADC0)    (AIN1/OC0) PB3 -[43]---+ | | | | | | | +--[35]- PA2 (ADC2)
//         (T1) PB1 -|  2        39 |- PA1 (ADC1)         (~SS) PB4 -[44]-+ | | | | | | | | | +[34]- PA3 (ADC3)
//  (INT2/AIN0) PB2 -|  3        38 |- PA2 (ADC2)                         | | | | | | | | | | |
//   (OC0/AIN1) PB3 -|  4        37 |- PA3 (ADC3)                    +----------------------------+
//        (~SS) PB4 -|  5        36 |- PA4 (ADC4)                    |O  44  42  40  38  36  34   |
//       (MOSI) PB5 -|  6        35 |- PA5 (ADC5)                    |     43  41  39  37  35     |
//       (MISO) PB6 -|  7        34 |- PA6 (ADC6)        (MOSI) PB5 -|  1 +------------------+ 33 |- PA4 (ADC4)
//        (SCK) PB7 -|  8        33 |- PA7 (ADC7)        (MISO) PB6 -|  2 |                  | 32 |- PA5 (ADC5)
//           ~RESET -|  9        32 |- AREF               (SCK) PB7 -|  3 |                  | 31 |- PA6 (ADC6)
//              VCC -| 10        31 |- GND                   ~RESET -|  4 |                  | 30 |- PA7 (ADC7)
//              GND -| 11        30 |- AVCC                     VCC -|  5 |                  | 29 |- AREF
//            XTAL2 -| 12        29 |- PC7 (TOSC2)              GND -|  6 |                  | 28 |- GND
//            XTAL1 -| 13        28 |- PC6 (TOSC1)            XTAL2 -|  7 |                  | 27 |- AVCC
//        (RXD) PD0 -| 14        27 |- PC5 (TDI)              XTAL1 -|  8 |                  | 26 |- PC7 (TOSC2)
//        (TXD) PD1 -| 15        26 |- PC4 (TDO)          (RXD) PD0 -|  9 |                  | 25 |- PC6 (TOSC1)
//       (INT0) PD2 -| 16        25 |- PC3 (TMS)          (TXD) PD1 -| 10 |                  | 24 |- PC5 (TDI)
//       (INT1) PD3 -| 17        24 |- PC2 (TCK)         (INT0) PD2 -| 11 +------------------+ 23 |- PC4 (TDO)
//       (OC1B) PD4 -| 18        23 |- PC1 (SDA)                     |     13  15  17  19  21     |
//       (OC1A) PD5 -| 19        22 |- PC0 (SCL)                     |   12  14  16  18  20  22   |
//       (ICP1) PD6 -| 20        21 |- PD7 (OC2)                     +----------------------------+
//                   +--------------+                                     | | | | | | | | | | |
//                                                       (INT1) PD3 -[12]-+ | | | | | | | | | +[22]- PC3 (TMS)
//                                                       (OC1B) PD4 -[13]---+ | | | | | | | +--[21]- PC2 (TCK)
//                                                       (OC1A) PD5 -[14]-----+ | | | | | +----[20]- PC1 (SDA)
//                                                       (ICP1) PD6 -[15]-------+ | | | +------[19]- PC0 (SCL)
//                                                        (OC2) PD7 -[16]---------+ | +--------[18]- GND
//                                                                                  +----------[17]- VCC


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
    // ���� �� ��� PUD ���������� � 1, �������������
    // ��������� ����������� �� ���� ������� ����������������.

    //sbi( SFIOR, PUD );
    cbi( SFIOR, PUD );

    // ��������� ������: A, B, C, D. ��������� ������������� �������

    // Port A Data Direction Register
    // [ ������� ����������� ����� A ][ATmega16]
    //          00000000 - Initial Value
    DDRA = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDA0: (ADC0) -
    //          ||||||+-- 1, rw, DDA1: (ADC1) -
    //          |||||+--- 2, rw, DDA2: (ADC2) -
    //          ||||+---- 3, rw, DDA3: (ADC3) -
    //          |||+----- 4, rw, DDA4: (ADC4) -
    //          ||+------ 5, rw, DDA5: (ADC5) -
    //          |+------- 6, rw, DDA6: (ADC6) -
    //          +-------- 7, rw, DDA7: (ADC7) -
    // ����������:


    // Port A Data Register
    // [ ������� ������ ����� A ][ATmega16]
    //           00000000 - Initial Value
    PORTA = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTA0: (ADC0) -
    //           ||||||+-- 1, rw, PORTA1: (ADC1) -
    //           |||||+--- 2, rw, PORTA2: (ADC2) -
    //           ||||+---- 3, rw, PORTA3: (ADC3) -
    //           |||+----- 4, rw, PORTA4: (ADC4) -
    //           ||+------ 5, rw, PORTA5: (ADC5) -
    //           |+------- 6, rw, PORTA6: (ADC6) -
    //           +-------- 7, rw, PORTA7: (ADC7) -
    // ����������:


    // Port B Data Direction Register
    // [ ������� ����������� ����� B ][ATmega16]
    //          00000000 - Initial Value
    DDRB = BIN8(11110111); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDB0: (XCK/T0)    - LCD RS
    //          ||||||+-- 1, rw, DDB1: (T1)        - LCD RW
    //          |||||+--- 2, rw, DDB2: (INT2/AIN0) - LCD E
    //          ||||+---- 3, rw, DDB3: (OC0/AIN1)  -
    //          |||+----- 4, rw, DDB4: (~SS)       - LCD_D4
    //          ||+------ 5, rw, DDB5: (MOSI)      - LCD_D5
    //          |+------- 6, rw, DDB6: (MISO)      - LCD_D6
    //          +-------- 7, rw, DDB7: (SCK)       - LCD_D7
    // ����������:


    // Port B Data Register
    // [ ������� ������ ����� B ][ATmega16]
    //           00000000 - Initial Value
    PORTB = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTB0: (XCK/T0)    - LCD RS
    //           ||||||+-- 1, rw, PORTB1: (T1)        - LCD RW
    //           |||||+--- 2, rw, PORTB2: (INT2/AIN0) - LCD E
    //           ||||+---- 3, rw, PORTB3: (OC0/AIN1)  -
    //           |||+----- 4, rw, PORTB4: (~SS)       - LCD_D4
    //           ||+------ 5, rw, PORTB5: (MOSI)      - LCD_D5
    //           |+------- 6, rw, PORTB6: (MISO)      - LCD_D6
    //           +-------- 7, rw, PORTB7: (SCK)       - LCD_D7
    // ����������:


    // Port C Data Direction Register
    // [ ������� ����������� ����� C ][ATmega16]
    //          00000000 - Initial Value
    DDRC = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDC0: (SCL)   -
    //          ||||||+-- 1, rw, DDC1: (SDA)   -
    //          |||||+--- 2, rw, DDC2: (TCK)   -
    //          ||||+---- 3, rw, DDC3: (TMS)   -
    //          |||+----- 4, rw, DDC4: (TDO)   -
    //          ||+------ 5, rw, DDC5: (TDI)   -
    //          |+------- 6, rw, DDC6: (TOSC1) -
    //          +-------- 7, rw, DDC7: (TOSC2) -
    // ����������:


    // Port C Data Register
    // [ ������� ������ ����� C ][ATmega16]
    //           00000000 - Initial Value
    PORTC = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTC0: (SCL)   -
    //           ||||||+-- 1, rw, PORTC1: (SDA)   -
    //           |||||+--- 2, rw, PORTC2: (TCK)   -
    //           ||||+---- 3, rw, PORTC3: (TMS)   -
    //           |||+----- 4, rw, PORTC4: (TDO)   -
    //           ||+------ 5, rw, PORTC5: (TDI)   -
    //           |+------- 6, rw, PORTC6: (TOSC1) -
    //           +-------- 7, rw, PORTC7: (TOSC2) -
    // ����������:


    // Port D Data Direction Register
    // [ ������� ����������� ����� D ][ATmega16]
    //          00000000 - Initial Value
    DDRD = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, DDD0: (RXD)  - RXD
    //          ||||||+-- 1, rw, DDD1: (TXD)  - TXD
    //          |||||+--- 2, rw, DDD2: (INT0) -
    //          ||||+---- 3, rw, DDD3: (INT1) -
    //          |||+----- 4, rw, DDD4: (OC1B) -
    //          ||+------ 5, rw, DDD5: (OC1A) -
    //          |+------- 6, rw, DDD6: (ICP1) -
    //          +-------- 7, rw, DDD7: (OC2)  -
    // ����������:


    // Port D Data Register
    // [ ������� ������ ����� D ][ATmega16]
    //           00000000 - Initial Value
    PORTD = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, PORTD0: (RXD)  - RXD
    //           ||||||+-- 1, rw, PORTD1: (TXD)  - TXD
    //           |||||+--- 2, rw, PORTD2: (INT0) -
    //           ||||+---- 3, rw, PORTD3: (INT1) -
    //           |||+----- 4, rw, PORTD4: (OC1B) -
    //           ||+------ 5, rw, PORTD5: (OC1A) -
    //           |+------- 6, rw, PORTD6: (ICP1) -
    //           +-------- 7, rw, PORTD7: (OC2)  -
    // ����������:


}


/**
 * ��������� ����������� ����������� �������
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
    // [ ������� ���������� ����������� ������� ][ATmega16]
    //           00000000 - Initial Value
    //WDTCR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
    //           ||||||||
    //           76543210
    //           |||||||+- 0, rw, WDP0: -+ - ����������� ������� ������������ ����������� �������
    //           ||||||+-- 1, rw, WDP1:  |
    //           |||||+--- 2, rw, WDP2: _|
    //           ||||+---- 3, rw, WDE:     - ���������� ������ �� ����-���� ����������� �������
    //           |||+----- 4, rw, WDCE:    - ���������� ��������� ������������ ����������� �������
    //           ||+------ 5, r,        -+ - Reserved Bits
    //           |+------- 6, r,         |
    //           +-------- 7, r,        _|
    // ����������:
    // ������� ���������� ������. ����� � ���� ��� ���������� ����������
    // ����� ���������� ������� ������� ������� ��� ���������� � ������
    // ������������� �������.

}


/**
 * ��������� ������ � �������� ������������
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

    // ��������� ������� ����������
    GICR &= ~( ( 1 << INT0 ) | ( 1 << INT1 ) | ( 1 << INT2 ) );

    // ��������� ���������� INT0 � INT1
    // MCU Control Register
    // [ ������� ���������� ����������������� ]
    //          ****0000 - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
    //          ||||||+-- 1, rw, ISC01: _|
    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
    //          ||||+---- 3, rw, ISC11: _|
    //          |||+----- 4, rw, SM0:      -
    //          ||+------ 5, rw, SM1:      -
    //          |+------- 6, rw, SE:       -
    //          +-------- 7, rw, SM2:      -
    // ����������:
    // ��� ����������� ��������� ������� ������� �������� MCUCR �� ����������
    // ���������
    //temp &= ( 1 << ISC11 ) | ( 1 << ISC01 );
    MCUCR &= ~( ( 1 << ISC11 ) | ( 1 << ISC10 ) | ( 1 << ISC01 ) | ( 1 << ISC00 ) );
    MCUCR |= temp;

    // ��������� ���������� INT2
    // If ISC2 is written to zero, a falling edge on INT2 activates the
    // interrupt. If ISC2 is written to one, a rising edge on INT2
    // activates the interrupt.

    // MCU Control and Status Register
    // [ ������� ���������� ����������������� ]
    //          *0****** - Initial Value
    temp = BIN8(00000000);
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, PORF:  -
    //          ||||||+-- 1, rw, EXTRF: -
    //          |||||+--- 2, rw, BORF:  -
    //          ||||+---- 3, rw, WDRF:  -
    //          |||+----- 4, rw, JTRF:  -
    //          ||+------ 5, r: 0
    //          |+------- 6, rw, ISC2:  - Interrupt Sense Control 2
    //          +-------- 7, rw, JTD:   -
    // ����������:
    // ��� ����������� ��������� ������ ������� �������� MCUCSR �� ����������
    // ���������
    //temp &= ( 1 << ISC2 );
    MCUCSR &= ~( 1 << ISC2 );
    MCUCSR |= temp;

}


// -=[ ����������� ���������� ]=-

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
 * External Interrupt Request 2
 */
void CMCU::OnExternalInterruptRequest2(){

}


/**
 * Timer/Counter2 Compare Match
 */
void CMCU::OnTimerCounter2CompareMatch(){

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
 * Timer/Counter Compare Match B
 */
void CMCU::OnTimerCounter1CompareMatchB(){

}


/**
 * Timer/Counter1 Overflow
 */
void CMCU::OnTimerCounter1Overflow(){

}


/**
 * Timer/Counter0 Compare Match
 */
void CMCU::OnTimerCounter0CompareMatch(){

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
void CMCU::OnADCConversionComplete(){

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
