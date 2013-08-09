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
#include "CommandShell.h"
#include "Game.h"
#include "PLC.h"


// -=[ ���������������� ���� ]=-


// -=[ ������� ������ ]=-
extern MSG EmptyMsg;
extern CGame Game;

// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-
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

HWND CPLC::hwndMain = HWND_COMMAND_SHELL;


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * ��������� ��������� ���� ����� ������.
 */
void CPLC::Initialization(){

    __disable_interrupt();

    // �������������� ��������� ������� �������.
    pMsgsQueue = & MessagesQueue;

    // ������������� ����������������.
    CMCU::Initialization();

    // ������������� ���������� ��������������.
    CCommandShell::Initialization();

}


/**
 * ������ ������.
 */
void CPLC::Run(){

    __enable_interrupt();

    // ��������� �������� ����� ���������� ������.
    WinMain();

}


/**
 * ������ WinMain � Windows.
 */
int CPLC::WinMain() {

    // �������� ������� SW_SHOW.
    ShowWindow( hwndMain, SW_SHOW );

    // �������� ������� WM_PAINT ����, ����� ��� ���� ������������.
    UpdateWindow( hwndMain );

    // ���� ��������� ���������.
    while ( GetMessage( & msg, ( HWND ) NULL, 0, 0 ) ) {

        // ������� ��������, ���������������� ������ ���������.
        TranslateMessage( & msg );

        // "���������" ������ ����� �� �����.
        DispatchMessage( & msg );

    }

    // ���������� ��� ����������.
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
 * �������� ���������� ��������� ����.
 */
inline HWND CPLC::GetActiveWindow() {

	return hwndActiveWindow;

}


/**
 * ������� ���� ��������.
 */
HWND CPLC::SetActiveWindow( HWND hWnd ) {

    HWND hWndOld = hwndActiveWindow;

    PostMessage( hWndOld, WM_ACTIVATE, WA_INACTIVE, hWnd );

    PostMessage( hWnd, WM_ACTIVATE, WA_ACTIVE, hWndOld );
	
    hwndActiveWindow = hWnd;

    return  hWndOld;
}


bool CPLC::GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) {

    // ������ ��������� ��������� �� ������� �������.
    pMsgsQueue->get( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );

	return true;

}


LRESULT CPLC::DispatchMessage( const MSG * lpMsg ) {

    if ( lpMsg->hwnd == HWND_COMMAND_SHELL ) {

	    return CCommandShell::WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else if ( lpMsg->hwnd == HWND_GAME ) {

        return Game.WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else {

        // ���� ��������� �� ��� �� ��������, �� ����� ������������ ���.
	    return DefWindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
    }

}


bool CPLC::TranslateMessage( const MSG * lpMsg ) {

    // ����� ����� ������������� ���������.

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


void CPLC::DoTimer500ms(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_500MS;   // timer identifier
    TimerCounter0OverflowMessage.lParam = 0;                // timer callback (TIMERPROC)

    ProcessMessages();

}


void CPLC::DoTimer1s(){

    TimerCounter0OverflowMessage.hwnd = GetActiveWindow();  // handle to window
    TimerCounter0OverflowMessage.message = WM_TIMER;        // WM_TIMER
    TimerCounter0OverflowMessage.wParam = ID_TIMER_1S;   // timer identifier
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

    // ��������� ��� ��������� � ��������� �� ������.
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

    // ��, ��� �� ���������� � ����������, �������� ����.

	return  NULL;

}