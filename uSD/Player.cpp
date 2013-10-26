#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "PLC.h"
#include "FileManager.h"
#include "Player.h"


// FourCC
#define FCC( c1, c2, c3, c4 ) (((DWORD)c4<<24)+((DWORD)c3<<16)+((WORD)c2<<8)+(BYTE)c1)

// -=[ Внешние ссылки ]=-

extern char szROOT[2];
extern char buffer[ 16 ];
extern char CommandString[ 128 ];
extern FRESULT res;
extern FATFS fs;

extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

// FIFO controls
volatile BYTE FifoRi, FifoWi, FifoCt;
// Return value. Put this here to avoid bugs of avr-gcc
WORD rb;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


LRESULT CPlayer::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    switch ( uMsg ) {

        case WM_ACTIVATE: {

            switch ( wParam ) {

                case WA_ACTIVE: { 
                    
                    FormActivate();
                    break; 
                }

                case WA_INACTIVE: { break; }

            }

            break;
        }

        case WM_KEYDOWN: { 
            
            FormKeyDown( wParam );
            break;

        }

    }

    return 0;

}


DWORD CPlayer::LoadHead() {

	DWORD fcc, sz;
	UINT i;

	// Load file header (256 bytes)
    res = CFAT::Read( CommandString, 256, & rb );	

	if ( res || rb != 256 ) return res;

    if ( LD_DWORD( CommandString + 8 ) != FCC( 'W', 'A', 'V', 'E' ) ) return 12;

	i = 12;

	while ( i < 200 ) {

		// FCC
        fcc = LD_DWORD( & CommandString[i] );

		// Chunk size
        sz = LD_DWORD( & CommandString[ i + 4 ] );

		i += 8;

		switch ( fcc ) {

            // 'fmt ' chunk
            case FCC( 'f', 'm', 't', ' ' ): {
			
                // Check chunk size
                if ( sz > 100 || sz < 16 ) return 10;

			    // Check coding type (1)
                if ( CommandString[ i + 0 ] != 1 ) return 11;

			    // Check channels (1/2)
                if ( CommandString[ i + 2 ] != 1 && CommandString[ i + 2 ] != 2 ) return 11;

			    // Channel flag
                //GPIOR0 = CommandString[ i + 2 ];

			    // Check resolution (8/16)
                if ( CommandString[ i + 14 ] != 8 && CommandString[ i + 14 ] != 16 ) return 11;

			    // Resolution flag
                //GPIOR0 |= CommandString[ i + 14 ];

			    // Sampling freq
                OCR0 = ( BYTE ) ( F_CPU / 8 / LD_WORD( & CommandString[ i + 4 ] ) ) - 1;
			
                break;
            }

		    // 'fact' chunk (skip)
            case FCC( 'f', 'a', 'c', 't' ): { break; }

		    // 'data' chunk (start to play)
            case FCC( 'd', 'a', 't', 'a' ): {		
			
                fs.fptr = i;
			    return sz;
            }

		    // Unknown chunk (error)
            default: return 14;

		}

		i += sz;

	}

	return 15;

}


// Проигрывание звукового файла.
FRESULT CPlayer::Play ( const char * fn ) {
	
    BYTE sw;
    WORD btr;
    DWORD sz;

    // Открываем файл.
    res = CFAT::Open( CommandString );

    if ( res == FR_OK ) {

		// Load file header.
        sz = LoadHead();

		if ( sz < 256 ) return ( FRESULT ) 99;

		// Set stream mode.
        //fs.flag |= FA_STREAM;				

	    // Reset FIFO
        FifoCt = 0; 
        FifoRi = 0; 
        FifoWi = 0;

		// Snip sector unaligned part.
        res = CFAT::Read( 0, 512 - fs.fptr, & rb );

		sz -= rb;

		// Button status flag.
        sw = 1;		

		do {
			
            // Forward audio data.
			btr = ( sz > 1024 ) ? 1024 : ( WORD ) sz;

            // Читаем файл по блокам.
            res = CFAT::Read( 0, btr, & rb );

            if ( res != FR_OK || btr != rb ) break;

            sz -= rb;

            // Check button down and break on button down.
			sw <<= 1;

			if ( !CHECKBIT( PINB, PORTB0 ) && ( ++sw == 1 ) ) break;

		// Repeat until all data read.
        } while ( rb == 1024 );

	}

	// Wait for FIFO empty.
    while ( FifoCt );

	OCR1A = 128;
    OCR1B = 128;

	return res;

}


/**
 * Реакция на активацию окна.
 */
void CPlayer::FormActivate() {       

    CConsole::CursorOff();           

    // Считываем заголовок WAV-файла.
    strcpy_P( CommandString, ( PGM_P ) SPSTR( "Media Info: " ) );
     
    // Присоединяем к пути имя выбранного файла.
    strcat( CommandString, CFileManager::pCurrentPanel->Path );
    
    if ( strcmp( CommandString, szROOT ) != 0 ) strcat( CommandString, szROOT );

    strcat( CommandString, CFileManager::pCurrentPanel->FileInfo.fname );

    uint8_t top = 3;

    CConsole::DrawFrame( 10, top++, 60, 18, clBlack, clWhite, CommandString );

    top++;

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Полное имя" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Формат" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Размер файла" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Продолжительность" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Общий поток" ), CConsole::cp1251 );

    top++;

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Идентификатор" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Формат" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Параметр Endianness формата" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Идентификатор кодека" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Идентификатор кодека/Подсказка" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Продолжительность" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Битрейт" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Канал(ы)" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Частота" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Битовая глубина" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "Размер потока" ), CConsole::cp1251 );   

    for ( top = 5; top < 22; top++ ) {
        
        if ( top == 10 ) continue;

        CConsole::MoveTo( 43, top );
        CConsole::PutChar( ':' );

    }

    // Монтирование FAT32.
    res = CFAT::Mount( & fs );

    // Присоединяем к пути имя выбранного файла.
    strcpy( CommandString, CFileManager::pCurrentPanel->Path );
    
    if ( strcmp( CommandString, szROOT ) != 0 ) strcat( CommandString, szROOT );

    strcat( CommandString, CFileManager::pCurrentPanel->FileInfo.fname );

    // Если монтирование было успешным.
    if ( res == FR_OK ) Play( CommandString );

    // Размонтируем FatFs.
    res = CFAT::Mount( NULL );

    // Очищаем буфер.
    CommandString[0] = 0;

}


void CPlayer::FormKeyDown( uint16_t Key ) {

    switch ( Key ) {

        case VK_ESCAPE: { 
               
            CommandString[0] = 0;
            CPLC::SetActiveWindow( HWND_FILE_MANAGER );
            break; 
        }


    } // switch

}
