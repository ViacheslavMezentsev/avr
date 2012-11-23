#ifndef DefinesH
#define DefinesH

// Внимание:
// Для VS2008 нужно сделать явное определение, чтобы работала
// автоматическая подсветка логики препроцессора в IDE.
// Это делается в настройках проекта:
// IntelliSence\Preprocessor Definitions: __GNUC__ или __ICCAVR__

#ifdef __GNUC__

    // Стандартные библиотеки
    #include <inttypes.h>
    #include <stdint.h>

    #include <stdlib.h>
    #include <string.h>

    #include <stdio.h>

    #include <math.h>
    #include <util/delay.h>

    #include <compat/ina90.h>
    #include <compat/deprecated.h>

    #include <avr/pgmspace.h>
    #include <avr/interrupt.h>
    #include <avr/io.h>
    #include <avr/iom16.h>

    // Набор шаблонов для "типизации" указателей в AVR GCC
    #ifdef __cplusplus
        #include "SmartPtr.h"
    #endif

    #define nop() asm volatile ("nop")
    #define sleep() asm volatile ("sleep")

    #define __disable_interrupt() cli()
    #define __enable_interrupt() sei()
    #define __save_interrupt() SREG
    #define __restore_interrupt(x) SREG = x
    #define __delay_cycles(x) _delay_loop_2(x)

    #define FLASH_DECLARE(x) PROGMEM x

    #define FCHAR_PTR FlashPtr< char >
    #define FUCHAR_PTR FlashPtr< unsigned char >

    #define FU08T_PTR FlashPtr< uint8_t >
    #define FS08T_PTR FlashPtr< int8_t >

    #define FU16T_PTR FlashPtr< uint16_t >
    #define FS16T_PTR FlashPtr< int16_t >

    #define FU32T_PTR FlashPtr< uint32_t >
    #define FS32T_PTR FlashPtr< int32_t >

    #define FLASHSTR_DECLARE(type,name,init) \
        static PROGMEM type _##name[] = init; \
        FlashPtr<type> name(_##name);

#elif defined( __ICCAVR__ )

    #pragma hdrstop

    // Стандартные библиотеки

    #include <intrinsics.h>
    #include <stdint.h>
    #include <pgmspace.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include <iom16.h>
    #include <ina90.h>

    // TODO: Подумать об аналогах этих задержек (как в AVR GCC)
    // Динамический вызов тоже может пригодиться.
    // Внимание: Указыаейте округлённые до целых значения,
    // т.к. дробная часть в отличие от GCC будет отбрасываться.
    // Это надо как-то пофиксить для кросс-компиляторности.
    #define _delay_us(us) __delay_cycles( ( uint32_t ) (us) * ( F_CPU / 1000000UL ) )
    #define _delay_ms(ms) __delay_cycles( ( uint32_t ) (ms) * ( F_CPU / 1000UL ) )

    #define _BV(x) ( 1 << x )

    #define cbi(port,x) port &= ~( 1 << x )
    #define sbi(port,x) port |= ( 1 << x )

    #define outb(port,byte) port = byte
    #define inp(port) (port)

    #define utoa(num,buf,rad) sprintf(buf, "%i", num)
    #define nop() __no_operation()
    #define sleep() __sleep()

    #define pgm_read_byte(addr) *( (__flash unsigned char * ) ( addr ) )
    #define pgm_read_word(addr) *( (__flash unsigned short * ) ( addr ) )

    // Спецификаторы расположения в памяти Internal Flash

    // The extended keywords that can be used for data control the following:

    // - For data memory space, keywords that control the placement and type of objects
    //  and pointers: __tiny, __near, __far, __huge, and __regvar

    // - For the EEPROM memory space, keyword that controls the placement and type of
    //  objects and pointers: __eeprom

    // - For the code (flash) memory space, keywords that control the placement and type of
    //  objects and pointers: __tinyflash, __flash, __farflash, and __hugeflash

    // - For the I/O memory space, keyword that controls the placement and type of objects
    //  and pointers: __ext_io, __io

    // - Special pointer that can access data objects in both data and code memory space:
    //  __generic

    // - Other characteristics of objects: __root and __no_init.

    #define FLASHSTR_DECLARE( type, name, init ) \
        static __flash type name[] = init;

    #define FLASHARR_DECLARE( type, name, size, init ) \
        __flash type name[size] = init;

    #define FLASH_DECLARE(x) __flash x

    #define FCHAR_PTR char __flash *
    #define FUCHAR_PTR unsigned char __flash *

    #define FU08T_PTR uint8_t __flash *
    #define FS08T_PTR int8_t __flash *

    #define FU16T_PTR uint16_t __flash *
    #define FS16T_PTR int16_t __flash *

    #define FU32T_PTR uint32_t __flash *
    #define FS32T_PTR int32_t __flash *

    #define PROGMEM __flash
    #define prog_char char __flash
    #define PGM_P prog_char *
    #define PSTR(x) ( PGM_P ) x

#endif

/***********************************************************************
** MACROS:      PR_BEGIN_EXTERN_C
**              PR_END_EXTERN_C
** DESCRIPTION:
**      Macro shorthands for conditional C++ extern block delimiters.
***********************************************************************/
#ifdef __cplusplus
    #define PR_BEGIN_EXTERN_C       extern "C" {
    #define PR_END_EXTERN_C         }
#else
    #define PR_BEGIN_EXTERN_C
    #define PR_END_EXTERN_C
#endif

// Описание типов, аналогичных в Windows
#include "windows.h"

#include "fifo.h"

// Petit FAT File System Module
#include "diskio.h"
#include "pff.h"

#define TOGGLE(x,y) ((x) ^= (1<<(y)))
#define CHECKBIT(x,y) ((x) & (1<<(y)))

// Подключаем описание идентификаторов ресурсов
//#include "resource.h"

#ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

// url: http://www.piclist.com/techref/microchip/math/bit/parity.htm
#define PARITY(b) ((((((b)^(((b)<<4)|((b)>>4))) + 0x41) | 0x7C ) +2 ) & 0x80)

// Для самомодификации кода
#define JOIN(x,y) JOIN2(x,y)
#define JOIN2(x,y) x##y

// Пример: 'SET_PORT_PIN_NUMBER( E, 2 )' -> 'PE2' (эквивалентно)
#define SET_PORT_PIN_LETTER(x) JOIN(P, x)
#define SET_PORT_PIN_NUMBER(Letter, Pin) JOIN(SET_PORT_PIN_LETTER(Letter), Pin)

#define SET_DDR_NAME(x) JOIN(DDR, x)
#define SET_PORT_NAME(x) JOIN(PORT, x)
#define SET_PIN_NAME(x) JOIN(PIN, x)

// Convert Decimal to Binary Coded Decimal (BCD)
#define dec2bcd(num) ( ( num / 10 * 16) + ( num % 10 ) )

#define DecBinToBin( Value ) ( ( ( Value & 0xF0 ) >> 4 ) * 10 + ( Value & 0x0F ) )
#define BinToDecBin( Value ) ( ( ( Value / 10 ) << 4 ) | ( Value % 10 ) )

// Макросы для вставки двоичных констант
// Источник: RSDN, Как вставлять в программу на C++ двоичные константы?
// Автор: Александр Шаргин (mailto:rudankort@rsdn.ru)
// Опубликовано: 01.12.2001

// BIN8() не зависит от уровня оптимизации

// Вариант, предложенный Игорем Ширкалиным
// uni: avrgcc формирует warning на использование |
//#define BIN__N(x) (x) | x>>3 | x>>6 | x>>9
//#define BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
//#define BIN8(v) (BIN__B(BIN__N(0x##v)))

#define BIN8(x) BIN___(0##x)
#define BIN___(x)                                    \
	(                                                \
	((x / 01ul) % 010)*(2>>1) +                      \
	((x / 010ul) % 010)*(2<<0) +                     \
	((x / 0100ul) % 010)*(2<<1) +                    \
	((x / 01000ul) % 010)*(2<<2) +                   \
	((x / 010000ul) % 010)*(2<<3) +                  \
	((x / 0100000ul) % 010)*(2<<4) +                 \
	((x / 01000000ul) % 010)*(2<<5) +                \
	((x / 010000000ul) % 010)*(2<<6)                 \
	)

// Стандартные целые
//#include <stdint.h>

#define BIN16(x1,x2) \
    ((BIN(x1)<<8)+BIN(x2))

#define BIN24(x1,x2,x3) \
    ((BIN(x1)<<16)+(BIN(x2)<<8)+BIN(x3))

#define BIN32(x1,x2,x3,x4) \
    ((BIN(x1)<<24)+(BIN(x2)<<16)+(BIN(x3)<<8)+BIN(x4))

#define LongToBin(n) (((n >> 21) & 0x80) | \
                      ((n >> 18) & 0x40) | \
                      ((n >> 15) & 0x20) | \
                      ((n >> 12) & 0x10) | \
                      ((n >>  9) & 0x08) | \
                      ((n >>  6) & 0x04) | \
                      ((n >>  3) & 0x02) | \
                      ((n      ) & 0x01))


#endif
