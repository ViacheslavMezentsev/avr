#pragma once

enum EnCommand {

    cmdNone = 0,
    cmdUnknown,
    cmdHelp,
    cmdClearScreen,
    cmdGame

};


class CCommands {

public:   

    EnCommand operator[] ( char * Text );

};


class CCommandShell {

private:  

    inline static void Info();
    inline static void Prompt();

    inline static void FormActivate();
    inline static void FormKeyDown( uint16_t Key );
    inline static void Form10msTimer();
    inline static void Form100msTimer();
    inline static void Form500msTimer();
    inline static void Form1secTimer();
    inline static void Form5secTimer();

public:

    static void Initialization();
    static LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};
