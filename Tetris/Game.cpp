#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "Game.h"


// -=[ Внешние ссылки ]=-

extern char buffer[16];


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

CGame Game;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


CGame::CGame() {

    F_Level = 0;

}


/**
 * Инициализация игры
 */
void CGame::Initialization() {

    // Очистка экрана
    CConsole::ClearScreen();

    // Прячем курсор
    CConsole::CursorOff();

}


/**
 * Отрисовка стакана
 */
void CGame::DrawGlass( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
    uint8_t color, uint8_t bgcolor ) {

    CConsole::SetTextAttr( color );
    CConsole::GotoXY( x, y );

    CConsole::PutChar( ACS_ULCORNER );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_URCORNER );

    for ( uint8_t i = 0; i < height; i++ ) {

        CConsole::GotoXY( x, y + i + 1 );
        CConsole::PutChar( ACS_VLINE );
        
        CConsole::SetTextAttr( bgcolor );
        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::SetTextAttr( color );
        CConsole::PutChar( ACS_VLINE );

    }

    CConsole::GotoXY( x, y + height + 1 );

    CConsole::PutChar( ACS_LLCORNER );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_LRCORNER );


}


/**
 * Отрисовка фигуры
 */
void CGame::DrawFigure(){

}


/**
 * Цикл игры
 */
void CGame::Run() {

    char * cmd;

    do {
    
        cmd = CConsole::ReadString( buffer );

    } while ( ! ( ( cmd[0] == 'q' ) && ( cmd[1] == 0 ) ) );

    CConsole::SetTextAttr( LIGHTGRAY );
    CConsole::ClearScreen();

    CConsole::GotoXY( 1, 25 );

}