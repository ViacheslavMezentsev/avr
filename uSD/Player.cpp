#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "PLC.h"
#include "FileManager.h"
#include "Player.h"


// -=[ ������� ������ ]=-

extern char szROOT[2];
extern char buffer[ 16 ];
extern char CommandString[ 128 ];
extern FRESULT res;
extern FATFS fs;

extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-


/***********************
*  � � � � � � � � � �
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
 * ������� �� ��������� ����.
 */
void CPlayer::FormActivate() {       

    CConsole::CursorOff();           

    // ��������� ��������� WAV-�����.

    #ifdef __GNUC__

        strcpy_P( CommandString, ( PGM_P ) & SPSTR( "Media Info: " ) );

    #elif defined( __ICCAVR__ )

        strcpy_P( CommandString, SPSTR( "Media Info:" ) );

    #endif
     
    // ������������ � ���� ��� ���������� �����.
    strcat( CommandString, CFileManager::pCurrentPanel->Path );
    
    if ( strcmp( CommandString, szROOT ) != 0 ) strcat( CommandString, szROOT );

    strcat( CommandString, CFileManager::pCurrentPanel->FileInfo.fname );

    uint8_t top = 3;

    CConsole::DrawFrame( 10, top++, 60, 18, clBlack, clWhite, CommandString );

    top++;

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������ ���" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������ �����" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�����������������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "����� �����" ), CConsole::cp1251 );

    top++;

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�������������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�������� Endianness �������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������������� ������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������������� ������/���������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�����������������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�����(�)" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "�������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������� �������" ), CConsole::cp1251 );

    CConsole::MoveTo( 11, top++ );
    CConsole::WriteString( SPSTR( "������ ������" ), CConsole::cp1251 );   

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
