#pragma once

// Status of Disk Functions
typedef BYTE DSTATUS;

// Results of Disk Functions
typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} DRESULT;

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

// allow write operations
#define _WRITE_FUNC	1			

// Definitions for MMC/SDC connection
//#define SD_DI   5   // MOSI
//#define SD_DO   6   // MISO
//#define SD_CLK  7   // CLK
//#define SD_CS   4   // SS
//#define SD_INS  0   // CD
//#define SD_WP   1   // WP

// Port Controls (Platform dependent)
#define SELECT()	PORTB &= ~_BV( SD_CS )		// MMC CS = L
#define	DESELECT()	PORTB |=  _BV( SD_CS )		// MMC CS = H
#define	MMC_SEL		!( PORTB & _BV( SD_CS ) )	// MMC CS status (true:selected)
#define	INIT_SPI()	{ PORTB=_BV(SD_CS)|_BV(SD_DO)|_BV(SD_DI)|_BV(SD_WP)|_BV(SD_INS); DDRB=_BV(SD_CS)|_BV(SD_DI)|_BV(SD_CLK); }	


/**
 * Класс CMMC
 * ~~~~~~~~~~
 *
 * Назначение:
 * Обёртка ....
 *
 * Автор: Мезенцев Вячеслав
 *
 * Почта: mailto:unihomelab@ya.ru
 *
 * ВНИМАНИЕ:
 * Этот файл формируется автоматически в среде EA.
 * Если Вы что-то изменили здесь, то синхронизируйте изменения в EA.
 */

class CMMC {

private:

    static BYTE CardType;

public:
    
    // Card type flags (CardType)
    enum EnCardType {

        CT_NONE = 0,    // Тип не определён
        CT_MMC = 1,	    // MMC ver 3
        CT_SD1 = 2,	    // SD ver 1
        CT_SD2 = 4,	    // SD ver 2
        CT_SDC = ( CT_SD1 | CT_SD2 ),	// SD
        CT_BLOCK = 8	// Block addressing
    };

    // Перечисление для команд MMC/SDC
    enum MMCCommand {

        CMD0    =	(0x40+0),   // GO_IDLE_STATE
        CMD1    =	(0x40+1),   // SEND_OP_COND (MMC)
	    ACMD41  =	(0xC0+41),  // SEND_OP_COND (SDC)
        CMD8    =	(0x40+8),   // SEND_IF_COND
        CMD16   =	(0x40+16),  // SET_BLOCKLEN
        CMD17   =	(0x40+17),  // READ_SINGLE_BLOCK
        CMD24   =	(0x40+24),  // WRITE_BLOCK
        CMD55   =	(0x40+55),  // APP_CMD
        CMD58   =	(0x40+58)   // READ_OCR
    };

    static DSTATUS Initialize();
    static DRESULT Read( BYTE * buff, DWORD lba, WORD ofs, WORD cnt );
    static DRESULT Write( const BYTE * buff, DWORD sa );
    static DRESULT Write( FCHAR_PTR buff, DWORD sa );

private:

    inline static void SPIRelease();
    static void SPIWrite( BYTE Data );
    static BYTE SPIRead();    
    static BYTE SendCommand( BYTE Command, DWORD Argument ); 

};

