#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "PLC.h"
#include "CommandShell.h"


// -=[ Внешние ссылки ]=-


// -=[ Постоянные во флеш-памяти ]=-

FLASHSTR_DECLARE( char, szNewLine, "\r\n" );
FLASHSTR_DECLARE( char, szcmdHelp, "help" );
FLASHSTR_DECLARE( char, szcmdClearScreen, "cls" );
FLASHSTR_DECLARE( char, szcmdFm, "fm" );
FLASHSTR_DECLARE( char, szcmdBeep, "beep" );

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


EnCommand CCommands::operator[] ( char * Text ) {

    // Убираем разделительные символы.
    trim( Text );

    // Пустая строка.
    if ( strlen( Text ) == 0 ) return cmdNone;

    if ( strcmp_P( Text, ( PGM_P ) szcmdHelp ) == 0 ) return cmdHelp;

    if ( strcmp_P( Text, ( PGM_P ) szcmdClearScreen ) == 0 ) return cmdClearScreen;

    if ( strcmp_P( Text, ( PGM_P ) szcmdFm ) == 0 ) return cmdFileManager;

    if ( strcmp_P( Text, ( PGM_P ) szcmdBeep ) == 0 ) return cmdBeep;

    return cmdUnknown;

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


void CCommandShell::Initialization() {

    // Очищаем командную строку.
    CommandString[0] = 0;

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
                    CConsole::WriteString( szcmdHelp );

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( " - вывод подсказки.\r\n" ), CConsole::cp1251 );

                    CConsole::SetForegroundColor( clLightRed );
                    CConsole::WriteString( szcmdClearScreen );

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( " - очистка экрана.\r\n" ), CConsole::cp1251 );

                    CConsole::SetForegroundColor( clLightRed );
                    CConsole::WriteString( szcmdFm );

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( " - запуск файл менеджера.\r\n" ), CConsole::cp1251 );

                    CConsole::SetForegroundColor( clLightRed );
                    CConsole::WriteString( szcmdBeep );

                    CConsole::SetForegroundColor( clWhite );
                    CConsole::WriteString( SPSTR( " - звуковой сигнал.\r\n" ), CConsole::cp1251 );

                    Prompt();
                    break;
                }

                // Запуск файл менеджера.
                case cmdFileManager: {

                    CPLC::SetActiveWindow( HWND_FILE_MANAGER );
                    break;
                }

                // Подача звукового сигнала.
                case cmdBeep: {

                    CConsole::Beep();
                    Prompt();
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

        case VK_LEFT: { break; }

        case VK_RIGHT: { break; }

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
