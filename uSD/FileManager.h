#pragma once


class CKeys {
   
public:	

    inline FCHAR_PTR operator []( uint8_t Key ) {
        
        switch ( Key ) {

            case 0: return SPSTR( "Выход" );
            
            default: return 0;

        }        

    }

};


class CPanel {

public:

    uint8_t Left;
    uint8_t Top;
    uint8_t Width;
    uint8_t Height;
    
    uint8_t Line;

    char Path[ 128 ];

};


class CFileManager {

private:
	static HWND hwndMain;

    inline static void FormActivate();

    static void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor );

    static void DrawMainMenu();
    static void DrawPanel( CPanel & Panel );
    static void DrawCommandLine( CPanel & Panel );
    static void DrawFunctionKeys( CKeys & Keys );

    static bool PostMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void ProcessMessages();
    static bool GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
    static LRESULT DispatchMessage(const MSG * lpMsg );
    static bool TranslateMessage(const MSG * lpMsg );
    static void Sleep(DWORD dwMilliseconds);
    static HWND GetActiveWindow();
    static HWND SetActiveWindow(HWND hWnd);
    static bool ShowWindow(HWND hWnd, uint8_t nCmdShow );
    static bool UpdateWindow( HWND hWnd );

    static LRESULT DefWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline static void FormKeyDown(uint16_t Key);
    inline static void Form10msTimer();
    inline static void Form100msTimer();
    inline static void Form500msTimer();
    inline static void Form1secTimer();
    inline static void Form5secTimer();    

    static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:

    static CPanel * pCurrentPanel;

    static void DoTimer10ms();
    static void DoTimer100ms();
    static void DoTimer500ms();
    static void DoTimer1s();
    static void DoTimer5s();
    static void DoKeyDown(uint8_t KeyCode, uint8_t KeyData);
    
    static void Initialization();
    static int WinMain();

};
