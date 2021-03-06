#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "CommandShell.h"


// -=[ ������� ������ ]=-


// -=[ ���������� �� ����-������ ]=-

FLASHSTR_DECLARE( char, szNewLine, "\r\n" );
FLASHSTR_DECLARE( char, szHelp, "help" );
FLASHSTR_DECLARE( char, szClearScreen, "cls" );


// -=[ ���������� � ��� ]=-

char CommandString[ 16 ];
CCommands Commands;


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/

// Programming Strings: http://www.wilsonmar.com/1strings.htm
void trim( char * s ) {

    // ������� ������� � ���� � ������ ������:
    uint8_t i = 0, j;

    while ( ( s[i] == ' ' ) || ( s[i] == '\t' ) ) i++;

    if ( i > 0 ) {

        for ( j = 0; j < strlen(s); j++ ) {

            s[j] = s[ j + i ];
        }

        s[j] = '\0';
    }

    // ������� ������� � ���� � ����� ������:
    i = strlen(s) - 1;

    while ( ( s[i] == ' ' ) || ( s[i] == '\t' ) ) i--;

    if ( i < ( strlen(s) - 1 ) ) s[ i + 1 ] = '\0';

}


EnCommand CCommands::operator[] ( char * Text ) {

    // ������� �������������� �������.
    trim( Text );

    // ������ ������.
    if ( Text[0] == '\0' ) return cmdNone;

    if ( strcmp_P( Text, ( PGM_P ) szHelp ) == 0 ) return cmdHelp;

    if ( strcmp_P( Text, ( PGM_P ) szClearScreen ) == 0 ) return cmdClearScreen;

    return cmdUnknown;

}


void CCommandShell::Initialization() {

    // ������� ��������� ������.
    CommandString[0] = '\0';

}


void CCommandShell::Info() {

    CConsole::SetColor( clLightGray, clBlack );
    CConsole::ClearScreen();

    CConsole::MoveTo( 1, 25 );

    CConsole::WriteString( SPSTR( "��������� ��������, ������ " ), CConsole::cp1251 );
    CConsole::WriteString( CVersion::GetVersionString() );

#ifdef __GNUC__
    CConsole::WriteString( SPSTR( " (gnu)\r\n" ) );
#elif defined __ICCAVR__
    CConsole::WriteString( SPSTR( " (iar)\r\n" ) );
#endif

}


void CCommandShell::Prompt() {

    // ������� ����������� �� ����� ������.
    CConsole::SetForegroundColor( clLightGreen );
    CConsole::WriteString( SPSTR( "\r\n[ATtiny2313]$ " ) );

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

    uint8_t tmp;

    switch ( Key ) {

        case VK_RETURN: {

            switch ( Commands[ CommandString ] ) {

                case cmdClearScreen: Info(); break;

                case cmdHelp: {

                    CConsole::WriteString( szNewLine );
                    CConsole::SetForegroundColor( clLightRed );
                    CConsole::WriteString( szHelp );
                    CConsole::WriteString( szNewLine );
                    break;
                }

                default:;

            }

            CommandString[0] = '\0';
            Prompt();            
            break;

        }

        case VK_BACK: {

	        tmp = strlen( CommandString );

            // ������� ���������� ������, ���� ����� ��������� ������ �� ����.
            if ( tmp > 0 ) {

                CConsole::PutChar( VK_BACK );
                CConsole::ClearForward(1);

                CommandString[ --tmp ] = '\0';

            }

            break;

        }

        default: {

            tmp = strlen( CommandString );

            if ( tmp >= sizeof( CommandString ) - 1 ) return;

            // ������� ������ �� �����.
            CConsole::PutChar( ( uint8_t ) Key );

            CommandString[ tmp++ ] = ( uint8_t ) Key;
            CommandString[ tmp ] = '\0';

        }

    } // switch

}
