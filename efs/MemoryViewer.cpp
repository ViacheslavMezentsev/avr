///////////////////////////////////////////////////////////
//  MemoryViewer.cpp
//  Implementation of the Class CMemoryViewer
//  Created on:      06-окт-2013 17:44:50
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "EFS.h"
#include "CommandShell.h"
#include "MemoryViewer.h"


// -=[ Внешние ссылки ]=-

extern HWND hwndActiveWindow;
extern char CommandString[];

// -=[ Постоянные во флеш-памяти ]=-

FLASHSTR_DECLARE( char, szViewMemory, "Просмотр памяти: EEPROM (CP1251)" );

// -=[ Переменные в ОЗУ ]=-

char hexchars[ 17 ] = "0123456789ABCDEF";

struct efs_file afile;
CKeys Keys;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


void CMemoryViewer::Initialization() {

    // Очищаем командную строку.
    CommandString[0] = 0;

}


/**
 * Отрисовка заголовка окна.
 */
void CMemoryViewer::DrawTitle() {

    CConsole::SetColor( clLightGray, clBlue );
    CConsole::ClearScreen();
    CConsole::CursorOff();

    CConsole::SetColor( clWhite, clGreen );
    CConsole::WriteString( szViewMemory, CConsole::cp1251 );
    CConsole::ClearLine( CConsole::cmFromCursorToEnd );

}


void CMemoryViewer::DrawMemory() {

    uint8_t i, j;

    for ( i = 0; i < 23; i++ ) {

        ECHAR_PTR mptr( ( char * ) ( i * 16 ) );

        CConsole::MoveTo( 1, i + 2 );

        CConsole::SetColor( clYellow, clBlue );

        j = 8;

        do {

            j--;
            CConsole::PutChar( hexchars[ ( ( ( uint16_t ) & mptr ) >> ( j * 4 ) ) & 0xF ] );

        } while ( j > 0 );

        CConsole::WriteString( ":  " );

        CConsole::SetForegroundColor( clWhite );

        for ( j = 0; j < 16; j++ ) {

            CConsole::PutChar( hexchars[ * mptr >> 4 ] );
            CConsole::PutChar( hexchars[ * mptr & 0xF ] );
            mptr++;
            CConsole::PutChar( ' ' );

            if ( j == 7 ) {

                CConsole::PutChar( '-' );
                CConsole::PutChar( ' ' );

            }

        }

        CConsole::PutChar( ACS_VLINE );
        CConsole::PutChar( ' ' );

        ECHAR_PTR eepch( ( char * ) ( i * 16 ) );

        for ( j = 0; j < 16; j++ ) {

            if ( * eepch >= ' ' ) {

                CConsole::PutChar( * eepch, CConsole::cp1251 );

            } else {

                CConsole::PutChar( '.' );
            }

            eepch++;

        }

        CConsole::ClearLine( CConsole::cmFromCursorToEnd );

    }

}


/**
 * Отображаем линейку функциональных клавиш.
 */
void CMemoryViewer::DrawFunctionKeys( CKeys & Keys ) {

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // Перебираем функциональные клавиши.
    for ( uint8_t i = 0; i < Keys.Count(); i++ ) {

        if ( Keys[i] != 0 ) {
            
            // Назначенная комбинация клавиш.
            CConsole::SetColor( clRed, clWhite );
            
            uint8_t ch, n = 0;

            ch = Keys[i][ n++ ];

            while ( ch != '\0' ) {                                 

                // После разделителя меняем цвет для описания действия.
                if ( ch == '|' ) { 
                    
                    CConsole::SetForegroundColor( clBlack );

                } else {

                    CConsole::PutChar( ch, CConsole::cp1251 );
                }

                ch = Keys[i][ n++ ];

            }

        }

    }

    // Заполняем пустое пространство до конца строки.
    CConsole::ClearLine();

}


void CMemoryViewer::FormActivate() {

    Initialization();

    DrawTitle();
    DrawMemory();
    DrawFunctionKeys( Keys );

}


void CMemoryViewer::FormKeyDown( uint16_t Key ) {

    uint8_t tmp = 0;

    switch ( Key ) {

        case VK_ESCAPE: {

            CCommandShell::FormActivate();

            hwndActiveWindow = HWND_COMMAND_SHELL;

            break;
        }

        case VK_RETURN: break;

        case VK_BACK: break;

        default:

            switch ( Key ) {

                // Очищаем EEPROM до смешения 240.
                case '0': {
                
                    for ( uint16_t n = 0; n < 240; n++ ) eeprom_write_byte( ( uint8_t * ) n, 0 );

                    DrawMemory();
                    break;
                }

                // Создать файл.
                case '1': {

                    CEFS::CreateFile( & afile, 0xCF, "Настройки" );

                    CEFS::WriteFile( & afile, ( uint8_t * ) "-123456-", 0, 8 );

                    DrawMemory();
                    break;
                }

                // Удалить файл.
                case '2': {

                    CEFS::DeleteFile( & afile );
                    DrawMemory();
                    break;
                }

            }

    } // switch

}