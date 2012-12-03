#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "Defines.h"

// �������� ������� �������� ��������� ����������
#ifdef __GNUC__

    // ������� ������� � makefile ����������
    // -DF_CPU=8000000UL
    #ifndef F_CPU
        #error "F_CPU undefined!"
    #endif

#elif defined( __ICCAVR__ )

    #define F_CPU 4000000UL

#endif

// ������������ ��� ���������� ������������
#ifndef __DEBUG__
    // ��������������� ��� ������ ��� ���������� ������
    //#define __DEBUG__
#endif

// �������� ������ ����������� USART
#define BAUD 9600UL
//#define BAUD 19200UL
//#define BAUD 38400UL
//#define BAUD 57600UL


// ������������ LCD �������

// ����� ������� �����
#define LCD_LINES   2
// ����� ������ (� ��������)
#define LCD_LINE_LENGTH 16

// ������ ����� � DDRAM
#define LCD_LINE0_DDRAMADDR 0x00
#define LCD_LINE1_DDRAMADDR 0x40
#define LCD_LINE2_DDRAMADDR 0x00
#define LCD_LINE3_DDRAMADDR 0x40

// �������� ������� ��� ������ � ���
// ����� DDR � PORT ��� ���������� LCD_E � LCD_RS

// �����: Enable Signal
#define LCD_DDR_NAME_En     SET_DDR_NAME( B )   // -> DDRB
#define LCD_PORT_NAME_En    SET_PORT_NAME( B )  // -> PORTB
#define LCD_PIN_NAME_En     SET_PIN_NAME( B )   // -> PINB
#define LCD_PIN_En          SET_PORT_PIN_NUMBER( B, 2 ) // -> PB2

// �����: Data/Instruction Select
#define LCD_DDR_NAME_RS     SET_DDR_NAME( B )   // -> DDRB
#define LCD_PORT_NAME_RS    SET_PORT_NAME( B )  // -> PORTB
#define LCD_PIN_NAME_RS     SET_PIN_NAME( B )   // -> PINB
#define LCD_PIN_RS          SET_PORT_PIN_NUMBER( B, 0 ) // -> PB0

// �����: Read/Write Select
//#define LCD_RW_UNUSED

#ifndef LCD_RW_UNUSED
    #define LCD_DDR_NAME_RW     SET_DDR_NAME( B )   // -> DDRB
    #define LCD_PORT_NAME_RW    SET_PORT_NAME( B )  // -> PORTB
    #define LCD_PIN_NAME_RW     SET_PIN_NAME( B )   // -> PINB
    #define LCD_PIN_RW          SET_PORT_PIN_NUMBER( B, 1 ) // -> PB1
#endif

// ���������� ����� � ���� ��� ������ � ���
#define LCD_DATA_LENGTH_4BIT
//#define LCD_DATA_LENGTH_8BIT

#ifdef LCD_DATA_LENGTH_4BIT

    #define LCD_DDR_NAME_D4     SET_DDR_NAME( B )       // -> DDRB
    #define LCD_PORT_NAME_D4    SET_PORT_NAME( B )      // -> PORTB
    #define LCD_PIN_NAME_D4     SET_PIN_NAME( B )       // -> PINB
    #define LCD_PIN_D4          SET_PORT_PIN_NUMBER( B, 4 ) // -> PB4

    #define LCD_DDR_NAME_D5     SET_DDR_NAME( B )       // -> DDRB
    #define LCD_PORT_NAME_D5    SET_PORT_NAME( B )      // -> PORTB
    #define LCD_PIN_NAME_D5     SET_PIN_NAME( B )       // -> PINB
    #define LCD_PIN_D5          SET_PORT_PIN_NUMBER( B, 5 ) // -> PB5

    #define LCD_DDR_NAME_D6     SET_DDR_NAME( B )       // -> DDRB
    #define LCD_PORT_NAME_D6    SET_PORT_NAME( B )      // -> PORTB
    #define LCD_PIN_NAME_D6     SET_PIN_NAME( B )       // -> PINB
    #define LCD_PIN_D6          SET_PORT_PIN_NUMBER( B, 6 ) // -> PB6

    #define LCD_DDR_NAME_D7     SET_DDR_NAME( B )       // -> DDRB
    #define LCD_PORT_NAME_D7    SET_PORT_NAME( B )      // -> PORTB
    #define LCD_PIN_NAME_D7     SET_PIN_NAME( B )       // -> PINB
    #define LCD_PIN_D7          SET_PORT_PIN_NUMBER( B, 7 ) // -> PB7

#endif

#ifdef LCD_DATA_LENGTH_8BIT
    #define LCD_DDR_D0D7 SET_DDR_NAME( F )   // -> DDRF
    #define LCD_PORT_D0D7 SET_PORT_NAME( F )   // -> PORTF
    #define LCD_PIN_D0D7 PINF
#endif

// �������� �������� ���������� �������� ��������� ���
#define LCD_DELAYS_DATA_US 50
#define LCD_DELAYS_COMMAND_US 50
#define LCD_DELAYS_CLEAR_MS 2.0


/**
 * ������� �������� ���������� [ATmega16]
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ���������/��������� ��������� ����������.
 *
 * ��� ����������� ������ �� ���������/���������� ����
 * ������������ � ������ ISRList.cpp. ���� ������������ ���������
 * ���������� ����������, ������� �� �������� � ISRList.cpp, ��
 * ��������������� ����� ����� ������ ���� ����������������, �����
 * �������� ��������� ��� ����������� ������ ������� ����������
 * ������������ � ���������� ������ �������.
 */

//#define External_Interrupt_Request_0
//#define External_Interrupt_Request_1
//#define External_Interrupt_Request_2

#define Timer_Counter0_Overflow
//#define Timer_Counter0_Compare_Match

//#define Timer_Counter1_Overflow
//#define Timer_Counter1_Capture_Event
//#define Timer_Counter1_Compare_Match_A
//#define Timer_Counter1_Compare_Match_B
//#define Timer_Counter1_Compare_Match_C

//#define Timer_Counter2_Overflow
//#define Timer_Counter2_Compare_Match

#define USART_Rx_Complete
//#define USART_Tx_Complete
//#define USART_Data_Register_Empty

//#define SPI_Serial_Transfer_Complete

#define ADC_Conversion_Complete

//#define EEPROM_Ready

//#define Analog_Comparator

//#define TWI_Serial_Interface

//#define Store_Program_Memory_Read

#endif // _CONFIGURATION_H_
