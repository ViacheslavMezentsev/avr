$nocompile


'/**
' * ������� (��������) ����� ���������
' */
Sub MCU_MainThreadProcedure

    Do

    Loop

End Sub


'/**
' * ������������� ���� ���������
' */
Sub MCU_Initialization

    ' ����� ���������� (�������� �������) [ATmega16]
    MCU_PortsInit

    ' ��������� ��� [ATmega16]
    'MCU_ADCInit

    ' ��������� �������/�������� 0 [ATmega16]
    'MCU_Timer0Init

    ' ��������� �������/�������� 1 [ATmega16]
    'MCU_Timer1Init()

    ' ��������� �������/�������� 2 [ATmega16]
    'MCU_Timer2Init

    ' ��������� ����������� ����������� ������� [ATmega16]
    'MCU_InternalWDTInit()

    ' ��������� ����������� USART [ATmega16]
    'MCU_USARTInit

    ' ��������� ����������������� ���������� SPI [ATmega16]
    'MCU_SPIInit()

    ' ��������� TWI [ATmega16]
    I2cinit

    ' ��������� ������ � �������� ������������ [ATmega16]
    'MCU_ExternalInterruptsInit

    ' ��������� ����������� ��������� ����������� [ATmega16]
    MCU_ControlRegistersInit

End Sub


'/**
' * ��������� ����������� ��������� �����������
' */
Sub MCU_ControlRegistersInit

'    Dim temp As Byte

'    // MCU Control Register
'    // [ ������� ���������� ����������������� ][ATmega16]
'    //          00000000 - Initial Value
'    temp = BIN8(00000000);
'    //          ||||||||
'    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
'    //          ||||||+-- 1, rw, ISC01: _|
'    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
'    //          ||||+---- 3, rw, ISC11: _|
'    //          |||+----- 4, rw, SM0:   -+ - Sleep Mode Select Bits 0, 1
'    //          ||+------ 5, rw, SM1:   _|
'    //          |+------- 6, rw, SE:       - Sleep Enable
'    //          +-------- 7, rw, SM2:      - Sleep Mode Select Bit 2
'    // ����������:
'    // ��� ����������� ��������� ������� ������� �������� MCUCR �� ����������
'    // ���������
'    //temp &= ( 1 << SM2 ) | ( 1 << SM1 ) | ( 1 << SM0 );
'    MCUCR &= ~( ( 1 << SE ) | ( 1 << SM2 ) | ( 1 << SM1 ) | ( 1 << SM0 ) );
'    MCUCR |= temp;


'    // General Interrupt Control Register
'    // [ ����� ������� ����������� ������������ ][ATmega16]
'    //          00000000 - Initial Value
        GICR = &B00000000
'    //          ||||||||
'    //          |||||||+- 0, rw, IVCE:  -
'    //          ||||||+-- 1, rw, IVSEL: -
'    //          |||||+--- 2, r: 0
'    //          ||||+---- 3, r: 0
'    //          |||+----- 4, r: 0
'    //          ||+------ 5, rw, INT2:  - External Interrupt Request 2 Enable
'    //          |+------- 6, rw, INT0:  - External Interrupt Request 0 Enable
'    //          +-------- 7, rw, INT1:  - External Interrupt Request 1 Enable
'    // ����������:


'    // Timer/Counter Interrupt Mask Register
'    // [ ������� ����� ���������� �� ��������/��������� ][ATmega16]
'    //           00000000 - Initial Value
        TIMSK = &B00000000
'    //           ||||||||
'    //           |||||||+- 0, rw, TOIE0:  - Timer/Counter0 Overflow Interrupt Enable
'    //           ||||||+-- 1, rw, OCIE0:  - OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable
'    //           |||||+--- 2, rw, TOIE1:  - Timer/Counter1, Overflow Interrupt Enable
'    //           ||||+---- 3, rw, OCIE1B: - Timer/Counter1, Output Compare B Match Interrupt Enable
'    //           |||+----- 4, rw, OCIE1A: - Timer/Counter1, Output Compare A Match Interrupt Enable
'    //           ||+------ 5, rw, TICIE1: - Timer/Counter1, Input Capture Interrupt Enable
'    //           |+------- 6, rw, TOIE2:  - Timer/Counter2 Overflow Interrupt Enable
'    //           +-------- 7, rw, OCIE2:  - Timer/Counter2 Output Compare Match Interrupt Enable
'    // ����������:

'    // Timer/Counter Interrupt Flag Register
'    // [ ������� ������ ���������� ��������/��������� ][ATmega16]
'    //          00000000 - Initial Value
'    //TIFR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, rw, TOV0:  - Timer/Counter0 Overflow Flag
'    //          ||||||+-- 1, rw, OCF0:  - Output Compare Flag 0
'    //          |||||+--- 2, rw, TOV1:  - Timer/Counter1, Overflow Flag
'    //          ||||+---- 3, rw, OCF1B: - Timer/Counter1, Output Compare B Match Flag
'    //          |||+----- 4, rw, OCF1A: - Timer/Counter1, Output Compare A Match Flag
'    //          ||+------ 5, rw, ICF1:  - Timer/Counter1, Input Capture Flag
'    //          |+------- 6, rw, TOV2:  - Timer/Counter2 Overflow Flag
'    //          +-------- 7, rw, OCF2:  - Output Compare Flag 2
'    // ����������:

'    // Special Function IO Register
'    // [ ������� ����������� ������� �����/������ ][ATmega16]
'    //           00000000 - Initial Value
'        SFIOR = &B00000000
'    //           ||||||||
'    //           |||||||+- 0, rw, PSR321: - Prescaler Reset Timer/Counter3,
'    //           |||||||                  - Timer/Counter2, and Timer/Counter1
'    //           ||||||+-- 1, rw, PSR0:   - Prescaler Reset Timer/Counter0
'    //           |||||+--- 2, rw, PUD:    - Pull-up disable
'    //           ||||+---- 3, rw, ACME:   - Analog Comparator Multiplexer Enable
'    //           |||+----- 4, r: 0
'    //           ||+------ 5, r: 0
'    //           |+------- 6, r: 0
'    //           +-------- 7, rw, TSM:    - Timer/Counter Synchronization Mode
'    // ����������:


'    // ADC Control and Status Register A � ADCSRA
'    // [ ADC Control and Status Register A ]
'    //            00000000 - Initial Value
'    ADCSRA = BIN8(10001000); // BIN8() �� ������� �� ������ �����������
'    //            ||||||||
'    //            |||||||+- 0, rw, ADPS0: -+ - ADC Prescaler Select Bits
'    //            ||||||+-- 1, rw, ADPS1:  |
'    //            |||||+--- 2, rw, ADPS2: _|
'    //            ||||+---- 3, rw, ADIE:     - ADC Interrupt Enable
'    //            |||+----- 4, rw, ADIF:     - ADC Interrupt Flag
'    //            ||+------ 5, rw, ADATE:    - ADC Auto Trigger Enable
'    //            |+------- 6, rw, ADSC:     - ADC Start Conversion
'    //            +-------- 7, rw, ADEN:     - ADC Enable
'    // ����������:

End Sub


'/**
' * -=[ ����� ���������� (�������� �������) MCU ATmega16 ]=-
' */
Sub MCU_PortsInit

'    // ��������� �������:
'    // ��� �������: ATmega16 (�� ������ Pinboard II)
'    // ���� ����� (URI): file:///
'    // ��� ����� ����� (MD5):
'    // ���:
'    // ����: 13.03.2012
'    // ���������� ������������:
'    //  mailto:unihomelab@ya.ru
'    //  skype: viacheslavmezentsev

'    // Table 25. Port Pin Configurations
'    //+-----+------+----------+--------+---------+--------------------------------------------+
'    //| DDxn|PORTxn|   PUD    |  I/O   | Pull-up |   Comment                                  |
'    //|     |      |(in SFIOR)|        |         |                                            |
'    //+-----+------+----------+--------+---------+--------------------------------------------+
'    //|  0  |  0   |    X     | Input  |   No    | Tri-state (Hi-Z)                           |
'    //|  0  |  1   |    0     | Input  |   Yes   | Pxn will source current if ext. pulled low.|
'    //|  0  |  1   |    1     | Input  |   No    | Tri-state (Hi-Z)                           |
'    //|  1  |  0   |    X     | Output |   No    | Output Low (Sink)                          |
'    //|  1  |  1   |    X     | Output |   No    | Output High (Source)                       |
'    //+-----+------+----------+--------+---------+--------------------------------------------+
'    // ���� �� ��� PUD ���������� � 1, �������������
'    // ��������� ����������� �� ���� ������� ����������������.

'    sbi( SFIOR, PUD );
'    //cbi( SFIOR, PUD );

    Reset SFIOR.PUD
'
'    // ��������� ������: A, B, C, D. ��������� ������������� �������

'    // Port A Data Direction Register
'    // [ ������� ����������� ����� A ][ATmega16]
'    //          00000000 - Initial Value
        DDRA = &B00000000
'    //          ||||||||
'    //          |||||||+- 0, rw, DDA0: (ADC0) - ������������
'    //          ||||||+-- 1, rw, DDA1: (ADC1) - BTN3
'    //          |||||+--- 2, rw, DDA2: (ADC2) - BTN2
'    //          ||||+---- 3, rw, DDA3: (ADC3) - BTN1
'    //          |||+----- 4, rw, DDA4: (ADC4) - Segment E
'    //          ||+------ 5, rw, DDA5: (ADC5) - Segment D
'    //          |+------- 6, rw, DDA6: (ADC6) - Segment H
'    //          +-------- 7, rw, DDA7: (ADC7) - Segment C
'    // ����������:

'    // Port A Data Register
'    // [ ������� ������ ����� A ][ATmega16]
'    //           00000000 - Initial Value
        PORTA = &B00000000
'    //           ||||||||
'    //           |||||||+- 0, rw, PORTA0: (ADC0) - ������������
'    //           ||||||+-- 1, rw, PORTA1: (ADC1) - BTN3
'    //           |||||+--- 2, rw, PORTA2: (ADC2) - BTN2
'    //           ||||+---- 3, rw, PORTA3: (ADC3) - BTN1
'    //           |||+----- 4, rw, PORTA4: (ADC4) - Segment E
'    //           ||+------ 5, rw, PORTA5: (ADC5) - Segment D
'    //           |+------- 6, rw, PORTA6: (ADC6) - Segment H
'    //           +-------- 7, rw, PORTA7: (ADC7) - Segment C
'    // ����������:


'    // Port B Data Direction Register
'    // [ ������� ����������� ����� B ][ATmega16]
'    //          00000000 - Initial Value
        DDRB = &B11110011
'    //          ||||||||
'    //          |||||||+- 0, rw, DDB0: (XCK/T0)    - LCD E
'    //          ||||||+-- 1, rw, DDB1: (T1)        - LCD RW / Dig2
'    //          |||||+--- 2, rw, DDB2: (INT2/AIN0) - Clock PC AT (PS/2) ���������� / Ch A (�������)
'    //          ||||+---- 3, rw, DDB3: (OC0/AIN1)  - �������
'    //          |||+----- 4, rw, DDB4: (~SS)       - LCD_D4
'    //          ||+------ 5, rw, DDB5: (MOSI)      - LCD_D5
'    //          |+------- 6, rw, DDB6: (MISO)      - LCD_D6
'    //          +-------- 7, rw, DDB7: (SCK)       - LCD_D7
'    // ����������:

'    // Port B Data Register
'    // [ ������� ������ ����� B ][ATmega16]
'    //           00000000 - Initial Value
        PORTB = &B00000000
'    //           ||||||||
'    //           |||||||+- 0, rw, PORTB0: (XCK/T0)    - LCD E
'    //           ||||||+-- 1, rw, PORTB1: (T1)        - LCD RW
'    //           |||||+--- 2, rw, PORTB2: (INT2/AIN0) - Clock PC AT (PS/2) ���������� / Ch A (�������)
'    //           ||||+---- 3, rw, PORTB3: (OC0/AIN1)  - �������
'    //           |||+----- 4, rw, PORTB4: (~SS)       - LCD_D4
'    //           ||+------ 5, rw, PORTB5: (MOSI)      - LCD_D5
'    //           |+------- 6, rw, PORTB6: (MISO)      - LCD_D6
'    //           +-------- 7, rw, PORTB7: (SCK)       - LCD_D7
'    // ����������:


'    // Port C Data Direction Register
'    // [ ������� ����������� ����� C ][ATmega16]
'    //          00000000 - Initial Value
        DDRC = &B01000000
'    //          ||||||||
'    //          |||||||+- 0, rw, DDC0: (SCL)   - SCL
'    //          ||||||+-- 1, rw, DDC1: (SDA)   - SDA
'    //          |||||+--- 2, rw, DDC2: (TCK)   - JTAG
'    //          ||||+---- 3, rw, DDC3: (TMS)   - JTAG
'    //          |||+----- 4, rw, DDC4: (TDO)   - JTAG
'    //          ||+------ 5, rw, DDC5: (TDI)   - JTAG
'    //          |+------- 6, rw, DDC6: (TOSC1) - LCD RS / Dig1
'    //          +-------- 7, rw, DDC7: (TOSC2) - Ch B (�������)
'    // ����������:

'    // Port C Data Register
'    // [ ������� ������ ����� C ][ATmega16]
'    //           00000000 - Initial Value
        PORTC = &B00000000
'    //           ||||||||
'    //           |||||||+- 0, rw, PORTC0: (SCL)   - SCL
'    //           ||||||+-- 1, rw, PORTC1: (SDA)   - SDA
'    //           |||||+--- 2, rw, PORTC2: (TCK)   - JTAG
'    //           ||||+---- 3, rw, PORTC3: (TMS)   - JTAG
'    //           |||+----- 4, rw, PORTC4: (TDO)   - JTAG
'    //           ||+------ 5, rw, PORTC5: (TDI)   - JTAG
'    //           |+------- 6, rw, PORTC6: (TOSC1) - LCD RS / Dig1
'    //           +-------- 7, rw, PORTC7: (TOSC2) - Ch B (�������)
'    // ����������:


'    // Port D Data Direction Register
'    // [ ������� ����������� ����� D ][ATmega16]
'    //          00000000 - Initial Value
        DDRD = &B00100000
'    //          ||||||||
'    //          |||||||+- 0, rw, DDD0: (RXD)  - RXD
'    //          ||||||+-- 1, rw, DDD1: (TXD)  - TXD
'    //          |||||+--- 2, rw, DDD2: (INT0) - V-USB D+
'    //          ||||+---- 3, rw, DDD3: (INT1) - ����������� ����������
'    //          |||+----- 4, rw, DDD4: (OC1B) - V-USB D-
'    //          ||+------ 5, rw, DDD5: (OC1A) - V-USB led
'    //          |+------- 6, rw, DDD6: (ICP1) - Data PC AT (PS/2) ����������
'    //          +-------- 7, rw, DDD7: (OC2)  -
'    // ����������:


'    // Port D Data Register
'    // [ ������� ������ ����� D ][ATmega16]
'    //           00000000 - Initial Value
        PORTD = &B00010100
'    //           ||||||||
'    //           |||||||+- 0, rw, PORTD0: (RXD)  - RXD
'    //           ||||||+-- 1, rw, PORTD1: (TXD)  - TXD
'    //           |||||+--- 2, rw, PORTD2: (INT0) - V-USB D+
'    //           ||||+---- 3, rw, PORTD3: (INT1) - ����������� ����������
'    //           |||+----- 4, rw, PORTD4: (OC1B) - V-USB D-
'    //           ||+------ 5, rw, PORTD5: (OC1A) - V-USB led
'    //           |+------- 6, rw, PORTD6: (ICP1) - Data PC AT (PS/2) ����������
'    //           +-------- 7, rw, PORTD7: (OC2)  -
'    // ����������:


End Sub


'/**
' * ������ � �������� �� �����
' */
Sub MCU_KeypadProcedure

End Sub


'/**
' * ��������� ���
' */
Sub MCU_ADCInit

'    // ADC Multiplexer Selection Register � ADMUX
'    // [ ADC Multiplexer Selection Register ]
'    //           00000000 - Initial Value
'    ADMUX = BIN8(01000000); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, MUX0:  -+ - Analog Channel Selection Bits
'    //           ||||||+-- 1, rw, MUX1:   |
'    //           |||||+--- 2, rw, MUX2:   |
'    //           ||||+---- 3, rw, MUX3:   |
'    //           |||+----- 4, rw: MUX4:  _|
'    //           ||+------ 5, rw, ADLAR:    - ADC Left Adjust Result
'    //           |+------- 6, rw, REFS0: -+ - Reference Selection Bits
'    //           +-------- 7, rw, REFS1: _|
'    // ����������: AVCC


'    // Digital Input Disable Register 0 � DIDR0
'    // [ Digital Input Disable Register 0 ]
'    //           00000000 - Initial Value
'    //DIDR0 = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, ADC0D:  -+ - ADC5..0 Digital Input Disable
'    //           ||||||+-- 1, rw, ADC1D:   |
'    //           |||||+--- 2, rw, ADC2D:   |
'    //           ||||+---- 3, rw, ADC3D:   |
'    //           |||+----- 4, rw, ADC4D:   |
'    //           ||+------ 5, rw, ADC5D:  _|
'    //           |+------- 6, r: 0
'    //           +-------- 7, r: 0
'    // ����������: �� ����� ��� ADC7 � ADC6, �.�. �� ���.


'    // ADC Control and Status Register B � ADCSRB
'    // [ ADC Control and Status Register B ]
'    //            00000000 - Initial Value
'    //ADCSRB = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //            ||||||||
'    //            |||||||+- 0, rw, ADTS0: -+ - ADC Auto Trigger Source
'    //            ||||||+-- 1, rw, ADTS1:  |
'    //            |||||+--- 2, rw, ADTS2: _|
'    //            ||||+---- 3, r: 0
'    //            |||+----- 4, r: 0
'    //            ||+------ 5, r: 0
'    //            |+------- 6, rw, ACME:     - (?)
'    //            +-------- 7, r: 0
'    // ����������: Free Running mode.

End Sub


'/**
' * ��������� �������/�������� 0
' */
Sub MCU_Timer0Init

'    // [ATmega16] Table 42, p. 85. Clock Select Bit Description
'    // +----+----+----+-----------------------------------------------------------------+
'    // |CSn2|CSn1|CSn0| Description                                                     �
'    // +----+----+----+-----------------------------------------------------------------+
'    // | 0  | 0  | 0  | No clock source. (������/������� ����������)                    |
'    // | 0  | 0  | 1  | clkT0S/(No prescaling)                                          |
'    // | 0  | 1  | 0  | clkT0S/8    (From prescaler)                                    |
'    // | 0  | 1  | 1  | clkT0S/64   (From prescaler)                                    |
'    // | 1  | 0  | 0  | clkT0S/256  (From prescaler)                                    |
'    // | 1  | 0  | 1  | clkT0S/1024 (From prescaler)                                    |
'    // | 1  | 1  | 0  | External clock source on T0 pin. Clock on falling edge.         |
'    // | 1  | 1  | 1  | External clock source on T0 pin. Clock on rising edge.          |
'    // +----+----+----+-----------------------------------------------------------------+

'    // Timer/Counter 0 Control Register
'    // [ ������� ���������� ��������/��������� 0 ]
'    //           00000000 - Initial Value
'    TCCR0 = BIN8(00000011); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, CS00:  -+ - ���������� �������� ��������
'    //           ||||||+-- 1, rw, CS01:   |
'    //           |||||+--- 2, rw, CS02:  _|
'    //           ||||+---- 3, rw, WGM01: -+ - Waveform Generation Mode
'    //           |||+----- 4, rw, COM00: -+ - Compare Match Output Mode
'    //           ||+------ 5, rw: COM01  _|
'    //           |+------- 6, rw, WGM00:    - Waveform Generation Mode
'    //           +-------- 7, w, FOC0:      - Force Output Compare
'    // ����������:

'    // ������������� �������� ��� �������� ��������
'    TCNT0 = 0xFF - F_CPU / 64000UL;

'    // Timer/Counter0 Output Compare Register
'    OCR0 = 0x00;

'    // Timer/Counter0 Asynchronous Status Register
'    // [ ������� ... ][ATmega16]
'    //          00000000 - Initial Value
'    //ASSR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, r, TCR0UB: - Timer/Counter Control Register0 Update Busy
'    //          ||||||+-- 1, r, OCR0UB: - Output Compare Register0 Update Busy
'    //          |||||+--- 2, r, TCN0UB: - Timer/Counter0 Update Busy
'    //          ||||+---- 3, rw, AS0:   - Asynchronous Timer/Counter0
'    //          |||+----- 4, r, -:
'    //          ||+------ 5, r, -:
'    //          |+------- 6, r, -:
'    //          +-------- 7, r, -:
'    // ����������:

End Sub


'/**
' * ��������� �������/�������� 1
' */
Sub MCU_Timer1Init


'    // [ATmega16] Table 62. Clock Select Bit Description
'    // +----+----+----+-----------------------------------------------------------------+
'    // |CSn2|CSn1|CSn0| Description                                                     �
'    // +----+----+----+-----------------------------------------------------------------+
'    // | 0  | 0  | 0  | No clock source. (������/������� ����������)                    |
'    // | 0  | 0  | 1  | clkI/O/1    (No prescaling)                                     |
'    // | 0  | 1  | 0  | clkI/O/8    (From prescaler)                                    |
'    // | 0  | 1  | 1  | clkI/O/64   (From prescaler)                                    |
'    // | 1  | 0  | 0  | clkI/O/256  (From prescaler)                                    |
'    // | 1  | 0  | 1  | clkI/O/1024 (From prescaler)                                    |
'    // | 1  | 1  | 0  | ����� �n, ���� �������������� �� ���������� ������ ���������    |
'    // | 1  | 1  | 1  | ����� �n, ���� �������������� �� ������������ ������ ���������  |
'    // +----+----+----+-----------------------------------------------------------------+

'    // Timer/Counter1 Control Register B
'    // [ ������� ���������� B ��������/��������� 1 ][ATmega16]
'    //            00000000 - Initial Value
'    TCCR1B = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //            ||||||||
'    //            |||||||+- 0, rw, CS10:  -+
'    //            ||||||+-- 1, rw, CS11:   | - ���������� �������� ��������
'    //            |||||+--- 2, rw, CS12:  _|
'    //            ||||+---- 3, rw, WGM12: -+ - ����� ������ �������/��������
'    //            |||+----- 4, rw, WGM13: _|
'    //            ||+------ 5, r: 0          - ��������������, ������ ���� ���������� � 0 ��� ������
'    //            |+------- 6, rw, ICES1:    - ����� ��������� ������ ������� �������
'    //            +-------- 7, rw, ICNC1:    - ���������� ������ ���������� ����� ����� �������
'    // ����������:

'    // ������������� �������� ��� ������� ���������
'    TCNT1H = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue ) >> 8
'    TCNT1L = 0x00; // ( 0xFFFF - Delay * F_CPU / PrescaleValue )
'    /*
'    OCR1AH = 0x00;
'    OCR1AL = 0x39;

'    OCR1BH = 0x00;
'    OCR1BL = 0x39;

'    OCR1CH = 0x00;
'    OCR1CL = 0x39;

'    ICR1H  = 0x00;
'    ICR1L  = 0x39;
'    */
'    // Timer/Counter1 Control Register A
'    // [ ������� ���������� A ��������/��������� 1 ][ATmega16]
'    //            00000000 - Initial Value
'    TCCR1A = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //            ||||||||
'    //            |||||||+- 0, rw, WGM10:  -+ - ����� ������ �������/��������
'    //            ||||||+-- 1, rw, WGM11:  _|
'    //            |||||+--- 2, rw, COM1C0: -+ - ����� ������ ������ ��������� C
'    //            ||||+---- 3, rw, COM1C1: _|
'    //            |||+----- 4, rw, COM1B0: -+ - ����� ������ ������ ��������� B
'    //            ||+------ 5, rw, COM1B1: _|
'    //            |+------- 6, rw, COM1A0: -+ - ����� ������ ������ ��������� A
'    //            +-------- 7, rw, COM1A1: _|
'    // ����������: ���������� ����� ������ ...

'    TCCR1B = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //            ||||||||
'    //            |||||||+- 0, rw, CS10:  -+
'    //            ||||||+-- 1, rw, CS11:   | - ���������� �������� ��������
'    //            |||||+--- 2, rw, CS12:  _|
'    //            ||||+---- 3, rw, WGM12: -+ - ����� ������ �������/��������
'    //            |||+----- 4, rw, WGM13: _|
'    //            ||+------ 5, r: 0          - ��������������, ������ ���� ���������� � 0 ��� ������
'    //            |+------- 6, rw, ICES1:    - ����� ��������� ������ ������� �������
'    //            +-------- 7, rw, ICNC1:    - ���������� ������ ���������� ����� ����� �������
'    // ����������: ����������� ������� �������� ������� ���������� ������ ...
'    // ����� ������: ...

End Sub


'/**
' * ��������� �������/�������� 2
' */
Sub MCU_Timer2Init

'    // Timer/Counter2 Control Register
'    // [ ������� ���������� ��������/��������� 2 ][ATmega16]
'    //           00000000 - Initial Value
'    TCCR2 = BIN8(00000100); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, CS20:  -+ - Clock Select
'    //           ||||||+-- 1, rw, CS21:   |
'    //           |||||+--- 2, rw, CS22:  _|
'    //           ||||+---- 3, rw, WGM21:    - Waveform Generation Mode
'    //           |||+----- 4, rw, COM20: -+ - Compare Match Output Mode
'    //           ||+------ 5, rw, COM21: _|
'    //           |+------- 6, rw, WGM20:    - Waveform Generation Mode
'    //           +-------- 7, w,  FOC2:     - Force Output Compare
'    // ����������:

'    // ������������� �������� ��� �������� ��������
'    TCNT2 =  0xFF - F_CPU / 64000UL;

'    // Timer/Counter2 Output Compare Register
'    OCR2 = 0x00;

'    // Timer/Counter2 Asynchronous Status Register
'    // [ ������� ... ][ATmega16]
'    //          00000000 - Initial Value
'    //ASSR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, r, TCR2UB: - Timer/Counter Control Register2 Update Busy
'    //          ||||||+-- 1, r, OCR2UB: - Output Compare Register2 Update Busy
'    //          |||||+--- 2, r, TCN2UB: - Timer/Counter2 Update Busy
'    //          ||||+---- 3, rw, AS2:   - Asynchronous Timer/Counter2
'    //          |||+----- 4, r: 0
'    //          ||+------ 5, r: 0
'    //          |+------- 6, r: 0
'    //          +-------- 7, r: 0
'    // ����������:

End Sub


'/**
' * ��������� ����������������� ���������� SPI
' */
Sub MCU_SPIInit

'    // SPI Control Register
'    // [ ������� ���������� SPI ][ATmega16]
'    //          00000000 - Initial Value
'    SPCR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, rw, SPR0: -+ - �������� ��������
'    //          ||||||+-- 1, rw, SPR1: _|
'    //          |||||+--- 2, rw, CPHA:    - ���� ��������� �������
'    //          ||||+---- 3, rw, CPOL:    - ���������� ��������� �������
'    //          |||+----- 4, rw, MSTR:    - ����� ������ ������ (Master/Slave)
'    //          ||+------ 5, rw, DORD:    - ������� �������� ������
'    //          |+------- 6, rw, SPE:     - ���������/���������� SPI
'    //          +-------- 7, rw, SPIE:    - ���������� ���������� �� SPI
'    // ����������:

'    // SPI Status Register
'    // [ ������� ������� SPI ][ATmega16]
'    //          00000000 - Initial Value
'    SPSR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, rw, SPI2X:    - Double SPI Speed Bit
'    //          ||||||+-- 1, r, 0       -+
'    //          |||||+--- 2, r, 0        |
'    //          ||||+---- 3, r, 0        | - ���������������
'    //          |||+----- 4, r, 0        |
'    //          ||+------ 5, r, 0       _|
'    //          |+------- 6, r, WCOL:      - Write COLlision flag
'    //          +-------- 7, r, SPIF:      - SPI Interrupt Flag
'    // ����������:

End Sub


'/**
' * ��������� ����������������� ���������� TWI
' */
Sub MCU_TWIInit


'    // TWI Bit Rate Register
'    TWBR = 0x00; // TODO: �������� ������� ��� ��������� ��������

'    // TWI Control Register
'    // [ ������� ���������� TWI ][ATmega16]
'    //          00000000 - Initial Value
'    TWCR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, rw, TWIE:  - TWI Interrupt Enable
'    //          ||||||+-- 1, r:         - reserved (will always read as zero)
'    //          |||||+--- 2, rw, TWEN:  - TWI Enable Bit
'    //          ||||+---- 3, r,  TWWC:  - TWI Write Collision Flag
'    //          |||+----- 4, rw, TWSTO: - TWI STOP Condition Bit
'    //          ||+------ 5, rw, TWSTA: - TWI START Condition Bit
'    //          |+------- 6, rw, TWEA:  - TWI Enable Acknowledge Bit
'    //          +-------- 7, rw, TWINT: - TWI Interrupt Flag
'    // ����������:

'    // TWI Status Register
'    // [ TWI ������ ������� ][ATmega16]
'    //          11111000 - Initial Value
'    TWSR = BIN8(11111000); // BIN8() �� ������� �� ������ �����������
'    //          ||||||||
'    //          |||||||+- 0, rw, TWPS0: -+ - TWI Prescaler Bits
'    //          ||||||+-- 1, rw, TWPS1: _|
'    //          |||||+--- 2, r:            - reserved (will always read as zero)
'    //          ||||+---- 3, r, TWS3:   -+
'    //          |||+----- 4, r, TWS4:    |
'    //          ||+------ 5, r, TWS5:    | - TWI Status
'    //          |+------- 6, r, TWS6:    |
'    //          +-------- 7, r, TWS7:   _|
'    // ����������:

'    // TWI (Slave) Address Register
'    // ����� � ����, ��� ������� �������� ��� TWGCE (TWI General Call Recognition Enable Bit)
'    TWAR = 0xFE;

End Sub


'/**
' * ��������� ����������� USART
' */
Sub MCU_USARTInit

'    // USART Control and Status Register A
'    // [ ������� ���������� UCSRA ][ATmega16]
'    //           00100000 - Initial Value
'    UCSRA = BIN8(00100000); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, MPCM: - Multi-processor Communication Mode
'    //           ||||||+-- 1, rw, U2X:  - Double the USART Transmission Speed
'    //           |||||+--- 2, r, PE:    - Parity Error
'    //           ||||+---- 3, r, DOR:   - Data OverRun
'    //           |||+----- 4, r, FE:    - Frame Error
'    //           ||+------ 5, r, UDRE:  - USART Data Register Empty
'    //           |+------- 6, rw, TXC:  - USART Transmit Complete
'    //           +-------- 7, r, RXC:   - USART Receive Complete
'    // ����������:

'    UCSRB = 0x00; // ���������, ���� ����������� ��������

'    // ����������� BAUD ��. � �����: "Configuration.h"
'    UBRRL = ( uint8_t ) ( F_CPU / ( 16UL * BAUD ) - 1UL ); // ������������� ��������
'    UBRRH = ( uint8_t ) ( ( F_CPU / ( 16UL * BAUD ) - 1UL ) >> 8 );

'    // USART Control and Status Register B
'    // [ ������� ���������� UCSRB ][ATmega16]
'    //           00000000 - Initial Value
'    UCSRB = BIN8(11011000); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, TXB8:  - Transmit Data Bit 8
'    //           ||||||+-- 1, r,  RXB8:  - Receive Data Bit 8
'    //           |||||+--- 2, rw, UCSZ2: - Character Size
'    //           ||||+---- 3, rw, TXEN:  - Transmitter Enable
'    //           |||+----- 4, rw, RXEN:  - Receiver Enable
'    //           ||+------ 5, rw, UDRIE: - USART Data Register Empty Interrupt Enable
'    //           |+------- 6, rw, TXCIE: - TX Complete Interrupt Enable
'    //           +-------- 7, rw, RXCIE: - RX Complete Interrupt Enable
'    // ����������:

'    // USART Control and Status Register C
'    // [ ������� ���������� UCSRC ][ATmega16]
'    //           10000110 - Initial Value
'    UCSRC = BIN8(10000110); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, UCPOL:    - Clock Polarity
'    //           ||||||+-- 1, rw, UCSZ0: -+ - Character Size
'    //           |||||+--- 2, rw, UCSZ1: _|
'    //           ||||+---- 3, rw, USBS:     - Stop Bit Select
'    //           |||+----- 4, rw, UPM0:  -+ - Parity Mode
'    //           ||+------ 5, rw, UPM1:  _|
'    //           |+------- 6, rw, UMSEL:    - USART Mode Select
'    //           +-------- 7, rw: URSEL     - Register Select
'    // ����������:

End Sub


'/**
' * ��������� ����������� ����������� �������
' */
Sub MCU_InternalWDTInit

'    // [ATmega16] Table 22. Watchdog Timer Prescale Select
'    // +----+----+----+------------------+---------+---------+
'    // |WDP2|WDP1|WDP0|OscCycles         | VCC3.0V | VCC5.0V |
'    // +----+----+----+------------------+---------+---------+
'    // | 0  | 0  | 0  |16K (16,384)      | 14.8 ms | 14.0 ms |
'    // | 0  | 0  | 1  |32K (32,768)      | 29.6 ms | 28.1 ms |
'    // | 0  | 1  | 0  |64K (65,536)      | 59.1 ms | 56.2 ms |
'    // | 0  | 1  | 1  |128K (131,072)    | 0.12 s  | 0.11 s  |
'    // | 1  | 0  | 0  |256K (262,144)    | 0.24 s  | 0.22 s  |
'    // | 1  | 0  | 1  |512K (524,288)    | 0.47 s  | 0.45 s  |
'    // | 1  | 1  | 0  |1,024K (1,048,576)| 0.95 s  | 0.9 s   |
'    // | 1  | 1  | 1  |2,048K (2,097,152)| 1.9 s   | 1.8 s   |
'    // +----+----+----+------------------+---------+---------+

'    // Watchdog Timer ControlRegister
'    // [ ������� ���������� ����������� ������� ][ATmega16]
'    //           00000000 - Initial Value
'    WDTCR = BIN8(00000000); // BIN8() �� ������� �� ������ �����������
'    //           ||||||||
'    //           |||||||+- 0, rw, WDP0: -+
'    //           ||||||+-- 1, rw, WDP1:  |- ����������� ������� ������������ ����������� �������
'    //           |||||+--- 2, rw, WDP2: _|
'    //           ||||+---- 3, rw, WDE:  ���������� ������ �� ����-���� ����������� �������
'    //           |||+----- 4, rw, WDCE: ���������� ��������� ������������ ����������� �������
'    //           ||+------ 5, r, -+
'    //           |+------- 6, r,  |- Reserved Bits
'    //           +-------- 7, r, _|
'    // ����������:
'    // ������� ���������� ������. ����� � ���� ��� ���������� ����������
'    // ����� ���������� ������� ������� ������� ��� ���������� � ������
'    // ������������� �������.


End Sub


'/**
' * ��������� ������ � �������� ������������
' */
Sub MCU_ExternalInterruptsInit

'    uint8_t temp;

'    // Table 48. Interrupt Sense Control
'    //+-----+------+----------+--------+---------+--------------------------------------------+
'    //|ISCn1|ISCn0 | Description                                                              |
'    //+-----+------+----------+--------+---------+--------------------------------------------+
'    //|  0  |  0   | The low level of INTn generates an interrupt request.                    |
'    //|  0  |  1   | Reserved                                                                 |
'    //|  1  |  0   | The falling edge of INTn generates asynchronously an interrupt request.  |
'    //|  1  |  1   | The rising edge of INTn generates asynchronously an interrupt request.   |
'    //+-----+------+----------+--------+---------+--------------------------------------------+

'    // ��������� ������� ����������
'    GICR &= ~( ( 1 << INT0 ) | ( 1 << INT1 ) | ( 1 << INT2 ) );

'    // ��������� ���������� INT0 � INT1
'    // MCU Control Register
'    // [ ������� ���������� ����������������� ]
'    //          ****0000 - Initial Value
'    temp = BIN8(00000000);
'    //          ||||||||
'    //          |||||||+- 0, rw, ISC00: -+ - Interrupt Sense Control 0 Bit 1 and Bit 0
'    //          ||||||+-- 1, rw, ISC01: _|
'    //          |||||+--- 2, rw, ISC10: -+ - Interrupt Sense Control 1 Bit 1 and Bit 0
'    //          ||||+---- 3, rw, ISC11: _|
'    //          |||+----- 4, rw, SM0:      -
'    //          ||+------ 5, rw, SM1:      -
'    //          |+------- 6, rw, SE:       -
'    //          +-------- 7, rw, SM2:      -
'    // ����������:
'    // ��� ����������� ��������� ������� ������� �������� MCUCR �� ����������
'    // ���������
'    temp &= ( 1 << ISC11 ) | ( 1 << ISC01 );
'    MCUCR &= ~( ( 1 << ISC11 ) | ( 1 << ISC10 ) | ( 1 << ISC01 ) | ( 1 << ISC00 ) );
'    MCUCR |= temp;

'    // ��������� ���������� INT2
'    // If ISC2 is written to zero, a falling edge on INT2 activates the
'    // interrupt. If ISC2 is written to one, a rising edge on INT2
'    // activates the interrupt.

'    // MCU Control and Status Register
'    // [ ������� ���������� ����������������� ]
'    //          *0****** - Initial Value
'    temp = BIN8(00000000);
'    //          ||||||||
'    //          |||||||+- 0, rw, PORF:  -
'    //          ||||||+-- 1, rw, EXTRF: -
'    //          |||||+--- 2, rw, BORF:  -
'    //          ||||+---- 3, rw, WDRF:  -
'    //          |||+----- 4, rw, JTRF:  -
'    //          ||+------ 5, r: 0
'    //          |+------- 6, rw, ISC2:  - Interrupt Sense Control 2
'    //          +-------- 7, rw, JTD:   -
'    // ����������:
'    // ��� ����������� ��������� ������ ������� �������� MCUCSR �� ����������
'    // ���������
'    //temp &= ( 1 << ISC2 );
'    MCUCSR &= ~( 1 << ISC2 );
'    MCUCSR |= temp;

End Sub