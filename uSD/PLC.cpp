///////////////////////////////////////////////////////////
//  PLC.cpp
//  Implementation of the Class CPLC
//  Created on:      18-���-2011 14:40:05
//  Original author: �����
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "MessagesQueue.h"
#include "Console.h"
#include "MCU.h"
#include "FileManager.h"
#include "Viewer.h"
#include "Player.h"
#include "PLC.h"


// -=[ ���������������� ���� ]=-

struct divmod10_t {

    uint32_t quot;
    uint8_t rem;
};


// -=[ ������� ������ ]=-

extern MSG EmptyMsg;


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-

char Version[ 16 ]; // ������ ���������
char buffer[ 16 ];

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

HWND CPLC::hwndMain = HWND_MAIN_SCREEN;


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


divmod10_t divmodu10( uint32_t n ) {

    divmod10_t res;

    // �������� �� 0.8
    res.quot = n >> 1;
    res.quot += res.quot >> 1;
    res.quot += res.quot >> 4;
    res.quot += res.quot >> 8;
    res.quot += res.quot >> 16;
    uint32_t qq = res.quot;

    // ����� �� 8
    res.quot >>= 3;

    // ��������� �������
    res.rem = uint8_t( n - ( ( res.quot << 1 ) + ( qq & ~7ul ) ) );

    // ������������ ������� � �������
    if ( res.rem > 9 ) {

        res.rem -= 10;
        res.quot++;
    }

    return res;

}


char * utoa_fast_div( uint32_t value, char * buffer ) {

    buffer += 11;
    * --buffer = 0;

    do {

        divmod10_t res = divmodu10( value );
        * --buffer = res.rem + '0';
        value = res.quot;

    } while ( value != 0 );

    return buffer;

}


/**
 * ��������� ��������� ���� ����� ������
 */
void CPLC::Initialization(){

    // ������������� ����� ������
    // ��������� ��� ����������
    __disable_interrupt();

    char szDot[] = ".";

    // ���������� ������ � ������� ���������
    strcat( Version, utoa_fast_div( CVersion::GetMajor(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetMinor(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetRevision(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetBuild(), buffer ) );

    // �������������� ��������� ������� �������
    pMsgsQueue = & MessagesQueue;

    // ������������� ����������������
    CMCU::Initialization();

    // ������������� ��������� ���������.
    CFileManager::Initialization();

}


/**
 * ������ ������
 */
void CPLC::Run(){

    // ��������� ����������
    __enable_interrupt();

    // ��������� �������� ����� ���������� ������
    WinMain();

}


/**
 * ������ WinMain � Windows
 */
int CPLC::WinMain() {

    // �������� ������� SW_SHOW
    ShowWindow( hwndMain, SW_SHOW );

    // �������� ������� WM_PAINT ����, ����� ��� ���� ������������
    UpdateWindow( hwndMain );

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


bool CPLC::ShowWindow( HWND hWnd, uint8_t nCmdShow ) {

    PostMessage( hWnd, nCmdShow, 0, 0 );

    SetActiveWindow( hWnd );
	
    return true;

}


bool CPLC::UpdateWindow( HWND hWnd ) {

	return PostMessage( hWnd, WM_PAINT, 0, 0 );

}



// The Sleep function suspends the execution of the current thread
// for a specified interval.
void CPLC::Sleep (DWORD dwMilliseconds ) {

    while( dwMilliseconds-- ) _delay_ms(1);

}


/**
 * �������� ���������� ��������� ����
 */
inline HWND CPLC::GetActiveWindow() {

	return hwndActiveWindow;

}


/**
 * ������� ���� ��������
 */
HWND CPLC::SetActiveWindow( HWND hWnd ) {

    HWND hWndOld = hwndActiveWindow;

    PostMessage( hWndOld, WM_ACTIVATE, WA_INACTIVE, hWnd );

    PostMessage( hWnd, WM_ACTIVATE, WA_ACTIVE, hWndOld );
	
    hwndActiveWindow = hWnd;

    return  hWndOld;
}


bool CPLC::GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) {

    // ������ ��������� ��������� �� ������� �������
    pMsgsQueue->get( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );

	return true;

}


LRESULT CPLC::DispatchMessage( const MSG * lpMsg ) {

    if ( lpMsg->hwnd == HWND_MAIN_SCREEN ) {

	    return CFileManager::WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else if ( lpMsg->hwnd == HWND_VIEWER ) {

        return CViewer::WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else if ( lpMsg->hwnd == HWND_PLAYER ) {

        return CPlayer::WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else {

        // ���� ��������� �� ��� �� ��������, �� ����� ������������ ���
	    return DefWindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
    }

}


bool CPLC::TranslateMessage( const MSG * lpMsg ) {

    // ����� ����� ������������� ���������

	return false;

}


bool CPLC::PostMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    UserInterfaceMessage.hwnd = hwnd;
    UserInterfaceMessage.message = uMsg;
    UserInterfaceMessage.wParam = wParam;
    UserInterfaceMessage.lParam = lParam;

    // �������� ������� ��� ������� ��������� � �������.
    ProcessMessages();

	return true;

}


void CPLC::DoTimer10ms(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_10MS;    // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


void CPLC::DoTimer100ms(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_100MS;   // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


/**
 * ������� ������
 */
void CPLC::DoKeyDown( uint8_t KeyCode, uint8_t KeyData ) {

    KeyboardMessage.hwnd = GetActiveWindow();   // handle to window
    KeyboardMessage.message = WM_KEYDOWN;       // WM_KEYDOWN
    KeyboardMessage.wParam = KeyCode;           // virtual-key code
    KeyboardMessage.lParam = KeyData;           // key data (������ ����-���)

    ProcessMessages();

}


void CPLC::ProcessMessages() {

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



LRESULT CPLC::DefWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    // ��, ��� �� ���������� � ����������, �������� ����

	return  NULL;

}