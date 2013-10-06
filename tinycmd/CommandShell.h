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

public:

    static void Initialization();

    static void FormActivate();
    static void FormKeyDown( uint16_t Key );

};
