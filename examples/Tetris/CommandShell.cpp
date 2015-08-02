#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "Game.h"
#include "PLC.h"
#include "CommandShell.h"


// -=[ Внешние ссылки ]=-

extern CGame Game;


// -=[ Постоянные во флеш-памяти ]=-

FLASHSTR_DECLARE( char, szHelp, "help" );
FLASHSTR_DECLARE( char, szClearScreen, "cls" );
FLASHSTR_DECLARE( char, szGame, "game" );


// -=[ Переменные в ОЗУ ]=-

char CommandString[ 128 ];
CCommands Commands;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/

// Programming Strings: http://www.wilsonmar.com/1strings.htm
void trim( char * s ) {
    
    // удаляем пробелы и табы с начала строки:
    uint8_t i = 0, j;

    while ( ( s[i] == ' ' ) || ( s[i] == '\t' ) ) i++;

    if ( i > 0 ) {

        for ( j = 0; j < strlen(s); j++ ) {
            
            s[j] = s[ j + i ];
        }

        s[j] = '\0';
    }

    // удаляем пробелы и табы с конца строки:
    i = strlen(s) - 1;

    while ( ( s[i] == ' ' ) || ( s[i] == '\t' ) ) i--;

    if ( i < ( strlen(s) - 1 ) ) s[ i + 1 ] = '\0';

}


// Убираем пробелы справа.
void rtrim( char * string, char * trim ) {
       
    uint8_t i;
    
    for( i = strlen( string ) - 1;
        i >= 0 && strchr( trim, string[i] ) != NULL;
        i-- ){	  
        
        // переставляем терминатор строки 
        string[i] = '\0';
    }

}


// Убираем пробелы слева.
void ltrim( char * string, char * trim ) {

    while ( string[0] != '\0' && strchr( trim, string[0] ) != NULL ) {

        memmove( &string[0], &string[1], strlen( string ) );
    }

}


EnCommand CCommands::operator[] ( char * Text ) {

    // Убираем разделительные символы.
    trim( Text );

    // Пустая строка.
    if ( strlen( Text ) == 0 ) return cmdNone;

    if ( strcmp_P( Text, ( PGM_P ) ( & szHelp ) ) == 0 ) return cmdHelp;

    if ( strcmp_P( Text, ( PGM_P ) ( & szClearScreen ) ) == 0 ) return cmdClearScreen;

    if ( strcmp_P( Text, ( PGM_P ) ( & szGame ) ) == 0 ) return cmdGame;

    return cmdUnknown;

}


void CCommandShell::Initialization() {

    // Очищаем командную строку.
    CommandString[0] = 0;

}


void CCommandShell::Info() {

    CConsole::SetColor( clLightGray, clBlack );
    CConsole::ClearScreen();

    CConsole::MoveTo( 1, 25 );

    CConsole::WriteString( SPSTR( "Командная оболочка, версия " ), CConsole::cp1251 );
    CConsole::WriteString( CVersion::GetVersionString() );

#ifdef __GNUC__
    CConsole::WriteString( SPSTR( " (gnu)\r\n" ) );
#elif defined __ICCAVR__
    CConsole::WriteString( SPSTR( " (iar)\r\n" ) );
#endif

    CConsole::WriteString( SPSTR( "Дата сборки проекта: " ), CConsole::cp1251 );
    CConsole::WriteString( CVersion::GetBuildDateString(), CConsole::cp1251 );

    CConsole::WriteString( SPSTR( "\r\nАвтор: Мезенцев Вячеслав (viacheslavmezentsev@gmail.com)\r\n" ), CConsole::cp1251 );

}


void CCommandShell::Prompt() {

    // Выводим приглашение на новой строке.
    CConsole::SetForegroundColor( clLightGreen );
    CConsole::WriteString( SPSTR( "\r\n[ATmega16]$ " ) );
    
    CConsole::SetForegroundColor( clLightGray );
    CConsole::ClearLine();
    CConsole::CursorOn();

}


LRESULT CCommandShell::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    switch ( uMsg ) {

        // Здесь можно сделать какой-нибудь эффект.
        // Это событие делает окно активным.
        case SW_SHOW: { break; }

        case WM_ACTIVATE: {

            switch ( wParam ) {

                case WA_ACTIVE: { FormActivate(); break; }

                case WA_INACTIVE: { break; }

            }

            break;
        }

        case WM_KEYDOWN: { FormKeyDown( wParam ); break; }

        case WM_KEYUP: { break; }

        case WM_TIMER: {

            switch ( wParam ) {

                case ID_TIMER_10MS: { Form10msTimer(); break; }

                case ID_TIMER_100MS: { Form100msTimer(); break; }

                case ID_TIMER_500MS: { Form500msTimer(); break; }

                case ID_TIMER_1S: { Form1secTimer(); break; }

                case ID_TIMER_5S: { Form5secTimer(); break; }

            }

            break;

        }

        case WM_PAINT: { break; }

    }

    return 0;

}


void CCommandShell::FormActivate() {

    Initialization();
    Info();
    Prompt();

};


void CCommandShell::FormKeyDown( uint16_t Key ) {

    uint8_t tmp = 0;

    switch ( Key ) {

        case VK_ESCAPE: { break; }

        case VK_RETURN: {

            EnCommand command = Commands[ CommandString ];

            switch ( command ) {

                case cmdNone: { Prompt(); break; }

                case cmdClearScreen: { 
                    
                    CConsole::SetColor( clLightGray, clBlack );
                    CConsole::ClearScreen();

                    CConsole::MoveTo( 1, 25 );

                    Prompt(); 
                    break; 
                }

                case cmdHelp: {

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( "\r\nДоступные команды:\r\n" ), CConsole::cp1251 );

                    CConsole::SetForegroundColor( clLightRed );
                    CConsole::WriteString( szHelp );

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( " - вывод подсказки.\r\n" ), CConsole::cp1251 );

                    CConsole::SetForegroundColor( clLightRed );
                    CConsole::WriteString( szGame );

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( " - запуск игры Тетрис.\r\n" ), CConsole::cp1251 );

                    Prompt();
                    break;
                }

                // Запуск игры.
                case cmdGame: {

                    CPLC::SetActiveWindow( HWND_GAME );

                    break;
                }

                // Выводим сообщение о неподдерживаемой команде.
                case cmdUnknown: {

                    CConsole::SetForegroundColor( clWhite );

                    CConsole::WriteString( "\r\n\"" );
                    CConsole::WriteString( CommandString, CConsole::cp1251 );
                    
                    CConsole::WriteString( SPSTR( "\" не является внутренней командой или исполняемой программой.\r\n" ), CConsole::cp1251 );

                    Prompt();
                    break;
                }

            } // switch

            CommandString[0] = 0;
            break;

        }

        case VK_BACK: {

            tmp = strlen( CommandString );

            // Удаляем предыдущий символ, если буфер командной строки не пуст.
            if ( tmp > 0 ) {

                CConsole::PutChar( VK_BACK );
                CConsole::ClearForward(1);

                CommandString[ --tmp ] = 0;

            }

            break;

        }

        case VK_UP: { break; }

        case VK_DOWN: { break; }

        case VK_HOME: { break; }

        case VK_END: { break; }

        case VK_TAB: { break; }

        default: {

            tmp = strlen( CommandString );

            if ( tmp >= sizeof( CommandString ) - 1 ) return;

            // Выводим символ на экран.
            CConsole::PutChar( ( uint8_t ) Key );

            CommandString[ tmp++ ] = ( uint8_t ) Key;
            CommandString[ tmp ] = 0;

        }

    } // switch

}


void CCommandShell::Form10msTimer() {

};


void CCommandShell::Form100msTimer() {

};

void CCommandShell::Form500msTimer() {

};


void CCommandShell::Form1secTimer() {

};


void CCommandShell::Form5secTimer() {

};
