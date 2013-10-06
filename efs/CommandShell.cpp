#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "CommandShell.h"


// -=[ Внешние ссылки ]=-


// -=[ Постоянные во флеш-памяти ]=-

FLASHSTR_DECLARE( char, szNewLine, "\r\n" );
FLASHSTR_DECLARE( char, szcmdHelp, "help" );
FLASHSTR_DECLARE( char, szcmdClearScreen, "cls" );


// -=[ Переменные в ОЗУ ]=-

char CommandString[ 16 ];
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
    if ( Text[0] == '\0' ) return cmdNone;

    if ( strcmp_P( Text, ( PGM_P ) ( & szcmdHelp ) ) == 0 ) return cmdHelp;

    if ( strcmp_P( Text, ( PGM_P ) ( & szcmdClearScreen ) ) == 0 ) return cmdClearScreen;

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


void CCommandShell::FormActivate() {

    Initialization();
    Info();
    Prompt();

};


void CCommandShell::FormKeyDown( uint16_t Key ) {

    uint8_t tmp = 0;

    switch ( Key ) {

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

        default: {

            if ( strlen( CommandString ) >= sizeof( CommandString ) - 1 ) return;

            // Выводим символ на экран.
            CConsole::PutChar( ( uint8_t ) Key );
            strcat( CommandString, ( char * ) & Key );

        }

    } // switch

}

