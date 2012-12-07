#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "PLC.h"
#include "FileManager.h"
#include "Viewer.h"


// -=[ Внешние ссылки ]=-

extern char buffer[ 16 ];
extern char CommandString[ 128 ];
extern FRESULT res;
extern FATFS fs;

// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


LRESULT CViewer::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

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


void CViewer::FormActivate() {

    uint16_t Counter;
    WORD wSize;   

    CConsole::CursorOff();                   
    CFileManager::DrawFrame( 1, 2, 78, 21, clLightGray, clBlue );

    // Монтирование FAT32
    res = CFAT::Mount( & fs );

    // Если монтирование было успешным
    if ( res == FR_OK ) {		

        // Открываем файл
        CommandString[0] = 0;
        
        strcat( CommandString, CFileManager::pCurrentPanel->Path );

        wSize = strlen( CommandString );

        if ( strlen( CFileManager::pCurrentPanel->Path ) > 1 ) {
        
            CommandString[ wSize ] = '/';                            
            CommandString[ wSize + 1 ] = 0;

        }

        strcat( CommandString, CFileManager::pCurrentPanel->FileInfo.fname );

        res = CFAT::Open( CommandString );

        Counter = 0;

        // Читаем
        res = CFAT::Read( & buffer, 15, & wSize );

        CConsole::MoveTo( 2, 3 );

        while ( ( res == FR_OK ) && ( wSize > 0 ) ) {

            for ( uint8_t i = 0; i < 15; i++ ) {

                if ( buffer[i] == 0x0D ) continue;
                
                if ( buffer[i] == 0x0A ) {

                    CConsole::WriteString( SPSTR( "\r\n" ) );
                    CConsole::Move( mdForward, 1 ); 
                    Counter++;

                    continue;

                }

                CConsole::PutChar( buffer[i] , CConsole::cp1251 );

            }

            if ( Counter > 19 ) break;

            res = CFAT::Read( & buffer, 15, & wSize );

        }

    }

    // Отмонтируем FatFs
    res = CFAT::Mount( NULL );
}


void CViewer::FormKeyDown( uint16_t Key ) {

    switch ( Key ) {

        case VK_ESCAPE: { 
               
            CommandString[0] = 0;
            CPLC::SetActiveWindow( HWND_MAIN_SCREEN );
            break; 
        }


    } // switch

}