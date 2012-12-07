#include "Defines.h"
#include "Configuration.h"
#include "Version.h"
#include "Console.h"
#include "FAT.h"
#include "MessagesQueue.h"
#include "FileManager.h"


// -=[ Внешние ссылки ]=-

extern MSG EmptyMsg;

extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

FRESULT res;
UFDATE FDate;
UFTIME FTime;
FILINFO fno;
DIR dir;
FATFS fs;

char Version[16]; // Версия программы
char buffer[16];
char CommandString[128];


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

CKeys Keys;
CPanel LeftPanel;
CPanel RightPanel;

HWND CFileManager::hwndMain = HWND_MAIN_SCREEN;
CPanel * CFileManager::pCurrentPanel = & LeftPanel;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


/**
 * Отрисовка окна с рамкой
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

    // Вычисление строки с версией программы
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

    // Очищаем командрую строку.
    CommandString[0] = 0;

}


/**
 * Отрисовка заголовка окна
 */
void CFileManager::DrawMainMenu() {

    CConsole::SetTextAttributes( atOff );        
    CConsole::MoveTo( 1, 1 );

    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clWhite );
    CConsole::WriteString( SPSTR( "Файловый менеджер, ATmega16 @ 16 МГц, версия сборки: " ), CConsole::cp1251 );
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

    // Заголовки колонок.
    CConsole::MoveTo( Panel.Left + 5, 3 );

    CConsole::WriteString( SPSTR( "Имя" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 8 );

    CConsole::WriteString( SPSTR( "Размер" ), CConsole::cp1251 );

    CConsole::Move( mdForward, 5 );

    CConsole::WriteString( SPSTR( "Дата   Время" ), CConsole::cp1251 );

    CConsole::SetForegroundColor( clLightGray );    

    // Добавляем стыкующие элементы.
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

    // Монтирование
    res = CFAT::Mount( & fs );

    // Открываем директорию (папку)
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

            // Если объект - папка
            if ( fno.fattrib & AM_DIR ) {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGreen );
                }

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                len = 12 - strlen( fno.fname );

                while ( len-- ) CConsole::PutChar( ' ' );

                CConsole::MoveTo( Panel.Left + 14, i );

                CConsole::WriteString( SPSTR( "  [Папка] " ), CConsole::cp1251 );

            // Если объект - файл
            } else {

                if ( !( ( & Panel == pCurrentPanel ) 
                    && ( Panel.Line == ( i - Panel.Top - 2 ) ) ) ) {
                    
                    CConsole::SetForegroundColor( clLightGray );
                }

                // Имя
                CConsole::WriteString( fno.fname, CConsole::cp1251 );

                len = 12 - strlen( fno.fname );

                while ( len-- ) CConsole::PutChar( ' ' );

                CConsole::MoveTo( Panel.Left + 14, i );

                len = 9 - strlen( utoa_fast_div( fno.fsize, buffer ) );

                do CConsole::PutChar( ' ' ); while ( len-- );

                CConsole::WriteString( utoa_fast_div( fno.fsize, buffer ) );

            }

            CConsole::MoveTo( Panel.Left + 25, i );

            // Вывод даты
            FDate.Value = fno.fdate;

            // День
            CConsole::PutChar( FDate.fdate.Day / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Day % 10 + '0' );
            CConsole::PutChar( '.' );

            // Месяц
            CConsole::PutChar( FDate.fdate.Month / 10 + '0' );
            CConsole::PutChar( FDate.fdate.Month % 10 + '0' );
            CConsole::PutChar( '.' );

            // Год
            uint16_t tmp = 1980U + FDate.fdate.Year;

            tmp %= 1000U;
            tmp %= 100;

            CConsole::PutChar( ( uint8_t ) ( tmp / 10 ) + '0' );
            CConsole::PutChar( ( uint8_t ) ( tmp % 10 ) + '0' );

            CConsole::Move( mdForward, 1 );

            // Вывод времени
            FTime.Value = fno.ftime;

            // Часы
            CConsole::PutChar( FTime.ftime.Hour / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Hour % 10 + '0' );

            CConsole::PutChar( ':' );

            // Минуты
            CConsole::PutChar( FTime.ftime.Minute / 10 + '0' );
            CConsole::PutChar( FTime.ftime.Minute % 10 + '0' );

        } // for

    } // if

    // Отмонтируем
    res = CFAT::Mount( NULL );

}


void CFileManager::DrawCommandLine( CPanel & Panel ) {

    // Выводим приглашение
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( clLightGreen );

    CConsole::MoveTo( 1, 24 );
    
    CConsole::PutChar( '[' );
    CConsole::WriteString( Panel.Path );
    CConsole::WriteString( SPSTR( "]$ " ) );

    CConsole::CursorOn();

}


/**
 * Отображаем линейку функциональных клавиш.
 */
void CFileManager::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // Перебираем функциональные клавиши.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetForegroundColor( clRed );
            CConsole::SetBackgroundColor( clWhite );
            
            // Разделитель
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // Разделитель
            CConsole::PutChar( ' ' );

            CConsole::SetForegroundColor( clBlack );
            CConsole::WriteString( Keys[i], CConsole::cp1251 );

        }

    }

    // Заполняем пустое пространство до конца строки.
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
 * Получить дискриптор активного окна
 */
inline HWND CFileManager::GetActiveWindow() {

	return hwndActiveWindow;

}


/**
 * Сделать окно активным
 */
HWND CFileManager::SetActiveWindow( HWND hWnd ) {

    HWND hWndOld = hwndActiveWindow;

    PostMessage( hWndOld, WM_ACTIVATE, WA_INACTIVE, hWnd );

    PostMessage( hWnd, WM_ACTIVATE, WA_ACTIVE, hWndOld );
	
    hwndActiveWindow = hWnd;

    return  hWndOld;
}


bool CFileManager::GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) {

    // Достаём очередное сообщение из текущей очереди
    pMsgsQueue->get( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );

	return true;

}


LRESULT CFileManager::DispatchMessage( const MSG * lpMsg ) {

    if ( lpMsg->hwnd == HWND_MAIN_SCREEN ) {

	    return WindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

    } else {

        // Если сообщение ни кем не получено, всё равно обрабатываем его
	    return DefWindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
    }

}


bool CFileManager::TranslateMessage( const MSG * lpMsg ) {

    // Здесь можно отфильтровать сообщения

	return false;

}


bool CFileManager::PostMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    UserInterfaceMessage.hwnd = hwnd;
    UserInterfaceMessage.message = uMsg;
    UserInterfaceMessage.wParam = wParam;
    UserInterfaceMessage.lParam = lParam;

    // Вызываем функцию для укладки сообщения в очередь.
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

    // Обновляем системное время, если доступны внешние данные

    // Формируем сообщение
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
 * Клавиша нажата
 */
void CFileManager::DoKeyDown( uint8_t KeyCode, uint8_t KeyData ) {

    KeyboardMessage.hwnd = GetActiveWindow();   // handle to window
    KeyboardMessage.message = WM_KEYDOWN;       // WM_KEYDOWN
    KeyboardMessage.wParam = KeyCode;           // virtual-key code
    KeyboardMessage.lParam = KeyData;           // key data (просто скан-код)

    ProcessMessages();

}


void CFileManager::ProcessMessages() {

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


LRESULT CFileManager::DefWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    // Всё, что не обработано в интерфейсе, попадает сюда

	return  NULL;

}


LRESULT CFileManager::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

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

    // Выводим приглашение командной строки.
    DrawCommandLine( * pCurrentPanel );

    CConsole::SetForegroundColor( clLightGray );

};


void CFileManager::FormKeyDown( uint16_t Key ) {

    uint8_t tmp;

    switch ( Key ) {

        case VK_ESCAPE: { break; }

        case VK_BACK: { 

            tmp = strlen( CommandString );

            // Удаляем предыдущий символ, если буфер командной строки не пуст.
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

            // Выводим символ на экран.
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
 * Аналог WinMain в Windows
 */
int CFileManager::WinMain() {

    // Посылаем событие SW_SHOW
    ShowWindow( hwndMain, SW_SHOW );

    // Посылаем событие WM_PAINT окну, чтобы оно себя перерисовало
    UpdateWindow( hwndMain );

    // Разрешаем прерывания
    __enable_interrupt();

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