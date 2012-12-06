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

EnColor FigureColor;
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
char Glass[ ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT ];


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

    x = 23; 
    y = GLASS_OFFSET_TOP;

    oldx = x; oldy = y;

    FigureColor = clDarkGray;
    FigureType = ftJ;

    // Очищаем память стакана.
    for ( uint16_t i = 0; i < ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT; i++ ) Glass[i] = 0;
    
    // Прячем курсор.
    CConsole::CursorOff();

    // Очистка экрана.
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetBackgroundColor( clBlue );
    CConsole::ClearScreen();

}


/**
 * Отрисовка заголовка окна
 */
void CGame::DrawTitle() {

        CConsole::SetTextAttributes( atOff );        
        CConsole::MoveTo( 1, 1 );

        CConsole::SetForegroundColor( clBlack );
        CConsole::SetBackgroundColor( clWhite );
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
        EnColor Color, EnColor bgColor ) {

    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( Color );
    CConsole::SetBackgroundColor( bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    CConsole::PutChar( ACS_DBL_HLINE );
    
    // Элемент заголовка для закрытия окна при помощи мыши.
    CConsole::PutChar( '[' );
    CConsole::SetForegroundColor( clGreen );
    CConsole::PutChar( 0xFE );
    CConsole::SetForegroundColor( Color );
    CConsole::PutChar( ']' );

    for ( uint8_t i = 0; i < Width / 2 - 6; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::WriteString( SPSTR( " Тетрис " ), CConsole::cp1251 );

    for ( uint8_t i = 0; i < Width / 2 - 4; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );

    // Тень.
    CConsole::SetTextAttributes( atOff );
    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clBlue );
    CConsole::PutChar( 0xDC );

    CConsole::SetForegroundColor( Color );
    CConsole::SetBackgroundColor( bgColor );

    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );

        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < Width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::PutChar( ACS_DBL_VLINE );

        // Тень.
        CConsole::SetTextAttributes( atOff );
        CConsole::SetForegroundColor( clBlack );
        CConsole::SetBackgroundColor( clBlue );
        CConsole::PutChar( 0xDB );

        CConsole::SetForegroundColor( Color );
        CConsole::SetBackgroundColor( bgColor );
    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );
    
    // Тень.
    CConsole::SetTextAttributes( atOff );
    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clBlue );
    CConsole::PutChar( 0xDB );

    CConsole::MoveTo( Left + 1, Top + Height + 2 );
    
    CConsole::SetTextAttributes( atOff );
    CConsole::SetForegroundColor( clBlack );
    CConsole::SetBackgroundColor( clBlue );

    for ( uint8_t i = 0; i < Width + 4; i++ ) CConsole::PutChar( 0xDF );

}


/**
 * Отрисовка стакана
 */
void CGame::DrawGlass( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor ) {

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( bgColor );
    CConsole::SetForegroundColor( Color );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_ULCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_URCORNER );

    for ( uint8_t i = 0; i < Height; i++ ) {

        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::SetBackgroundColor( clBlack );
        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < Width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::SetBackgroundColor( bgColor );
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

    // Заглушка: если игра не запущена, то ничего не делаем.
    if ( F_State != gsRunning ) return;

    // Проверка столкновения.

    // Если обнаружено столкновение или дно стакана, то сохраняем фигуру
    // в памяти и перерисовываем стакан.
    //for ( uint8_t i = 0; i < GLASS_HEIGHT; i++ ) {
    //    
    //    for ( uint8_t j = 0; j < GLASS_WIDTH; j++ ) {

    //    }

    //}

    // Сохраняем текущие координаты.
    oldx = x; oldy = y;

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( clBlack );

    // Удаляем старое изображение, перерисовывая участок стакана.
    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {            

            offset = ( y - GLASS_OFFSET_TOP + i ) * ( GLASS_WIDTH / 2 ) + ( x - GLASS_OFFSET_LEFT + j ) / 2;

            if ( offset >= ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT ) continue;

            // Если нечётное, то берём младшую тетраду.
            if ( ( x - GLASS_OFFSET_LEFT + j ) % 2 ) {
                
                CConsole::SetForegroundColor( ( EnColor ) ( Glass[ offset ] & 0x0F ) );

            // Если чётное, то берём старшую тетраду.
            } else { 
                
                CConsole::SetForegroundColor( ( EnColor ) ( Glass[ offset ] >> 4 ) );
            }

            CConsole::MoveTo( x + j, y + i );
            CConsole::PutChar( 0xDB );

        } // for
    
    } // for

    y++;

    if ( y > 19 ) { 
       
        y = GLASS_OFFSET_TOP;

        FigureType = ( EnFigureType ) ( ( uint8_t ) FigureType + 1 );
        FigureType = ( EnFigureType ) ( ( uint8_t ) FigureType % 7 );

        FigureColor = ( EnColor ) ( ( uint8_t ) FigureColor + 1 );
        FigureColor = ( EnColor ) ( ( uint8_t ) FigureColor % 7 + 7 );

    }

    CConsole::SetBackgroundColor( clBlack );
    CConsole::SetForegroundColor( FigureColor );

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

}


/**
 * Отображаем линейку функциональных клавиш.
 */
void CGame::DrawFunctionKeys( CKeys & Keys ) {

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

}