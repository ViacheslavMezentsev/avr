#pragma once


class CViewer {

private:  

    inline static void DrawTitle( char * Caption );
    inline static void FormActivate();
    inline static void FormKeyDown( uint16_t Key );

public:

    static LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};
