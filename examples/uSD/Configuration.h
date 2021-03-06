#pragma once

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

#define ID_TIMER_10MS   0x01
#define ID_TIMER_100MS  0x02
#define ID_TIMER_500MS  0x03
#define ID_TIMER_1S     0x04
#define ID_TIMER_5S     0x05

#define HWND_COMMAND_SHELL 100
#define HWND_FILE_MANAGER 101
#define HWND_VIEWER 102
#define HWND_PLAYER 103

// ������ ������� ��������� � ���
// ���� ������� �������� � ��� sizeof( MSG ) = 20 ����
#define MESSAGE_QUEUE_LENGTH 8

// �������� ������ ����������� USART
//#define BAUD 9600UL
//#define BAUD 19200UL
#define BAUD 38400UL
//#define BAUD 57600UL

// Definitions for MMC/SDC connection
#define SD_DI   5   // MOSI
#define SD_DO   6   // MISO
#define SD_CLK  7   // CLK
#define SD_CS   4   // SS
#define SD_INS  0   // CD
#define SD_WP   1   // WP


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

//#define Timer_Counter0_Overflow
//#define Timer_Counter0_Compare_Match

//#define Timer_Counter1_Overflow
//#define Timer_Counter1_Capture_Event
//#define Timer_Counter1_Compare_Match_A
//#define Timer_Counter1_Compare_Match_B
//#define Timer_Counter1_Compare_Match_C

#define Timer_Counter2_Overflow
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

