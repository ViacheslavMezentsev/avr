#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "Game.h"


// -=[ Внешние ссылки ]=-

extern char buffer[ 16 ];
extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

CGame Game;
CKeys GameKeys;


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
 * Отрисовка окна с рамкой
 */
void CGame::DrawFrame( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
    uint8_t color, uint8_t bgcolor ) {

    CConsole::SetTextColor( color );
    CConsole::SetTextBackground( bgcolor );

    CConsole::GotoXY( x, y );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    CConsole::PutChar( ACS_DBL_HLINE );
    
    // Элемент заголовка для закрытия окна при помощи мыши.
    CConsole::PutChar( '[' );
    CConsole::SetTextColor( GREEN );
    CConsole::PutChar( 0xFE );
    CConsole::SetTextColor( color );
    CConsole::PutChar( ']' );

    for ( uint8_t i = 0; i < width / 2 - 6; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::WriteString( SPSTR( " Тетрис " ), CConsole::cp1251 );

    for ( uint8_t i = 0; i < width / 2 - 4; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );

    for ( uint8_t i = 0; i < height; i++ ) {

        CConsole::GotoXY( x, y + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );
        
        //CConsole::SetTextAttr( color );
        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        //CConsole::SetTextAttr( color );
        CConsole::PutChar( ACS_DBL_VLINE );

    }

    CConsole::GotoXY( x, y + height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );


}


/**
 * Отображаем фигуру.
 */
void CGame::DrawFigure(){

}


/**
 * Отображаем линейку функциональных клавиш.
 */
void CGame::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::GotoXY( 1, 25 );

    // Перебираем функциональные клавиши.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetTextColor( RED );
            CConsole::SetTextBackground( LIGHTGRAY );
            
            // Разделитель
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // Разделитель
            CConsole::PutChar( ' ' );

            CConsole::SetTextColor( BLACK );
            CConsole::WriteString( Keys[i], CConsole::cp1251 );

        }

    }

    // Заполняем пустое пространство до конца строки.
    CConsole::ClearEndOfLine();

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