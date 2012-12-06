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

    #define F_CPU 16000000UL

#endif

// ������������ ��� ���������� ������������
#ifndef __DEBUG__
    // ��������������� ��� ������ ��� ���������� ������
    //#define __DEBUG__
#endif

// �������� ������ ����������� USART
//#define BAUD 9600UL
//#define BAUD 19200UL
//#define BAUD 38400UL
#define BAUD 57600UL
//#define BAUD 115200UL

#define GLASS_OFFSET_LEFT   15 
#define GLASS_OFFSET_TOP    4
#define GLASS_WIDTH     24
#define GLASS_HEIGHT    17


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

//#define ADC_Conversion_Complete

//#define EEPROM_Ready

//#define Analog_Comparator

//#define TWI_Serial_Interface

//#define Store_Program_Memory_Read

#endif // _CONFIGURATION_H_
