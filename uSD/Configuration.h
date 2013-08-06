#pragma once

#include "Defines.h"

// Выбираем частоту внешнего тактового генератора
#ifdef __GNUC__

    // Частота задаётся в makefile директивой
    // -DF_CPU=8000000UL
    #ifndef F_CPU
        #error "F_CPU undefined!"
    #endif

#elif defined( __ICCAVR__ )

    #define F_CPU 8000000UL

#endif

// Используется для отладочной конфигурации
#ifndef __DEBUG__
    // Закомментируйте эту строку для компиляции релиза
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

// Размер очереди сообщений в ОЗУ
// Один элемент занимает в ОЗУ sizeof( MSG ) = 20 байт
#define MESSAGE_QUEUE_LENGTH 8

// Скорость работы внутреннего USART
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
 * Таблица векторов прерываний [ATmega16]
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Разрешаем/запрещаем обработку прерываний.
 *
 * Эти определения влияют на включение/выключение кода
 * обработчиков в модуле ISRList.cpp. Если используется сторонний
 * обработчик прерывания, который не определён в ISRList.cpp, то
 * соответствующие флаги здесь должны быть закомментированы, чтобы
 * избежать конфликта при определении одного вектора прерывания
 * одновременно в нескольких местах проекта.
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

