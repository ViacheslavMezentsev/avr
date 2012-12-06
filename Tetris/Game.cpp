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

    x = 23; 
    y = GLASS_OFFSET_TOP;

    oldx = x; oldy = y;

    FigureColor = clDarkGray;
    FigureType = ftJ;

    // ������� ������ �������.
    for ( uint16_t i = 0; i < ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT; i++ ) Glass[i] = 0;
    
    // ������ ������.
    CConsole::CursorOff();

    // ������� ������.
    CConsole::SetTextAttributes( atOff );    
    CConsole::SetBackgroundColor( clBlue );
    CConsole::ClearScreen();

}


/**
 * ��������� ��������� ����
 */
void CGame::DrawTitle() {

        CConsole::SetTextAttributes( atOff );        
        CConsole::MoveTo( 1, 1 );

        CConsole::SetForegroundColor( clBlack );
        CConsole::SetBackgroundColor( clWhite );
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
void CGame::DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor ) {

    CConsole::SetTextAttributes( atOff );    
    CConsole::SetForegroundColor( Color );
    CConsole::SetBackgroundColor( bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_DBL_ULCORNER );

    CConsole::PutChar( ACS_DBL_HLINE );
    
    // ������� ��������� ��� �������� ���� ��� ������ ����.
    CConsole::PutChar( '[' );
    CConsole::SetForegroundColor( clGreen );
    CConsole::PutChar( 0xFE );
    CConsole::SetForegroundColor( Color );
    CConsole::PutChar( ']' );

    for ( uint8_t i = 0; i < Width / 2 - 6; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::WriteString( SPSTR( " ������ " ), CConsole::cp1251 );

    for ( uint8_t i = 0; i < Width / 2 - 4; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_URCORNER );

    // ����.
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

        // ����.
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
    
    // ����.
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
 * ��������� �������
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
 * ���������� ������.
 */
void CGame::DrawFigure(){
    
    uint16_t offset;

    // ��������: ���� ���� �� ��������, �� ������ �� ������.
    if ( F_State != gsRunning ) return;

    // �������� ������������.

    // ���� ���������� ������������ ��� ��� �������, �� ��������� ������
    // � ������ � �������������� ������.
    //for ( uint8_t i = 0; i < GLASS_HEIGHT; i++ ) {
    //    
    //    for ( uint8_t j = 0; j < GLASS_WIDTH; j++ ) {

    //    }

    //}

    // ��������� ������� ����������.
    oldx = x; oldy = y;

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( clBlack );

    // ������� ������ �����������, ������������� ������� �������.
    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {            

            offset = ( y - GLASS_OFFSET_TOP + i ) * ( GLASS_WIDTH / 2 ) + ( x - GLASS_OFFSET_LEFT + j ) / 2;

            if ( offset >= ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT ) continue;

            // ���� ��������, �� ���� ������� �������.
            if ( ( x - GLASS_OFFSET_LEFT + j ) % 2 ) {
                
                CConsole::SetForegroundColor( ( EnColor ) ( Glass[ offset ] & 0x0F ) );

            // ���� ������, �� ���� ������� �������.
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

    // ������ ����� �����������.
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
 * ���������� ������� �������������� ������.
 */
void CGame::DrawFunctionKeys( CKeys & Keys ) {

    char buf[3];

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // ���������� �������������� �������.
    for ( uint8_t i = 0; i < 10; i++ ) {

        if ( Keys[i] != 0 ) {
            
            CConsole::SetForegroundColor( clRed );
            CConsole::SetBackgroundColor( clWhite );
            
            // �����������
            CConsole::PutChar( ' ' );
            
            CConsole::WriteString( utoa_fast_div( i, buf ) );
            
            // �����������
            CConsole::PutChar( ' ' );

            CConsole::SetForegroundColor( clBlack );
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

}