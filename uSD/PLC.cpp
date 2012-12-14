///////////////////////////////////////////////////////////
//  PLC.cpp
//  Implementation of the Class CPLC
//  Created on:      18-июл-2011 14:40:05
//  Original author: Слава
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


// -=[ Пользовательские типы ]=-

struct divmod10_t {

    uint32_t quot;
    uint8_t rem;
};


// -=[ Внешние ссылки ]=-

extern MSG EmptyMsg;


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

char Version[ 16 ]; // Версия программы
char buffer[ 16 ];

// Дискриптор активного окна
HWND hwndActiveWindow;

// Временные "ящики" для хранения промежуточного сообщения
// перед попаданием в общую очередь. Они необходимы для
// передачи данных из обработчика конкретного прерывания
// в функцию, которая укладывает их содержимое в буферную
// очередь сообщений
MSG UserInterfaceMessage;
MSG TimerCounter0OverflowMessage;
MSG KeyboardMessage;

MSG msg;

// Создаём экземпляр очереди сообщений
CMessagesQueue MessagesQueue;

// Указатель для работы с очередями сообщений при их переключении
CMessagesQueue * pMsgsQueue;

HWND CPLC::hwndMain = HWND_MAIN_SCREEN;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


divmod10_t divmodu10( uint32_t n ) {

    divmod10_t res;

    // умножаем на 0.8
    res.quot = n >> 1;
    res.quot += res.quot >> 1;
    res.quot += res.quot >> 4;
    res.quot += res.quot >> 8;
    res.quot += res.quot >> 16;
    uint32_t qq = res.quot;

    // делим на 8
    res.quot >>= 3;

    // вычисляем остаток
    res.rem = uint8_t( n - ( ( res.quot << 1 ) + ( qq & ~7ul ) ) );

    // корректируем остаток и частное
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
 * Начальная настройка всех узлов модуля
 */
void CPLC::Initialization(){

    // Инициализация после сброса
    // запрещаем все прерывания
    __disable_interrupt();

    char szDot[] = ".";

    // Вычисление строки с версией программы
    strcat( Version, utoa_fast_div( CVersion::GetMajor(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetMinor(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetRevision(), buffer ) );
    strcat( Version, szDot );

    strcat( Version, utoa_fast_div( CVersion::GetBuild(), buffer ) );

    // Инициализируем указатель текущей очереди
    pMsgsQueue = & MessagesQueue;

    // Инициализация микроконтроллера
    CMCU::Initialization();

    // Инициализация файлового менеджера.
    CFileManager::Initialization();

}


/**
 * Запуск модуля
 */
void CPLC::Run(){

    // разрешаем прерывания
    __enable_interrupt();

    // Запускаем основной поток интерфейса модуля
    WinMain();

}


/**
 * Аналог WinMain в Windows
 */
int CPLC::WinMain() {

    // Посылаем событие SW_SHOW
    ShowWindow( hwndMain, SW_SHOW );

    // Посылаем событие WM_PAINT окну, чтобы оно себя перерисовало
    UpdateWindow( hwndMain );

    // Цикл обработки сообщений
    while ( GetMessage( & msg, ( HWND ) NULL, 0, 0 ) ) {

        // Цепочка фильтров, преобразователей потока сообщений
        TranslateMessage( & msg );

        // "Почтальон" раздаёт почту по окнам
        DispatchMessage( & msg );

    }

    // Возвращаем код завершения
    
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
 * Получить дискриптор активного окна
 */
inline HWND CPLC::GetActiveWindow() {

	return hwndActiveWindow;

}


/**
 * Сделать окно активным
 */
HWND CPLC::SetActiveWindow( HWND hWnd ) {

    HWND hWndOld = hwndActiveWindow;

    PostMessage( hWndOld, WM_ACTIVATE, WA_INACTIVE, hWnd );

    PostMessage( hWnd, WM_ACTIVATE, WA_ACTIVE, hWndOld );
	
    hwndActiveWindow = hWnd;

    return  hWndOld;
}


bool CPLC::GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) {

    // Достаём очередное сообщение из текущей очереди
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

        // Если сообщение ни кем не получено, всё равно обрабатываем его
	    return DefWindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
    }

}


bool CPLC::TranslateMessage( const MSG * lpMsg ) {

    // Здесь можно отфильтровать сообщения

	return false;

}


bool CPLC::PostMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    UserInterfaceMessage.hwnd = hwnd;
    UserInterfaceMessage.message = uMsg;
    UserInterfaceMessage.wParam = wParam;
    UserInterfaceMessage.lParam = lParam;

    // Вызываем функцию для укладки сообщения в очередь.
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
 * Клавиша нажата
 */
void CPLC::DoKeyDown( uint8_t KeyCode, uint8_t KeyData ) {

    KeyboardMessage.hwnd = GetActiveWindow();   // handle to window
    KeyboardMessage.message = WM_KEYDOWN;       // WM_KEYDOWN
    KeyboardMessage.wParam = KeyCode;           // virtual-key code
    KeyboardMessage.lParam = KeyData;           // key data (просто скан-код)

    ProcessMessages();

}


void CPLC::ProcessMessages() {

    // Проверяем все сообщения и добавляем не пустое
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

    // Всё, что не обработано в интерфейсе, попадает сюда

	return  NULL;

}