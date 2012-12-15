#pragma once


class CPlayer {

private:  

    inline static DWORD LoadHead();
    inline static FRESULT Play ( const char * fn );
    inline static void FormActivate();
    inline static void FormKeyDown( uint16_t Key );

public:

    static LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};
