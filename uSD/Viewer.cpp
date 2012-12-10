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

extern char * utoa_fast_div( uint32_t value, char * buffer );


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


/**
 * Отрисовка заголовка окна
 */
void CViewer::DrawTitle( char * Caption ) {

    CConsole::SetTextAttributes( atOff );        
    CConsole::MoveTo( 1, 1 );

    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clWhite );
    CConsole::WriteString( Caption );

    CConsole::ClearEndOfLine();

    CConsole::MoveTo( 60, 1 );
    CConsole::WriteString( SPSTR( "CP1251" ) );

    CConsole::MoveTo( 70, 1 );
    CConsole::WriteString( utoa_fast_div(  CFileManager::pCurrentPanel->FileInfo.fsize, buffer ) );

}


/**
 * Реакция на активацию окна.
 */
void CViewer::FormActivate() {

    WORD wSize = 0;   

    CConsole::CursorOff();                       

    // Монтирование FAT32.
    res = CFAT::Mount( & fs );

    // Если монтирование было успешным.
    if ( res == FR_OK ) {		

        // Присоеденяем к пути имя выбранного файла.
        CommandString[0] = 0;        
        
        strcat( CommandString, CFileManager::pCurrentPanel->Path );

        wSize = strlen( CommandString );

        if ( strlen( CFileManager::pCurrentPanel->Path ) > 1 ) {
        
            CommandString[ wSize ] = '/';                            
            CommandString[ wSize + 1 ] = 0;

        }

        strcat( CommandString, CFileManager::pCurrentPanel->FileInfo.fname );

        // Отображаем название файла в заголовке окна.
        DrawTitle( CommandString );

        // Заполняем фон.
        CConsole::SetTextAttributes( atOff );    
        CConsole::SetForegroundColor( clLightGray );
        CConsole::SetBackgroundColor( clBlue );

        for ( uint8_t i = 2; i < 25; i++ ) {

            CConsole::MoveTo( 1, i );
            CConsole::ClearEndOfLine();
        }

        CConsole::MoveTo( 1, 2 );

        uint8_t row = 0;

        // Открываем файл.
        res = CFAT::Open( CommandString );

        // Читаем файл по блокам.
        res = CFAT::Read( & CommandString, 128, & wSize );

        while ( ( res == FR_OK ) && ( wSize > 0 ) ) {
            
            for ( uint8_t i = 0; i < wSize; i++ ) {

                if ( ( CommandString[i] != 0x0D ) 
                    && ( CommandString[i] != 0x0A ) ) {

                    CConsole::PutChar( CommandString[i], CConsole::cp1251 );

                } else {

                    if ( CommandString[i] == 0x0D ) {
                    
                        row++;
                        if ( row > 22 ) break;
                    }

                    CConsole::PutChar( CommandString[i] );
                }

            } // while

            if ( row > 22 ) break;

            res = CFAT::Read( & CommandString, 128, & wSize );

        }

    }

    // Отмонтируем FatFs
    res = CFAT::Mount( NULL );

    // Очищаем буфер.
    CommandString[0] = 0;

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