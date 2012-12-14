#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "PLC.h"
#include "FileManager.h"
#include "Player.h"


// -=[ Внешние ссылки ]=-

extern char szROOT[2];
extern char buffer[ 16 ];
extern char CommandString[ 128 ];
extern FRESULT res;
extern FATFS fs;

extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-


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


/**
 * Реакция на активацию окна.
 */
void CPlayer::FormActivate() {       

    CConsole::CursorOff();           

    // Считываем заголовок WAV-файла.

    #ifdef __GNUC__

        strcpy_P( CommandString, ( PGM_P ) & SPSTR( "Media Info: " ) );

    #elif defined( __ICCAVR__ )

        strcpy_P( CommandString, SPSTR( "Media Info:" ) );

    #endif
     
    // Присоеденяем к пути имя выбранного файла.
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

}


void CPlayer::FormKeyDown( uint16_t Key ) {

    switch ( Key ) {

        case VK_ESCAPE: { 
               
            CommandString[0] = 0;
            CPLC::SetActiveWindow( HWND_MAIN_SCREEN );
            break; 
        }


    } // switch

}
