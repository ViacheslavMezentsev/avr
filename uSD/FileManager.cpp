#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "MessagesQueue.h"
#include "FileManager.h"


// -=[ ������� ������ ]=-

extern MSG EmptyMsg;

extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-

FRESULT res;
UFDATE FDate;
UFTIME FTime;
FILINFO fno;
DIR dir;
FATFS fs;

char Version[16]; // ������ ���������
char buffer[16];
char CommandString[128];


// ���������� ��������� ����
HWND hwndActiveWindow;

// ��������� "�����" ��� �������� �������������� ���������
// ����� ���������� � ����� �������. ��� ���������� ���
// �������� ������ �� ����������� ����������� ����������
// � �������, ������� ���������� �� ���������� � ��������
// ������� ���������
MSG UserInterfaceMessage;
MSG TimerCounter0OverflowMessage;
MSG KeyboardMessage;

MSG msg;

// ������ ��������� ������� ���������
CMessagesQueue MessagesQueue;

// ��������� ��� ������ � ��������� ��������� ��� �� ������������
CMessagesQueue * pMsgsQueue;

CKeys Keys;
CPanel LeftPanel;
CPanel RightPanel;

HWND CFileManager::hwndMain = HWND_MAIN_SCREEN;
CPanel * CFileManager::pCurrentPanel = & LeftPanel;


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * ��������� ���� � ������
 */
void CFileManager::DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor ) {

    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( Color );
    CConsole::SetBackgroundColor( bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    for ( uint8_t i = 0; i < Width; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );
    
    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );

        for ( uint8_t i = 0; i < Width; i++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ACS_DBL_VLINE );

    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );
    
}


void CFileManager::Initialization() {

    FLASHSTR_DECLARE( char, initpath, "/" );

    char szDot[] = ".";

    // ���������� ������ � ������� ���������
    strcat( Version, utoa_fast_div( CVersion::GetMajor(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetMinor(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetRevision(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetBuild(), buffer ) );

    pMsgsQueue = & MessagesQueue;

    LeftPanel.Left = 1;
    LeftPanel.Top = 2;
    LeftPanel.Width = 38;
    LeftPanel.Height = 20;
    LeftPanel.Line = 0;

    RightPanel.Left = 41;
    RightPanel.Top = 2;
    RightPanel.Width = 38;
    RightPanel.Height = 20;
    RightPanel.Line = 0;

    for ( uint8_t i = 0; i < 127; i++ ) {
        
        if ( initpath[i] == 0 ) {
            
            LeftPanel.Path[i] = 0;
            RightPanel.Path[i] = 0;
            break;
        }

        LeftPanel.Path[i] = initpath[i];
        RightPanel.Path[i] = initpath[i];
    }

    // ������� ��������� ������.
    CommandString[0] = 0;

}


/**
 * ��������� ��������� ����
 */
void CFileManager::DrawMainMenu() {

    CConsole::SetTextAttributes( atOff );        
    CConsole::MoveTo( 1, 1 );

    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clWhite );
    CConsole::WriteString( SPSTR( "�������� ��������, ATmega16 @ 16 ���, ������ ������: " ), CConsole::cp1251 );
    CConsole::WriteString( Version );

#ifdef __GNUC__
    CConsole::WriteString( SPSTR( " (gnu)" ) );
#elif defined __ICCAVR__
    CConsole::WriteString( SPSTR( " (iar)" ) );
#endif

    CConsole::ClearEndOfLine();

}


void CFileManager::DrawPanel( CPanel & Panel ) {
       
    uint8_t len;

    DrawFrame( Panel.Left, Panel.Top, Panel.Width, Panel.Height, clLightGray, clBlue );

    len = Panel.Width - strlen( utoa_fast_div( fno.fsize, buffer ) );

    len /= 2;

    CConsole::MoveTo( Panel.Left + len, 2 );
    
    CConsole::PutChar( ' ' );
    
    if ( & Panel == pCurrentPanel ) {        
    
        CConsole::SetTextAttributes( atOff );
        CConsole::SetForegroundColor( clBlack );
        CConsole::SetBackgroundColor( clWhite );

        CConsole::WriteString( pCurrentPanel->Path, CConsole::cp1251 );

        CConsole::SetForegroundColor( clLightGray );
        CConsole::SetBackgroundColor( clBlue );

    } else {

        CConsole::WriteString( pCurrentPanel->Path, CConsole::cp1251 );
    }

    CConsole::PutChar( ' ' );

    CConsole::SetForegroundColor( clLightYellow );

    // ��������� �������.
    CConsole::MoveTo( Panel.Left + 5, 3 );

    CConsole::WriteString( SPSTR( "���" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "������" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 5 );

    CConsole::WriteString( SPSTR( "����   �����" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

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

    for ( uint8_t i = Panel.Top + 1; i < Panel.Top + Panel.Height + 1; i++ ) {

        CConsole::MoveTo( Panel.Left + 13, i );

        if ( & Panel == pCurrentPanel ) {
        
            if ( Panel.Line == ( i - Panel.Top - 2 ) ) {        
        
                CConsole::SetTextAttributes( atOff );
                CConsole::SetForegroundColor( clBlack );
                CConsole::SetBackgroundColor( clWhite );

            } else {

                CConsole::SetForegroundColor( clLightGray );
                CConsole::SetBackgroundColor( clBlue );
            }

        }

        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 10 );

        CConsole::PutChar( ACS_VLINE );

        CConsole::Move( mdForward, 8 );

        CConsole::PutChar( ACS_VLINE );

    }

    // ������������
    res = CFAT::Mount( & fs );

    // ��������� ���������� (�����)
    res = CFAT::OpenDir( & dir, Panel.Path );

    if ( res == FR_OK ) {

        for ( uint8_t i = Panel.Top + 2; i < Panel.Top + Panel.Height + 1; i++ ) {

            CConsole::MoveTo( Panel.Left + 1, i );

            res = CFAT::ReadDir( & dir, & fno );

            if ( res != FR_OK || fno.fname[0] == 0 ) break;

            if ( & Panel == pCurrentPanel ) {
            
                if ( Panel.Line == ( i - Panel.Top - 2 ) ) {        
            
                    CConsole::SetTextAttributes( atOff );
                    CConsole::SetForegroundColor( clBlack );
                    CConsole::SetBackgroundColor( clWhite );

                } else {

                    CConsole::SetForegroundColor( clLightGray );
                    CConsole::SetBackgroundColor( clBlue );
                }

            }

            // ���� ������ - �����
            if ( fno.fattrib & AM_DIR ) {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGreen );
                }

                // ���
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                len = 12 - strlen( fno.fname );

                while ( len-- ) CConsole::PutChar( ' ' );

                CConsole::MoveTo( Panel.Left + 14, i );

                CConsole::WriteString( SPSTR( "  [�����] " ), CConsole::cp1251 );

            // ���� ������ - ����
            } else {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGray );
                }

                // ���
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                len = 12 - strlen( fno.fname );

                while ( len-- ) CConsole::PutChar( ' ' );

                CConsole::MoveTo( Panel.Left + 14, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

            CConsole::MoveTo( Panel.Left + 25, i );

            // ����� ����
            FDate.Value = fno.fdate;

            // ����
            CConsole::PutChar( FDate.fdate.Day / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Day % 10 + '0' );
            CConsole::PutChar( '.' );

            // �����
            CConsole::PutChar( FDate.fdate.Month / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Month % 10 + '0' );
            CConsole::PutChar( '.' );

            // ���
            uint16_t tmp = 1980U + FDate.fdate.Year;

            tmp %= 1000U;
            tmp %= 100;

            CConsole::PutChar( ( uint8_t ) ( tmp / 10 ) + '0' );
            CConsole::PutChar( ( uint8_t ) ( tmp % 10 ) + '0' );

            CConsole::Move( mdForward, 1 );

            // ����� �������
            FTime.Value = fno.ftime;

            // ����
            CConsole::PutChar( FTime.ftime.Hour / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Hour % 10 + '0' );

            CConsole::PutChar( ':' );

            // ������
            CConsole::PutChar( FTime.ftime.Minute / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Minute % 10 + '0' );

        } // for

    } // if

    // �����������
    res = CFAT::Mount( NULL );

}


void CFileManager::DrawCommandLine( CPanel & Panel ) {

    // ������� �����������
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( clLightGreen );

    CConsole::MoveTo( 1, 24 );
    
    CConsole::PutChar( '[' );
    CConsole::WriteString( Panel.Path );
    CConsole::WriteString( SPSTR( "]$ " ) );

    CConsole::CursorOn();

}


/**
 * ���������� ������� �������������� ������.
 */
void CFileManager::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // ���������� �������������� �������.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetForegroundColor( clRed );
            CConsole::SetBackgroundColor( clWhite );
            
            // �����������
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // �����������
            CConsole::PutChar( ' ' );

            CConsole::SetForegroundColor( clBlack );
            CConsole::WriteString( Keys[i], CConsole::cp1251 );

        }

    }

    // ��������� ������ ������������ �� ����� ������.
    CConsole::ClearEndOfLine();

}


bool CFileManager::ShowWindow( HWND hWnd, uint8_t nCmdShow ) {

    PostMessage( hWnd, nCmdShow, 0, 0 );

    SetActiveWindow( hWnd );
	
    return true;

}


bool CFileManager::UpdateWindow( HWND hWnd ) {

	return PostMessage( hWnd, WM_PAINT, 0, 0 );

}


// The Sleep function suspends the execution of the current thread
// for a specified interval.
void CFileManager::Sleep (DWORD dwMilliseconds ) {

    while( dwMilliseconds-- ) _delay_ms(1);

}


/**
 * �������� ���������� ��������� ����
 */
inline HWND CFileManager::GetActiveWindow() {

	return hwndActiveWindow;

}


/**
 * ������� ���� ��������
 */
HWND CFileManager::SetActiveWindow( HWND hWnd ) {

    HWND hWndOld = hwndActiveWindow;

    PostMessage( hWndOld, WM_ACTIVATE, WA_INACTIVE, hWnd );

    PostMessage( hWnd, WM_ACTIVATE, WA_ACTIVE, hWndOld );
	
    hwndActiveWindow = hWnd;

    return  hWndOld;
}


bool CFileManager::GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) {

    // ������ ��������� ��������� �� ������� �������
    pMsgsQueue->get( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );

	return true;

}


LRESULT CFileManager::DispatchMessage( const MSG * lpMsg ) {

    if ( lpMsg->hwnd == HWND_MAIN_SCREEN ) {

	    return WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else {

        // ���� ��������� �� ��� �� ��������, �� ����� ������������ ���
	    return DefWindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
    }

}


bool CFileManager::TranslateMessage( const MSG * lpMsg ) {

    // ����� ����� ������������� ���������

	return false;

}


bool CFileManager::PostMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    UserInterfaceMessage.hwnd = hwnd;
    UserInterfaceMessage.message = uMsg;
    UserInterfaceMessage.wParam = wParam;
    UserInterfaceMessage.lParam = lParam;

    // �������� ������� ��� ������� ��������� � �������.
    ProcessMessages();

	return true;

}


void CFileManager::DoTimer10ms(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_10MS;    // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}

void CFileManager::DoTimer100ms(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_100MS;   // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


void CFileManager::DoTimer500ms(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_500MS;   // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


void CFileManager::DoTimer1s(){

    // ��������� ��������� �����, ���� �������� ������� ������

    // ��������� ���������
    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_1S;      // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


void CFileManager::DoTimer5s(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_5S;      // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


/**
 * ������� ������
 */
void CFileManager::DoKeyDown( uint8_t KeyCode, uint8_t KeyData ) {

    KeyboardMessage.hwnd = GetActiveWindow();   // handle to window
    KeyboardMessage.message = WM_KEYDOWN;       // WM_KEYDOWN
    KeyboardMessage.wParam = KeyCode;           // virtual-key code
    KeyboardMessage.lParam = KeyData;           // key data (������ ����-���)

    ProcessMessages();

}


void CFileManager::ProcessMessages() {

    // ��������� ��� ��������� � ��������� �� ������
    if ( UserInterfaceMessage.hwnd != 0 ) {

        pMsgsQueue->put( & UserInterfaceMessage );

        UserInterfaceMessage = EmptyMsg;

        return;
    }

    if ( TimerCounter0OverflowMessage.hwnd != 0 ) {

        pMsgsQueue->put( & TimerCounter0OverflowMessage );

        TimerCounter0OverflowMessage = EmptyMsg;

        return;
    }

    if ( KeyboardMessage.hwnd != 0 ) {

        pMsgsQueue->put( & KeyboardMessage );

        KeyboardMessage = EmptyMsg;

        return;
    }

}


LRESULT CFileManager::DefWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    // ��, ��� �� ���������� � ����������, �������� ����

	return  NULL;

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

    CConsole::CursorOff();

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( clBlack );
    CConsole::SetForegroundColor( clLightGray );
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

        case VK_ESCAPE: { break; }

        case VK_BACK: { 

            tmp = strlen( CommandString );

            // ������� ���������� ������, ���� ����� ��������� ������ �� ����.
            if ( tmp > 0 ) {
            
                CConsole::PutChar( VK_BACK );
                CConsole::PutChar( ' ' );
                CConsole::PutChar( VK_BACK );
                 
                CommandString[ --tmp ] = 0;

            }

            break; 

        }

        case VK_UP: { 
        
            if ( pCurrentPanel->Line != 0 ) {

                pCurrentPanel->Line--;
                
                CConsole::SaveCursor();
                DrawPanel( * pCurrentPanel );
                CConsole::RestoreCursor();

            }

            break; 

        }

        case VK_DOWN: { 
        
            if ( pCurrentPanel->Line < pCurrentPanel->Height ) {
                
                pCurrentPanel->Line++;

                CConsole::SaveCursor();
                DrawPanel( * pCurrentPanel );
                CConsole::RestoreCursor();

            }

            break; 

        }

        case VK_TAB: { 

            CConsole::SaveCursor();

            if ( pCurrentPanel == & LeftPanel ) {

                pCurrentPanel = & RightPanel;

            } else {
        
                pCurrentPanel = & LeftPanel;
            }
                
            DrawPanel( LeftPanel );
            DrawPanel( RightPanel );

            CConsole::RestoreCursor();

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


/**
 * ������ WinMain � Windows
 */
int CFileManager::WinMain() {

    // �������� ������� SW_SHOW
    ShowWindow( hwndMain, SW_SHOW );

    // �������� ������� WM_PAINT ����, ����� ��� ���� ������������
    UpdateWindow( hwndMain );

    // ��������� ����������
    __enable_interrupt();

    // ���� ��������� ���������
    while ( GetMessage( & msg, ( HWND ) NULL, 0, 0 ) ) {

        // ������� ��������, ���������������� ������ ���������
        TranslateMessage( & msg );

        // "���������" ������ ����� �� �����
        DispatchMessage( & msg );

    }

    // ���������� ��� ����������
    
    return msg.wParam;

}