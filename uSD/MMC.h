#ifndef __MMC_H__
#define __MMC_H__

/* Status of Disk Functions */
typedef BYTE	DSTATUS;


/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} DRESULT;


#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */

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

public:

    static DSTATUS Initialize();
    static DRESULT Read( BYTE * buff, DWORD lba, WORD ofs, WORD cnt );
    static DRESULT Write( const BYTE * buff, DWORD sa );
    static DRESULT Write( FCHAR_PTR buff, DWORD sa );

private:

};

#endif // __MMC_H__
