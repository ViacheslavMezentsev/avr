#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "Figures.h"
#include "Game.h"


// -=[ Внешние ссылки ]=-

extern char Version[ 16 ];
extern char buffer[ 16 ];
extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ Постоянные во флеш-памяти ]=-


// -=[ Переменные в ОЗУ ]=-

uint8_t oldx, oldy;
uint8_t x, y;

EnFigureColor FigureColor;
EnFigureType FigureType;

CKeys GameKeys;

CFigureI FigureI;
CFigureJ FigureJ;
CFigureL FigureL;
CFigureO FigureO;
CFigureS FigureS;
CFigureT FigureT;
CFigureZ FigureZ;

CGame Game;

char Block[ 32 ];
char Glass[ GLASS_WIDTH / 2 * GLASS_HEIGHT ];


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


char * CopyFigureToRAM( char * dest, FCHAR_PTR src ) {

    for ( uint8_t i = 0; i < 4; i++ )
        for ( uint8_t j = 0; j < 8; j++ ) dest[ i * 8 + j ] = ( uint8_t ) * ( src++ );

    return dest;

}


CGame::CGame() {

    F_Level = 0;
    F_State = gsStopped;

}


/**
 * Инициализация игры
 */
void CGame::Initialization() {

    x = 20; y = 6;
    oldx = x; oldy = y;

    FigureColor = fcMAGENTA;
    FigureType = ftJ;

    // Очищаем память стакана.
    for ( uint8_t i = 0; i < GLASS_WIDTH / 2 * GLASS_HEIGHT; i++ ) Glass[i] = 0;

    Glass[ ( 4 * GLASS_WIDTH / 2 ) + 3 ] = 0x12;
    
    // Очистка экрана.
    CConsole::ClearScreen();

    // Прячем курсор.
    CConsole::CursorOff();

}


/**
 * Отрисовка заголовка окна
 */
void CGame::DrawTitle() {

        CConsole::SetTextBackground( BLUE );
        CConsole::ClearScreen();

        CConsole::SetTextColor( BLACK );
        CConsole::SetTextBackground( LIGHTGRAY );
        CConsole::WriteString( SPSTR( "Pinboard II, Тетрис, ATmega16 @ 16 МГц, версия сборки: " ), CConsole::cp1251 );
        CConsole::WriteString( Version );

#ifdef __GNUC__
        CConsole::WriteString( SPSTR( " (gnu)" ) );
#elif defined __ICCAVR__
        CConsole::WriteString( SPSTR( " (iar)" ) );
#endif

        CConsole::ClearEndOfLine();

}


/**
 * Отрисовка окна с рамкой
 */
void CGame::DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        uint8_t Color, uint8_t bgColor ) {

    CConsole::SetTextColor( Color );
    CConsole::SetTextBackground( bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    CConsole::PutChar( ACS_DBL_HLINE );
    
    // Элемент заголовка для закрытия окна при помощи мыши.
    CConsole::PutChar( '[' );
    CConsole::SetTextColor( GREEN );
    CConsole::PutChar( 0xFE );
    CConsole::SetTextColor( Color );
    CConsole::PutChar( ']' );

    for ( uint8_t i = 0; i < Width / 2 - 6; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::WriteString( SPSTR( " Тетрис " ), CConsole::cp1251 );

    for ( uint8_t i = 0; i < Width / 2 - 4; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );

    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );

        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < Width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::PutChar( ACS_DBL_VLINE );

    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );

}


/**
 * Отрисовка стакана
 */
void CGame::DrawGlass( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        uint8_t Color, uint8_t bgColor ) {

    CConsole::SetTextColor( Color );
    CConsole::SetTextBackground( bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_ULCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_URCORNER );

    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::SetTextBackground( BLACK );
        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < Width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::SetTextBackground( bgColor );
        CConsole::PutChar( ACS_VLINE );

    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( 0xCF );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( 0xCF );

}


/**
 * Отображаем фигуру.
 */
void CGame::DrawFigure(){
    
    uint16_t offset;

    if ( F_State != gsRunning ) return;

    CConsole::CursorOff();

    // Сохраняем текущие координаты.
    oldx = x; oldy = y;

    CConsole::SetTextBackground( BLACK );

    // Удаляем старое изображение.
    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {

            CConsole::MoveTo( x + j, y + i );

            offset = ( y - GLASS_OFFSET_TOP ) * GLASS_WIDTH / 2 + ( x - GLASS_OFFSET_LEFT ) / 2;

            ( x - GLASS_OFFSET_LEFT ) % 2 ? CConsole::PutChar( Glass[ offset ] & 0x0F ) : CConsole::PutChar( Glass[ offset ] >> 4 );

        } // for
    
    } // for

    y++;

    if ( y > 19 ) { 
       
        y = 6;

        FigureType = ( EnFigureType ) ( ( uint8_t ) FigureType + 1 );
        FigureType = ( EnFigureType ) ( ( uint8_t ) FigureType % 7 );

        FigureColor = ( EnFigureColor ) ( ( uint8_t ) FigureColor + 1 );
        FigureColor = ( EnFigureColor ) ( ( uint8_t ) FigureColor % 9 );

    }

    CConsole::SetTextColor( FigureColor );

    // Рисуем новое изображение.
    switch ( FigureType ) {

        case ftI: { CopyFigureToRAM( Block, FigureI[0] ); break; }
    
        case ftJ: { CopyFigureToRAM( Block, FigureJ[0] ); break; }

        case ftL: { CopyFigureToRAM( Block, FigureL[0] ); break; }

        case ftO: { CopyFigureToRAM( Block, FigureO[0] ); break; }

        case ftS: { CopyFigureToRAM( Block, FigureS[0] ); break; }

        case ftT: { CopyFigureToRAM( Block, FigureT[0] ); break; }

        case ftZ: { CopyFigureToRAM( Block, FigureZ[0] ); break; }

        default:;

    } // switch

    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {
            
            if ( Block[ i * 8 + j ] == 'x' ) {

                CConsole::MoveTo( x + j, y + i );
                CConsole::PutChar( 0xDB );

            } // if

        } // for
    
    } // for

    CConsole::SetTextColor( BLACK );

}


/**
 * Отображаем линейку функциональных клавиш.
 */
void CGame::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::MoveTo( 1, 25 );

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

    F_State = gsRunning;

    do {
    
        cmd = CConsole::ReadString( buffer );

    } while ( ! ( ( cmd[0] == 'q' ) && ( cmd[1] == 0 ) ) );

    F_State = gsStopped;

    CConsole::SetTextAttr( LIGHTGRAY );
    CConsole::ClearScreen();

    CConsole::MoveTo( 1, 25 );

}