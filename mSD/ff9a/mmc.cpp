/*-----------------------------------------------------------------------*/
/* MMCv3/SDv1/SDv2 (in SPI mode) control module  (C)ChaN, 2010           */
/*-----------------------------------------------------------------------*/

#include "Defines.h"
#include "Configuration.h"


// Port controls (platform dependent)
// CS = low
#define CS_LOW()	( PORTB &= ~( 1 << SD_CS ) )

// CS = high
#define	CS_HIGH()	( PORTB |= ( 1 << SD_CS ) )

// Card detected: yes:true, no:false, default:true
#define SOCKINS		( !( PINB & ( 1 << SD_INS ) ) )	

// Write protected: yes:true, no:false, default:false
#define SOCKWP		( PINB & ( SD_WP ) )

// Set slow clock ( F_CPU / 64 )
#define	FCLK_SLOW()	SPCR = 0x52

// Set fast clock ( F_CPU / 2 )
#define	FCLK_FAST()	SPCR = 0x50		


/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* Definitions for MMC/SDC command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */


// Disk status
static volatile DSTATUS Stat = STA_NOINIT;

// 100Hz decrement timer
static volatile BYTE Timer1, Timer2;

// Card type flags
static BYTE CardType;


/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */

/* Socket power state: 0=off, 1=on */
static int power_status() {
	
    //return (PORTE & _BV(7)) ? 0 : 1;
    return 1;

}

static void power_on() {

	//{	/* Remove this block if no socket power control */
	//	PORTE &= ~_BV(7);	/* Socket power on (PE7=low) */
	//	DDRE |= _BV(7);
	//	for (Timer1 = 2; Timer1; );	/* Wait for 20ms */
	//}
    
    // Port B Data Direction Register
    // [ Регистр направления порта B ][ATmega32]
    //          00000000 - Initial Value
    DDRB = BIN8(10110000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, DDB0: (XCK/T0)    - CD
    //          ||||||+-- 1, rw, DDB1: (T1)        - WP
    //          |||||+--- 2, rw, DDB2: (INT2/AIN0) - 
    //          ||||+---- 3, rw, DDB3: (OC0/AIN1)  -
    //          |||+----- 4, rw, DDB4: (~SS)       - SD_CS
    //          ||+------ 5, rw, DDB5: (MOSI)      - SD_DI
    //          |+------- 6, rw, DDB6: (MISO)      - SD_DO
    //          +-------- 7, rw, DDB7: (SCK)       - SD_CLK
    // Примечание:


    // Port B Data Register
    // [ Регистр данных порта B ][ATmega32]
    //           00000000 - Initial Value
    PORTB = BIN8(11110011); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PORTB0: (XCK/T0)    - CD
    //           ||||||+-- 1, rw, PORTB1: (T1)        - WP
    //           |||||+--- 2, rw, PORTB2: (INT2/AIN0) - 
    //           ||||+---- 3, rw, PORTB3: (OC0/AIN1)  -
    //           |||+----- 4, rw, PORTB4: (~SS)       - SD_CS
    //           ||+------ 5, rw, PORTB5: (MOSI)      - SD_DI
    //           |+------- 6, rw, PORTB6: (MISO)      - SD_DO
    //           +-------- 7, rw, PORTB7: (SCK)       - SD_CLK
    // Примечание:


    // SPI Control Register
    // [ Регистр управления SPI ][ATmega32]
    //          00000000 - Initial Value
    SPCR = BIN8(01010010); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, SPR0: -+ - Скорость передачи
    //          ||||||+-- 1, rw, SPR1: _|
    //          |||||+--- 2, rw, CPHA:    - Фаза тактового сигнала
    //          ||||+---- 3, rw, CPOL:    - Полярность тактового сигнала
    //          |||+----- 4, rw, MSTR:    - Выбор режима работы (Master/Slave)
    //          ||+------ 5, rw, DORD:    - Порядок передачи данных
    //          |+------- 6, rw, SPE:     - Включение/выключение SPI
    //          +-------- 7, rw, SPIE:    - Разрешение прерывания от SPI
    // Примечание: Enable SPI function in mode 0

    // SPI Status Register
    // [ Регистр статуса SPI ][ATmega32]
    //          00000000 - Initial Value
    SPSR = BIN8(00000001); // BIN8() не зависит от уровня оптимизации
    //          ||||||||	
    //          76543210
    //          |||||||+- 0, rw, SPI2X:    - Double SPI Speed Bit
    //          ||||||+-- 1, r, 0       -+ - зарезервированы
    //          |||||+--- 2, r, 0        |
    //          ||||+---- 3, r, 0        | 
    //          |||+----- 4, r, 0        |
    //          ||+------ 5, r, 0       _|
    //          |+------- 6, r, WCOL:      - Write Collision flag
    //          +-------- 7, r, SPIF:      - SPI Interrupt Flag
    // Примечание: SPI 2x mode

}


static void power_off() {

    // SPI Control Register
    // [ Регистр управления SPI ][ATmega32]
    //          00000000 - Initial Value
    SPCR = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          76543210
    //          |||||||+- 0, rw, SPR0: -+ - Скорость передачи
    //          ||||||+-- 1, rw, SPR1: _|
    //          |||||+--- 2, rw, CPHA:    - Фаза тактового сигнала
    //          ||||+---- 3, rw, CPOL:    - Полярность тактового сигнала
    //          |||+----- 4, rw, MSTR:    - Выбор режима работы (Master/Slave)
    //          ||+------ 5, rw, DORD:    - Порядок передачи данных
    //          |+------- 6, rw, SPE:     - Включение/выключение SPI
    //          +-------- 7, rw, SPIE:    - Разрешение прерывания от SPI
    // Примечание: Disable SPI function			


    // Port B Data Direction Register
    // [ Регистр направления порта B ][ATmega32]
    //          00000000 - Initial Value
    DDRB = BIN8(00000000); // BIN8() не зависит от уровня оптимизации
    //          ||||||||
    //          |||||||+- 0, rw, DDB0: (XCK/T0)    - CD
    //          ||||||+-- 1, rw, DDB1: (T1)        - WP
    //          |||||+--- 2, rw, DDB2: (INT2/AIN0) - 
    //          ||||+---- 3, rw, DDB3: (OC0/AIN1)  -
    //          |||+----- 4, rw, DDB4: (~SS)       - SD_CS
    //          ||+------ 5, rw, DDB5: (MOSI)      - SD_DI
    //          |+------- 6, rw, DDB6: (MISO)      - SD_DO
    //          +-------- 7, rw, DDB7: (SCK)       - SD_CLK
    // Примечание:

	/* Set SCK/MOSI/CS as hi-z, INS#/WP as pull-up */
    // Port B Data Register
    // [ Регистр данных порта B ][ATmega32]
    //           00000000 - Initial Value
    PORTB = BIN8(00000011); // BIN8() не зависит от уровня оптимизации
    //           ||||||||
    //           |||||||+- 0, rw, PORTB0: (XCK/T0)    - CD
    //           ||||||+-- 1, rw, PORTB1: (T1)        - WP
    //           |||||+--- 2, rw, PORTB2: (INT2/AIN0) - 
    //           ||||+---- 3, rw, PORTB3: (OC0/AIN1)  -
    //           |||+----- 4, rw, PORTB4: (~SS)       - SD_CS
    //           ||+------ 5, rw, PORTB5: (MOSI)      - SD_DI
    //           |+------- 6, rw, PORTB6: (MISO)      - SD_DO
    //           +-------- 7, rw, PORTB7: (SCK)       - SD_CLK
    // Примечание:

	//{	/* Remove this block if no socket power control */
	//	PORTE |= _BV(7);		/* Socket power off (PE7=high) */
	//	for (Timer1 = 20; Timer1; );	/* Wait for 20ms */
	//}

}


/*-----------------------------------------------------------------------*/
/* Transmit/Receive data from/to MMC via SPI  (Platform dependent)       */
/*-----------------------------------------------------------------------*/

/* Exchange a byte */
/* Returns received data */
/* dat - Data to be sent */
static BYTE xchg_spi ( BYTE dat ) {

	SPDR = dat;

	loop_until_bit_is_set( SPSR, SPIF );
	
    return SPDR;

}


/* Send a data block */
/* Data block to be sent */
/* Size of data block */
static void xmit_spi_multi ( const BYTE * p, UINT cnt ) {

    do {

        SPDR = * p++; 
        loop_until_bit_is_set( SPSR, SPIF );
		
        SPDR = * p++; 
        loop_until_bit_is_set( SPSR, SPIF );

	} while ( cnt -= 2 );

}


/* Receive a data block */
/* Data buffer */
/* Size of data block */
static void rcvr_spi_multi( BYTE *p, UINT cnt ) {
	
    do {
	
        SPDR = 0xFF;
        loop_until_bit_is_set( SPSR, SPIF );
        * p++ = SPDR;

        SPDR = 0xFF; 
        loop_until_bit_is_set( SPSR, SPIF ); 
        * p++ = SPDR;

	} while ( cnt -= 2 );

}


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

/* 1:OK, 0:Timeout */
static int wait_ready()	{
	
    BYTE d;

	// Wait for ready in timeout of 500ms
    Timer2 = 50;

    do {
		
        d = xchg_spi( 0xFF );
	
    } while ( d != 0xFF && Timer2 );

	return ( d == 0xFF ) ? 1 : 0;

}


/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static void deselect() {
	
    CS_HIGH();
	
    // Dummy clock (force DO hi-z for multiple slave SPI)
    xchg_spi( 0xFF );	

}


/*-----------------------------------------------------------------------*/
/* Select the card and wait for ready                                    */
/*-----------------------------------------------------------------------*/

/* 1:Successful, 0:Timeout */
static int select()	{
	
    CS_LOW();
	
    // Dummy clock (force DO enabled)
    xchg_spi( 0xFF );	

	if ( wait_ready() ) return 1;	/* OK */

	deselect();

	// Timeout
    return 0;	

}


/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

/* buff - Data buffer to store received data */
/* btr  - Byte count (must be multiple of 4) */
static int rcvr_datablock ( BYTE * buff, UINT btr ) {
	
    BYTE token;

	Timer1 = 20;

	// Wait for data packet in timeout of 200ms
    do {							

		token = xchg_spi( 0xFF );

	} while ( ( token == 0xFF ) && Timer1 );

	// If not valid data token, retutn with error
    if ( token != 0xFE ) return 0;	

	// Receive the data block into buffer
    rcvr_spi_multi( buff, btr );		

	// Discard CRC
    xchg_spi( 0xFF );					
	xchg_spi( 0xFF );

	// Return with success
    return 1;						

}


/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

static
int xmit_datablock (
	const BYTE *buff,	/* 512 byte data block to be transmitted */
	BYTE token			/* Data/Stop token */
)
{
	BYTE resp;


	if (!wait_ready()) return 0;

	xchg_spi(token);					/* Xmit data token */
	if (token != 0xFD) {	/* Is data token */
		xmit_spi_multi(buff, 512);		/* Xmit the data block to the MMC */
		xchg_spi(0xFF);					/* CRC (Dummy) */
		xchg_spi(0xFF);
		resp = xchg_spi(0xFF);			/* Reveive data response */
		if ((resp & 0x1F) != 0x05)		/* If not accepted, return with error */
			return 0;
	}

	return 1;
}



/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (		/* Returns R1 resp (bit7==1:Send failed) */
	BYTE cmd,		/* Command index */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready */
	deselect();
	if (!select()) return 0xFF;

	/* Send command packet */
	xchg_spi(0x40 | cmd);				/* Start + Command index */
	xchg_spi((BYTE)(arg >> 24));		/* Argument[31..24] */
	xchg_spi((BYTE)(arg >> 16));		/* Argument[23..16] */
	xchg_spi((BYTE)(arg >> 8));			/* Argument[15..8] */
	xchg_spi((BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) + Stop */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) Stop */
	xchg_spi(n);

	/* Receive command response */
	if (cmd == CMD12) xchg_spi(0xFF);		/* Skip a stuff byte when stop reading */
	n = 10;								/* Wait for a valid response in timeout of 10 attempts */
	do
		res = xchg_spi(0xFF);
	while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

// Physical drive nmuber (0)
DSTATUS disk_initialize( BYTE drv ) {
	
    BYTE n, cmd, ty, ocr[4];

	// Supports only single drive
    if ( drv ) return STA_NOINIT;			

	// Turn off the socket power to reset the card
    power_off();						

	// No card in the socket
    if ( Stat & STA_NODISK ) return Stat;

	// Turn on the socket power
    power_on();

	FCLK_SLOW();

	// 80 dummy clocks
    for ( n = 10; n; n-- ) xchg_spi( 0xFF );

	ty = 0;

	// Enter Idle state
    if ( send_cmd( CMD0, 0 ) == 1 ) {

		// Initialization timeout of 1000 msec
        Timer1 = 100;

		// SDv2?
        if ( send_cmd( CMD8, 0x1AA ) == 1) {	

			// Get trailing return value of R7 resp
            for ( n = 0; n < 4; n++ ) ocr[n] = xchg_spi( 0xFF );

			// The card can work at vdd range of 2.7-3.6V
            if ( ocr[2] == 0x01 && ocr[3] == 0xAA ) {

				// Wait for leaving idle state (ACMD41 with HCS bit)
                while ( Timer1 && send_cmd( ACMD41, 1UL << 30 ) );

				// Check CCS bit in the OCR
                if ( Timer1 && send_cmd( CMD58, 0 ) == 0 ) {

					for ( n = 0; n < 4; n++ ) ocr[n] = xchg_spi( 0xFF );

					// SDv2
                    ty = ( ocr[0] & 0x40 ) ? CT_SD2 | CT_BLOCK : CT_SD2;	

				}

			}

		// SDv1 or MMCv3
        } else {		

			if ( send_cmd( ACMD41, 0 ) <= 1 ) {
				
                // SDv1
                ty = CT_SD1; 
                cmd = ACMD41;	

			} else {

                // MMCv3
				ty = CT_MMC; 
                cmd = CMD1;	

			}

			// Wait for leaving idle state
            while ( Timer1 && send_cmd( cmd, 0 ) );			

			// Set R/W block length to 512
            if ( !Timer1 || send_cmd( CMD16, 512 ) != 0 ) ty = 0;

		}

	}

	CardType = ty;

	deselect();

	// Initialization succeded
    if ( ty ) {			

		// Clear STA_NOINIT
        Stat &= ~STA_NOINIT;		
		FCLK_FAST();

	// Initialization failed
    } else {			

		power_off();
	}

	return Stat;

}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
	if (drv) return STA_NOINIT;		/* Supports only single drive */
	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	if (drv || !count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) {	/* Single block read */
		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512))
			count = 0;
	}
	else {				/* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	if (drv || !count) return RES_PARERR;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (Stat & STA_PROTECT) return RES_WRPRT;

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) {	/* Single block write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE))
			count = 0;
	}
	else {				/* Multiple block write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
				count = 1;
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	BYTE n, csd[16];
    BYTE * ptr = ( BYTE * ) buff;
	DWORD csize;


	if (drv) return RES_PARERR;

	res = RES_ERROR;

	if (ctrl == CTRL_POWER) {
		switch (ptr[0]) {
		case 0:		/* Sub control code (POWER_OFF) */
			power_off();		/* Power off */
			res = RES_OK;
			break;
		case 1:		/* Sub control code (POWER_GET) */
			ptr[1] = (BYTE)power_status();
			res = RES_OK;
			break;
		default :
			res = RES_PARERR;
		}
	}
	else {
		if (Stat & STA_NOINIT) return RES_NOTRDY;

		switch (ctrl) {
		case CTRL_SYNC :		/* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
			if (select()) {
				deselect();
				res = RES_OK;
			}
			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
				if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
					csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
					*(DWORD*)buff = csize << 10;
				} else {					/* SDC ver 1.XX or MMC*/
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(DWORD*)buff = csize << (n - 9);
				}
				res = RES_OK;
			}
			break;

		case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			if (CardType & CT_SD2) {	/* SDv2? */
				if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
					xchg_spi(0xFF);
					if (rcvr_datablock(csd, 16)) {				/* Read partial block */
						for (n = 64 - 16; n; n--) xchg_spi(0xFF);	/* Purge trailing data */
						*(DWORD*)buff = 16UL << (csd[10] >> 4);
						res = RES_OK;
					}
				}
			} else {					/* SDv1 or MMCv3 */
				if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
					if (CardType & CT_SD1) {	/* SDv1 */
						*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
					} else {					/* MMCv3 */
						*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
					}
					res = RES_OK;
				}
			}
			break;

		case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
			*ptr = CardType;
			res = RES_OK;
			break;

		case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
			if (send_cmd(CMD9, 0) == 0		/* READ_CSD */
				&& rcvr_datablock(ptr, 16))
				res = RES_OK;
			break;

		case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
			if (send_cmd(CMD10, 0) == 0		/* READ_CID */
				&& rcvr_datablock(ptr, 16))
				res = RES_OK;
			break;

		case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
			if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
				for (n = 4; n; n--) *ptr++ = xchg_spi(0xFF);
				res = RES_OK;
			}
			break;

		case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
			if (send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
				xchg_spi(0xFF);
				if (rcvr_datablock(ptr, 64))
					res = RES_OK;
			}
			break;

		default:
			res = RES_PARERR;
		}

		deselect();
	}

	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure                                      */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

void disk_timerproc() {
	
    BYTE n, s;

    // 100Hz decrement timer
	n = Timer1;				

	if ( n ) Timer1 = --n;

	n = Timer2;

	if ( n ) Timer2 = --n;

	s = Stat;

	// Write protected
    if ( SOCKWP ) {

		s |= STA_PROTECT;

    // Write enabled
    } else {

		s &= ~STA_PROTECT;
    }

	// Card inserted
    if ( SOCKINS ) {			

        s &= ~STA_NODISK;
    
    // Socket empty
    } else {
	
        s |= ( STA_NODISK | STA_NOINIT );
    }

	// Update MMC status
    Stat = s;				

}
