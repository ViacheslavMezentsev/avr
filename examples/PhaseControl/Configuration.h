#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

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

// Скорость работы внутреннего USART
#define BAUD 9600UL
//#define BAUD 19200UL
//#define BAUD 38400UL
//#define BAUD 57600UL



/**
 * Таблица векторов прерываний [ATtiny2313]
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Разрешаем/запрещаем обработку прерываний.
 *
 * Эти определения влияют на включение/выключение кода
 * обработчиков в модуле MCU.cpp. Если используется сторонний
 * обработчик прерывания, который не определён в MCU.cpp, то
 * соответствующие флаги здесь должны быть закомментированы, чтобы
 * избежать конфликта при определении одного вектора прерывания
 * одновременно в нескольких местах проекта.
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
