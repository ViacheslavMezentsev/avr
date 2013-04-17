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

    #define F_CPU 8000000UL

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



/**
 * ������� �������� ���������� [ATtiny2313]
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

//#define Timer_Counter1_Capture_Event
#define Timer_Counter1_Compare_Match_A
#define Timer_Counter1_Overflow

#define Timer_Counter0_Overflow

#define USART_Rx_Complete
//#define USART_Data_Register_Empty
//#define USART_Tx_Complete

#define Analog_Comparator

//#define Pin_Change_Interrupt

//#define Timer_Counter1_Compare_Match_B

//#define Timer_Counter0_Compare_Match_A
//#define Timer_Counter0_Compare_Match_B

//#define USI_Start_Condition
//#define USI_Overflow

//#define EEPROM_Ready

//#define Watchdog_Timer_Overflow

#endif // _CONFIGURATION_H_
