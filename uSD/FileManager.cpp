#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "PLC.h"
#include "CommandShell.h"
#include "FileManager.h"


// -=[ ������� ������ ]=-

extern char buffer[ 16 ];
extern char CommandString[ 128 ];
extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-

FRESULT res;
UFDATE FDate;
UFTIME FTime;
FILINFO fno;
DIR dir;
FATFS fs;

CKeys Keys;
CPanel LeftPanel;
CPanel RightPanel;

CPanel * CFileManager::pCurrentPanel = & LeftPanel;

char szROOT[2] = "/";


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/

// ������� ������ ��������� ��������� �� ���� ����������� � �����������
// ���������� IAR.
#if defined( __ICCAVR__ )

char * strstr_P( const char * s1, PGM_P s2 ){

    int n;

    if ( * s2 ) {

        while ( * s1 ) {

            for ( n = 0; * ( s1 + n ) == * ( s2 + n ); n++ ) {

                if ( !*( s2 + n + 1 ) ) return ( char * ) s1;
            }

            s1++;
        }

        return NULL;

    } else return ( char * ) s1;

}

#endif


void CFileManager::Initialization() {
    
    LeftPanel.Left = 1;
    LeftPanel.Top = 2;
    LeftPanel.Width = 38;
    LeftPanel.Height = 20;

    RightPanel.Left = 41;
    RightPanel.Top = 2;
    RightPanel.Width = 38;
    RightPanel.Height = 20;

    // ���������� ��� ������ � ������� ��������� ���������.
    LeftPanel.ItemIndex = 0;
    LeftPanel.ItemsCount = 0;

    RightPanel.ItemIndex = 0;
    RightPanel.ItemsCount = 0;

    // �������������� ��������� ���� ��� ������ ������.
    strcpy( LeftPanel.Path, szROOT );
    strcpy( RightPanel.Path, szROOT );

    // ������� ��������� ������.
    CommandString[0] = 0;

}


/**
 * ��������� ��������� ����.
 */
void CFileManager::DrawMainMenu() {

    CConsole::MoveTo( 1, 1 );
    CConsole::SetColor( clBlack, clWhite );    

    CConsole::WriteString( SPSTR( "�������� ��������, ATmega16 @ 16 ���, ������ ������: " ), CConsole::cp1251 );
    CConsole::WriteString( CVersion::GetVersionString() );

#ifdef __GNUC__
    CConsole::WriteString( SPSTR( " (gnu)" ) );
#elif defined __ICCAVR__
    CConsole::WriteString( SPSTR( " (iar)" ) );
#endif

    CConsole::ClearLine();

}


void CFileManager::WriteDateTime( CPanel & Panel, FILINFO & fno ) {

    // ����� ����.
    FDate.Value = fno.fdate;

    // ����.
    buffer[0] = FDate.fdate.Day / 10 + '0';
    buffer[1] = FDate.fdate.Day % 10 + '0';
    buffer[2] = '.';

    // �����.
    buffer[3] = FDate.fdate.Month / 10 + '0';
    buffer[4] = FDate.fdate.Month % 10 + '0';
    buffer[5] = '.';

    // ���.
    uint16_t tmp = 1980U + FDate.fdate.Year;

    tmp %= 1000U;
    tmp %= 100;

    buffer[6] = ( uint8_t ) ( tmp / 10 ) + '0';
    buffer[7] = ( uint8_t ) ( tmp % 10 ) + '0';

    buffer[8] = '\0';

    CConsole::WriteString( buffer );

    // ����� �������.
    FTime.Value = fno.ftime;

    // ����.
    buffer[0] = FTime.ftime.Hour / 10 + '0';
    buffer[1] = FTime.ftime.Hour % 10 + '0';
    buffer[2] = ':';

    // ������.
    buffer[3] = FTime.ftime.Minute / 10 + '0';
    buffer[4] = FTime.ftime.Minute % 10 + '0';
    buffer[5] = '\0';

    CConsole::Move( mdForward, 1 );
    CConsole::WriteString( buffer );

}


/**
 * �������� ��������� �������� �������� ������.
 */
void CFileManager::HightlightPanel( CPanel & Panel ) {

    CConsole::SetColor( clLightGray, clBlue );

    // ���������� ���� � ��������� ������.
    uint8_t len = ( Panel.Width - strlen( Panel.Path ) ) / 2;

    CConsole::MoveTo( Panel.Left + len, Panel.Top );

    CConsole::PutChar( ' ' );

    if ( & Panel == pCurrentPanel ) {

        CConsole::SetColor( clBlack, clWhite );
        CConsole::WriteString( Panel.Path );
        CConsole::SetColor( clLightGray, clBlue );

    } else {

        CConsole::WriteString( Panel.Path );

    }

    CConsole::PutChar( ' ' );

    if ( Panel.ItemsCount != 0 ) {

        if ( & Panel == pCurrentPanel ) CConsole::SetColor( clBlack, clWhite );

        // ���������� ����������� ��� ���������� ������.
        CConsole::MoveTo( Panel.Left + 13, Panel.ItemIndex + Panel.Top + 2 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 10 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 8 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::SetColor( clLightGray, clBlue );
        CConsole::MoveTo( Panel.Left + 1, Panel.ItemIndex + Panel.Top + 2 );

        // ���������� ������ �������� ��� ���������� ������.
        // ���� ������ - �����.
        if ( Panel.FileInfo.fattrib & AM_DIR ) {

            ( & Panel == pCurrentPanel ) ? CConsole::SetColor( clBlack, clWhite ) : CConsole::SetForegroundColor( clLightGreen );

            // ���.
            CConsole::WriteString( Panel.FileInfo.fname );

            len = 12 - strlen( Panel.FileInfo.fname );

            CConsole::ClearForward( len );
            CConsole::Move( mdForward, len );

            CConsole::MoveTo( Panel.Left + 14, Panel.ItemIndex + Panel.Top + 2 );

            CConsole::WriteString( SPSTR( "  [�����] " ), CConsole::cp1251 );

        // ���� ������ - ����.
        } else {

            ( & Panel == pCurrentPanel ) ? CConsole::SetColor( clBlack, clWhite ) : CConsole::SetForegroundColor( clLightGray );

            // ���.
            CConsole::WriteString( Panel.FileInfo.fname );

            len = 12 - strlen( Panel.FileInfo.fname );

            CConsole::ClearForward( len );
            CConsole::Move( mdForward, len );

            CConsole::MoveTo( Panel.Left + 14, Panel.ItemIndex + Panel.Top + 2 );

            len = 9 - strlen( utoa_fast_div( Panel.FileInfo.fsize, buffer ) );

            CConsole::ClearForward( ++len );
            CConsole::Move( mdForward, len );

            CConsole::WriteString( utoa_fast_div( Panel.FileInfo.fsize, buffer ) );

        }

        CConsole::MoveTo( Panel.Left + 25, Panel.ItemIndex + Panel.Top + 2 );

        WriteDateTime( Panel, Panel.FileInfo );

    } // if

}


void CFileManager::DrawPanel( CPanel & Panel ) {

    CConsole::DrawFrame( Panel.Left, Panel.Top, Panel.Width, Panel.Height, clLightGray, clBlue, Panel.Path );

    CConsole::SetForegroundColor( clLightYellow );

    // ��������� �������.
    CConsole::MoveTo( Panel.Left + 5, 3 );

    CConsole::WriteString( SPSTR( "���" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "������" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 5 );

    CConsole::WriteString( SPSTR( "����   �����" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

    // ���������� �������������� ����� �������.
    for ( uint8_t i = Panel.Top + 1; i < Panel.Top + Panel.Height + 1; i++ ) {

        CConsole::MoveTo( Panel.Left + 13, i );
        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 10 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 8 );
        CConsole::PutChar( ACS_VLINE );

    }

    // ��������� ��������� ��������.
    CConsole::MoveTo( Panel.Left + 13, 2 );
    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdForward, 10 );
    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdForward, 8 );
    CConsole::PutChar( 0xD1 );

    CConsole::Move( mdDown, 21 );
    CConsole::Move( mdBackward, 1 );
    CConsole::PutChar( 0xCF );

    CConsole::Move( mdBackward, 10 );
    CConsole::PutChar( 0xCF );

    CConsole::Move( mdBackward, 12 );
    CConsole::PutChar( 0xCF );

    // ������������.
    res = CFAT::Mount( & fs );

    // ������������� ������� ���������� ������ �� ��������.
    Panel.ItemsCount = 0;

    uint8_t len;

    // ��������� ���������� (�����).
    res = CFAT::OpenDir( & dir, Panel.Path );

    if ( res == FR_OK ) {

        for ( uint8_t i = Panel.Top + 2; i < Panel.Top + Panel.Height + 1; i++ ) {

            CConsole::MoveTo( Panel.Left + 1, i );

            res = CFAT::ReadDir( & dir, & fno );

            if ( res != FR_OK || fno.fname[0] == 0 ) break;

            // ����������� ������� ���������� ���������.
            Panel.ItemsCount++;

            // ��������� �������� ���������� ��������.
            if ( Panel.ItemIndex == ( i - Panel.Top - 2 ) ) {

                Panel.FileInfo = fno;
            }

            // ���� ������ - �����.
            if ( fno.fattrib & AM_DIR ) {

                CConsole::SetForegroundColor( clLightGreen );

                // ���.
                CConsole::WriteString( fno.fname );

                len = 12 - strlen( fno.fname );

                CConsole::ClearForward( len );
                CConsole::Move( mdForward, len );

                CConsole::MoveTo( Panel.Left + 14, i );

                CConsole::WriteString( SPSTR( "  [�����] " ), CConsole::cp1251 );

            // ���� ������ - ����.
            } else {

                CConsole::SetForegroundColor( clLightGray );

                // ���.
                CConsole::WriteString( fno.fname );

                len = 12 - strlen( fno.fname );

                CConsole::ClearForward( len );
                CConsole::Move( mdForward, len );

                CConsole::MoveTo( Panel.Left + 14, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                CConsole::ClearForward( ++len );

                CConsole::Move( mdForward, len );
                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

            CConsole::MoveTo( Panel.Left + 25, i );

            WriteDateTime( Panel, fno );

        } // for

    } // if

    if ( Panel.ItemIndex > Panel.ItemsCount - 1 ) Panel.ItemIndex = Panel.ItemsCount - 1;

    // ������������ ��������� ������� �������.
    HightlightPanel( Panel );

    // �����������.
    res = CFAT::Mount( NULL );

}


void CFileManager::DrawCommandLine( CPanel & Panel ) {

    // ������� �����������.
    CConsole::SetColor( clLightGreen, clBlack );
    CConsole::MoveTo( 1, 24 );

    CConsole::PutChar( '[' );
    CConsole::WriteString( Panel.Path );
    CConsole::WriteString( SPSTR( "]$ " ) );

    CConsole::SetForegroundColor( clLightGray );

    CConsole::SaveCursor();
    CConsole::ClearLine( CConsole::cmFromCursorToEnd );
    CConsole::RestoreCursor();

    CConsole::CursorOn();

}


/**
 * ���������� ������� �������������� ������.
 */
void CFileManager::DrawFunctionKeys( CKeys & Keys ) {

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // ���������� �������������� �������.
    for ( uint8_t i = 0; i < Keys.Count(); i++ ) {

        if ( Keys[i] != 0 ) {

            // ����������� ���������� ������.
            CConsole::SetColor( clRed, clWhite );

            uint8_t ch, n = 0;

            ch = Keys[i][ n++ ];

            while ( ch != '\0' ) {                                 

                // ����� ����������� ������ ���� ��� �������� ��������.
                if ( ch == '|' ) { 

                    CConsole::SetForegroundColor( clBlack );

                } else {

                    CConsole::PutChar( ch, CConsole::cp1251 );
                }

                ch = Keys[i][ n++ ];

            }

        }

    }

    // ��������� ������ ������������ �� ����� ������.
    CConsole::ClearLine();

}


LRESULT CFileManager::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    switch ( uMsg ) {

        // ����� ����� ������� �����-������ ������.
        // ��� ������� ������ ���� ��������.
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


void CFileManager::FormActivate() {

    Initialization();

    CConsole::CursorOff();
    CConsole::SetColor( clLightGray, clBlack );
    
    CConsole::ClearScreen();
    CConsole::MoveTo( 1, 1 );

    DrawMainMenu();

    DrawPanel( LeftPanel );
    DrawPanel( RightPanel );

    DrawFunctionKeys( Keys );

    // ������� ����������� ��������� ������.
    DrawCommandLine( * pCurrentPanel );

    CConsole::SetForegroundColor( clLightGray );

};


void CFileManager::FormKeyDown( uint16_t Key ) {

    uint8_t tmp;

    switch ( Key ) {

        case VK_ESCAPE: { 
            
            CPLC::SetActiveWindow( HWND_COMMAND_SHELL );
            break;
        }

        case VK_RETURN: {

            // ���� ��������� ������ �� ������, �� ������� ���������� �� ����������.
            tmp = strlen( CommandString );

            if ( tmp > 0 ) {

                CommandString[0] = 0;

                // ������� ����������� ��������� ������.
                DrawCommandLine( * pCurrentPanel );

                break;
            }

            // ���� ������ - �����.
            if ( pCurrentPanel->FileInfo.fattrib & AM_DIR ) {

                // ������������ � ���� ��� ���������� �����.
                if ( strcmp( pCurrentPanel->Path, szROOT ) != 0 ) strcat( pCurrentPanel->Path, szROOT );

                strcat( pCurrentPanel->Path, pCurrentPanel->FileInfo.fname );

                pCurrentPanel->ItemIndex = 0;

                CConsole::CursorOff();

                DrawPanel( * pCurrentPanel );

                // ������� ����������� ��������� ������.
                DrawCommandLine( * pCurrentPanel );

                // ���� ������ - ����.
                } else {


                    #ifdef __GNUC__

                        if ( strstr_P( pCurrentPanel->FileInfo.fname, ( PGM_P ) & SPSTR( ".TXT" ) ) != NULL ) {

                    #elif defined( __ICCAVR__ )

                        if ( strstr_P( pCurrentPanel->FileInfo.fname, SPSTR( ".TXT" ) ) != NULL ) {

                    #endif
                            CPLC::SetActiveWindow( HWND_VIEWER );

                    #ifdef __GNUC__

                        } else if ( strstr_P( pCurrentPanel->FileInfo.fname, ( PGM_P ) & SPSTR( ".WAV" ) ) != NULL ) {

                    #elif defined( __ICCAVR__ )

                        } else if ( strstr_P( pCurrentPanel->FileInfo.fname, SPSTR( ".WAV" ) ) != NULL ) {

                    #endif
                            CPLC::SetActiveWindow( HWND_PLAYER );

                        }

                }

            break;

        }

        case VK_BACK: {

            tmp = strlen( CommandString );

            // ������� ���������� ������, ���� ����� ��������� ������ �� ����.
            if ( tmp > 0 ) {

                CConsole::PutChar( VK_BACK );
                CConsole::ClearForward(1);

                CommandString[ --tmp ] = 0;

            // ���� ����� ����, �� ��������� � ������� �����.
            } else {

                tmp = strlen( pCurrentPanel->Path );

                while ( ( pCurrentPanel->Path[ tmp-- ] != '/' ) && ( tmp > 0 ) );

                pCurrentPanel->Path[ tmp + 1 ] = 0;

                pCurrentPanel->ItemIndex = 0;

                CConsole::CursorOff();

                DrawPanel( * pCurrentPanel );

                // ������� ����������� ��������� ������.
                DrawCommandLine( * pCurrentPanel );

            }

            break;

        }

        case VK_LEFT: { break; }
        
        case VK_RIGHT: { break; }

        case VK_UP: {

            if ( pCurrentPanel->ItemIndex != 0 ) {

                pCurrentPanel->ItemIndex--;

                CConsole::CursorOff();
                CConsole::SaveCursor();
                DrawPanel( * pCurrentPanel );
                CConsole::RestoreCursor();
                CConsole::CursorOn();

            }

            break;

        }

        case VK_DOWN: {

            if ( pCurrentPanel->ItemIndex < pCurrentPanel->Height - 1 ) {

                pCurrentPanel->ItemIndex++;

                CConsole::CursorOff();
                CConsole::SaveCursor();
                DrawPanel( * pCurrentPanel );
                CConsole::RestoreCursor();
                CConsole::CursorOn();

            }

            break;

        }

        case VK_HOME: {

            pCurrentPanel->ItemIndex = 0;

            CConsole::CursorOff();
            CConsole::SaveCursor();
            DrawPanel( * pCurrentPanel );
            CConsole::RestoreCursor();
            CConsole::CursorOn();

            break;

        }

        case VK_END: {

            pCurrentPanel->ItemIndex = pCurrentPanel->ItemsCount - 1;

            CConsole::CursorOff();
            CConsole::SaveCursor();
            DrawPanel( * pCurrentPanel );
            CConsole::RestoreCursor();
            CConsole::CursorOn();

            break;

        }

        case VK_TAB: {

            CConsole::CursorOff();

            if ( pCurrentPanel == & LeftPanel ) {

                pCurrentPanel = & RightPanel;
                HightlightPanel( LeftPanel );

            } else {

                pCurrentPanel = & LeftPanel;
                HightlightPanel( RightPanel );
            }

            HightlightPanel( * pCurrentPanel );

            // ������� ����������� ��������� ������.
            DrawCommandLine( * pCurrentPanel );

            break;

        }

        default: {

            // ������� ������ �� �����.
            CConsole::PutChar( ( uint8_t ) Key );

            tmp = strlen( CommandString );
            CommandString[ tmp++ ] = ( uint8_t ) Key;
            CommandString[ tmp ] = 0;

        }

    } // switch

}


void CFileManager::Form10msTimer() {

};


void CFileManager::Form100msTimer() {

};

void CFileManager::Form500msTimer() {

};


void CFileManager::Form1secTimer() {

};


void CFileManager::Form5secTimer() {

};

