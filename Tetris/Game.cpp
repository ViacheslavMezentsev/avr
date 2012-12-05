#include "Defines.h"
#include "Configuration.h"
#include "Console.h"
#include "Figures.h"
#include "Game.h"


// -=[ ������� ������ ]=-

extern char Version[ 16 ];
extern char buffer[ 16 ];
extern char * utoa_fast_div( uint32_t value, char * buffer );


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-

uint8_t oldx, oldy;
uint8_t x, y;
EnFigureType FigureType;

CKeys GameKeys;
CFigureI FigureI;
CFigureJ FigureJ;
CFigureL FigureL;
CGame Game;

char Block[ 32 ];


/***********************
*  � � � � � � � � � �
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
 * ������������� ����
 */
void CGame::Initialization() {

    x = 20; y = 6;
    oldx = x; oldy = y;

    FigureType = ftJ;

    // ������� ������
    CConsole::ClearScreen();

    // ������ ������
    CConsole::CursorOff();

}


/**
 * ��������� ��������� ����
 */
void CGame::DrawTitle() {

        CConsole::SetTextBackground( BLUE );
        CConsole::ClearScreen();

        CConsole::SetTextColor( BLACK );
        CConsole::SetTextBackground( LIGHTGRAY );
        CConsole::WriteString( SPSTR( "Pinboard II, ������, ATmega16 @ 16 ���, ������ ������: " ), CConsole::cp1251 );
        CConsole::WriteString( Version );

#ifdef __GNUC__
        CConsole::WriteString( SPSTR( " (gnu)" ) );
#elif defined __ICCAVR__
        CConsole::WriteString( SPSTR( " (iar)" ) );
#endif

        CConsole::ClearEndOfLine();

}


/**
 * ��������� ���� � ������
 */
void CGame::DrawFrame( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
    uint8_t color, uint8_t bgcolor ) {

    CConsole::SetTextColor( color );
    CConsole::SetTextBackground( bgcolor );

    CConsole::GotoXY( x, y );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    CConsole::PutChar( ACS_DBL_HLINE );
    
    // ������� ��������� ��� �������� ���� ��� ������ ����.
    CConsole::PutChar( '[' );
    CConsole::SetTextColor( GREEN );
    CConsole::PutChar( 0xFE );
    CConsole::SetTextColor( color );
    CConsole::PutChar( ']' );

    for ( uint8_t i = 0; i < width / 2 - 6; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::WriteString( SPSTR( " ������ " ), CConsole::cp1251 );

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
 * ��������� �������
 */
void CGame::DrawGlass( uint8_t x, uint8_t y, uint8_t width, uint8_t height, 
    uint8_t color, uint8_t bgcolor ) {

    CConsole::SetTextColor( color );
    CConsole::SetTextBackground( bgcolor );

    CConsole::GotoXY( x, y );

    CConsole::PutChar( ACS_ULCORNER );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_URCORNER );

    for ( uint8_t i = 0; i < height; i++ ) {

        CConsole::GotoXY( x, y + i + 1 );
        CConsole::PutChar( ACS_VLINE );

        CConsole::SetTextBackground( BLACK );
        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::SetTextBackground( bgcolor );
        CConsole::PutChar( ACS_VLINE );

    }

    CConsole::GotoXY( x, y + height + 1 );

    CConsole::PutChar( 0xCF );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( 0xCF );

}


/**
 * ���������� ������.
 */
void CGame::DrawFigure(){
    
    if ( F_State != gsRunning ) return;

    CConsole::CursorOff();

    // ��������� ������� ����������.
    oldx = x; oldy = y;

    CConsole::SetTextColor( BLUE );
    CConsole::SetTextBackground( BLACK );

    // ������� ������ �����������.
    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {

            CConsole::GotoXY( x + j, y + i );
            CConsole::PutChar( ' ' );

        } // for
    
    } // for

    y++;
    if ( y > 19 ) y = 6;

    // ������ �����.
    switch ( FigureType ) {

        case ftI: { CopyFigureToRAM( Block, FigureI[0] ); break; }
    
        case ftJ: { CopyFigureToRAM( Block, FigureJ[0] ); break; }



        default:;

    } // switch

    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {
            
            if ( Block[ i * 8 + j ] == 'x' ) {

                CConsole::GotoXY( x + j, y + i );
                CConsole::PutChar( 0xDB );

            } // if

        } // for
    
    } // for

    CConsole::SetTextColor( BLACK );

}


/**
 * ���������� ������� �������������� ������.
 */
void CGame::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::GotoXY( 1, 25 );

    // ���������� �������������� �������.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetTextColor( RED );
            CConsole::SetTextBackground( LIGHTGRAY );
            
            // �����������
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // �����������
            CConsole::PutChar( ' ' );

            CConsole::SetTextColor( BLACK );
            CConsole::WriteString( Keys[i], CConsole::cp1251 );

        }

    }

    // ��������� ������ ������������ �� ����� ������.
    CConsole::ClearEndOfLine();

}


/**
 * ���� ����
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

    CConsole::GotoXY( 1, 25 );

}