///////////////////////////////////////////////////////////
//  PLC.h
//  Implementation of the Class CPLC
//  Created on:      18-июл-2011 14:40:04
//  Original author: Слава
///////////////////////////////////////////////////////////

#if !defined(EA_B7830AFF_EB6B_4088_993E_3DB61A1E727E__INCLUDED_)
#define EA_B7830AFF_EB6B_4088_993E_3DB61A1E727E__INCLUDED_

/**
 * Класс CPCL
 * ~~~~~~~~~~
 *
 * Назначение:
 * Является образом всего устройства.
 *
 * Автор: Мезенцев Вячеслав
 *
 * Почта: mailto:unihomelab@ya.ru
 *
 * ВНИМАНИЕ:
 * Этот файл формируется автоматически в среде EA.
 * Если Вы что-то изменили здесь, то синхронизируйте изменения в EA.
 */
class CPLC {

    public:
        static void Initialization();
        static int WinMain();
        static void Run();
        static bool GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
        static LRESULT DispatchMessage(const MSG * lpMsg );
        static bool TranslateMessage(const MSG * lpMsg );
        static bool ShowWindow(HWND hWnd, uint8_t nCmdShow );
        static bool UpdateWindow( HWND hWnd );
        static LRESULT DefWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static bool PostMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static void ProcessMessages();
        static void Sleep(DWORD dwMilliseconds);
        static void DoTimer10ms();
        static void DoTimer100ms();
        static HWND GetActiveWindow();
        static HWND SetActiveWindow(HWND hWnd);
        static void DoKeyDown(uint8_t KeyCode, uint8_t KeyData);

    private:	    
        static HWND hwndMain;

        inline static void DoProgramInterrupt();

};

 #endif // !defined(EA_B7830AFF_EB6B_4088_993E_3DB61A1E727E__INCLUDED_)
