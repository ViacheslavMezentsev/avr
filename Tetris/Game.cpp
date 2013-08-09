#include "Defines.h"
#include "Configuration.h"
#include "SystemTime.h"
#include "Version.h"
#include "Console.h"
#include "PLC.h"
#include "Figures.h"
#include "Game.h"


// -=[ ���������������� ���� ]=-


// -=[ ������� ������ ]=-

extern SYSTEMTIME Time;
extern char buffer[ 16 ];


// -=[ ���������� �� ����-������ ]=-


// -=[ ���������� � ��� ]=-

volatile uint8_t oldx, oldy;
volatile uint8_t x, y;

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


/**
 * �����������.
 */
CGame::CGame() {

    level = 0;
    state = gsStopped;

}


/**
 * ������������� ����.
 */
void CGame::Initialization() {

    x = 23; 
    y = GLASS_OFFSET_TOP;

    oldx = x; 
    oldy = y;

    FigureColor = clDarkGray;
    FigureType = ftJ;

    CopyFigureToRAM( Block, FigureJ[0] );

    // ������� ������ �������.
    for ( uint16_t i = 0; i < ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT; i++ ) Glass[i] = 0;    

}


/**
 * ��������� ��������� ����.
 */
void CGame::DrawTitle() {

    CConsole::MoveTo( 1, 1 );   
    CConsole::SetColor( clBlack, clWhite );

    CConsole::WriteString( SPSTR( "Pinboard II, ������, ATmega16 @ 16 ���, ������ ������: " ), CConsole::cp1251 );
    CConsole::WriteString( CVersion::GetVersionString() );

#ifdef __GNUC__
        CConsole::WriteString( SPSTR( " (gnu)" ) );
#elif defined __ICCAVR__
        CConsole::WriteString( SPSTR( " (iar)" ) );
#endif

    CConsole::ClearLine();

}


/**
 * ��������� ���� � ������.
 */
void CGame::DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor ) {

    CConsole::MoveTo( Left, Top );
    CConsole::SetColor( Color, bgColor );
    
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
    CConsole::SetColor( clBlack, clBlue );
    CConsole::PutChar( 0xDC );

    CConsole::SetColor( Color, bgColor );

    for ( uint8_t i = 0; i < Height; i++ ) {

        // ����� �������.
        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_DBL_VLINE );
        
        // ��������� �� ����� ������ �������.
        CConsole::ClearForward( Width + 2 );

        // ������ �������.
        CConsole::Move( mdForward, Width + 2 );
        CConsole::PutChar( ACS_DBL_VLINE );

        // ����.
        CConsole::SetColor( clBlack, clBlue );
        CConsole::PutChar( 0xDB );

        CConsole::SetColor( Color, bgColor );
    }

    CConsole::MoveTo( Left, Top + Height + 1 );

    CConsole::PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_DBL_HLINE );

    CConsole::PutChar( ACS_DBL_LRCORNER );
    
    // ����.
    CConsole::SetColor( clBlack, clBlue );
    CConsole::PutChar( 0xDB );

    CConsole::MoveTo( Left + 1, Top + Height + 2 );    
    CConsole::SetColor( clBlack, clBlue );

    for ( uint8_t i = 0; i < Width + 4; i++ ) CConsole::PutChar( 0xDF );

}


/**
 * ��������� �������.
 */
void CGame::DrawGlass( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height, 
        EnColor Color, EnColor bgColor ) {

    CConsole::SetColor( Color, bgColor );

    CConsole::MoveTo( Left, Top );

    CConsole::PutChar( ACS_ULCORNER );

    for ( uint8_t i = 0; i < Width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_URCORNER );

    for ( uint8_t i = 0; i < Height; i++ ) {

        // ����� �������.
        CConsole::MoveTo( Left, Top + i + 1 );
        CConsole::PutChar( ACS_VLINE );
        
        // ��������� �� ����� ������ �������.
        CConsole::SetBackgroundColor( clBlack );
        CConsole::ClearForward( Width + 2 );

        // ������ �������.
        CConsole::Move( mdForward, Width + 2 );        
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
    if ( state != gsRunning ) return;

    y++;

    // �������� ������������.

    // ���� ���������� ������������ ��� ��� �������, �� ��������� �
    // ��������� ������, �������� ������� �� ������.
    //for ( uint8_t i = 0; i < GLASS_HEIGHT; i++ ) {
    //    
    //    for ( uint8_t j = 0; j < GLASS_WIDTH; j++ ) {

    //    }

    //}

    CConsole::SetTextAttributes( atOff );
    CConsole::SetBackgroundColor( clBlack );

    // ������� ������ �����������.
    for ( uint8_t i = 0; i < 4; i++ ) {
        
        for ( uint8_t j = 0; j < 8; j++ ) {            

            if ( Block[ i * 8 + j ] != 'x' ) continue;

            offset = ( oldy - GLASS_OFFSET_TOP + i ) * ( GLASS_WIDTH / 2 ) + ( oldx - GLASS_OFFSET_LEFT + j ) / 2;

            if ( offset >= ( GLASS_WIDTH / 2 ) * GLASS_HEIGHT ) continue;

            // ���� ��������, �� ���� ������� �������.
            if ( ( oldx - GLASS_OFFSET_LEFT + j ) % 2 ) {
                
                CConsole::SetForegroundColor( ( EnColor ) ( Glass[ offset ] & 0x0F ) );

            // ���� ������, �� ���� ������� �������.
            } else { 
                
                CConsole::SetForegroundColor( ( EnColor ) ( Glass[ offset ] >> 4 ) );
            }

            CConsole::MoveTo( oldx + j, oldy + i );
            CConsole::PutChar( 0xDB );

        } // for
    
    } // for    

    // ��������� ������.
    if ( y > 19 ) { 
       
        x = 23;
        y = GLASS_OFFSET_TOP;

        FigureType = ( EnFigureType ) ( ( uint8_t ) FigureType + 1 );
        FigureType = ( EnFigureType ) ( ( uint8_t ) FigureType % 7 );

        FigureColor = ( EnColor ) ( ( uint8_t ) FigureColor + 1 );
        FigureColor = ( EnColor ) ( ( uint8_t ) FigureColor % 7 + 7 );

    }

    CConsole::SetColor( FigureColor, clBlack );

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
            
            if ( Block[ i * 8 + j ] != 'x' ) continue;

            CConsole::MoveTo( x + j, y + i );
            CConsole::PutChar( 0xDB );

        } // for
    
    } // for

    // ��������� ������� ����������.
    oldy = y;
    oldx = x;

}


/**
 * ���������� ������� �������������� ������.
 */
void CGame::DrawFunctionKeys( CKeys & Keys ) {

    CConsole::SetTextAttributes( atOff );
    CConsole::MoveTo( 1, 25 );

    // ���������� �������������� �������.
    for ( uint8_t i = 0; i < Keys.Count(); i++ ) {

        if ( Keys[i] != 0 ) {
            
            // ����������� ���������� ������.
            CConsole::SetColor( clRed, clWhite );
            
            uint8_t ch, n = 0;

            ch = Keys[i][ n++ ];

            while ( ch != '\0' ) {                                 

                // ����� ����������� ������ ���� ��� �������� ��������.
                if ( ch == '|' ) { 
                    
                    CConsole::SetForegroundColor( clBlack );

                } else {

                    CConsole::PutChar( ch, CConsole::cp1251 );
                }

                ch = Keys[i][ n++ ];

            }

        }

    }

    // ��������� ������ ������������ �� ����� ������.
    CConsole::ClearLine();

}


LRESULT CGame::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

    switch ( uMsg ) {

        // ����� ����� ������� �����-������ ������.
        // ��� ������� ������ ���� ��������.
        case SW_SHOW: { break; }

        case WM_ACTIVATE: {

            switch ( wParam ) {

                case WA_ACTIVE: { FormActivate(); break; }

                case WA_INACTIVE: { break; }

            }

            break;
        }

        case WM_KEYDOWN: { FormKeyDown( wParam ); break; }

        case WM_KEYUP: { break; }

        case WM_TIMER: {

            switch ( wParam ) {

                case ID_TIMER_10MS: { Form10msTimer(); break; }

                case ID_TIMER_100MS: { Form100msTimer(); break; }

                case ID_TIMER_500MS: { Form500msTimer(); break; }

                case ID_TIMER_1S: { Form1secTimer(); break; }

                case ID_TIMER_5S: { Form5secTimer(); break; }

            }

            break;

        }

        case WM_PAINT: { break; }

    }

    return 0;

}


void CGame::FormActivate() {

    Initialization();
    
    CConsole::CursorOff();

    // ������� ������.
    CConsole::SetColor( clLightGray, clBlue );
    CConsole::ClearScreen();

    DrawTitle();
    DrawFunctionKeys( GameKeys );
    DrawFrame( 13, 2, 52, 20, clLightGray, clWhite );

    DrawGlass( GLASS_OFFSET_LEFT - 1, GLASS_OFFSET_TOP - 1,
        GLASS_WIDTH + 2, GLASS_HEIGHT + 2, clLightGray, clWhite );

    // ��������� ����.
    state = gsRunning;

};


void CGame::FormKeyDown( uint16_t Key ) {   

    switch ( Key ) {

        case VK_ESCAPE: { 
            
            // ������������ � ��������� ��������.
            state = gsStopped; 
            CPLC::SetActiveWindow( HWND_COMMAND_SHELL );

            break; 
        }

        case VK_UP: { break; }

        case VK_DOWN: {  break; }

        case VK_LEFT: { 
        
            x++;
            break; 
        }

        case VK_RIGHT: { 
        
            x--;
            break; 
        }

        case VK_HOME: { break; }

        case VK_END: { break; }

        case VK_TAB: { break; }

        default: {

            switch ( Key ) {

                // �������� ����.
                case '0': {

                    break;
                }

                // ����� ����.
                case '2': {

                    state = gsStopped;
                    CPLC::SetActiveWindow( HWND_GAME );
                    break;
                }

            }

        }

    }

}

void CGame::Form10msTimer() {

};


void CGame::Form100msTimer() {

};

void CGame::Form500msTimer() {

    // ���������� ����� � ���������.
    CSystemTime::GetTimeAsSystemTime( & Time );

    CConsole::SaveCursor();

    CConsole::MoveTo( 73, 1 );
    CConsole::SetColor( clBlack, clWhite );

    buffer[0] = Time.wHour / 10 + '0';
    buffer[1] = Time.wHour / 10 + '0';
    buffer[2] = ':';
    buffer[3] = Time.wMinute / 10 + '0';
    buffer[4] = Time.wMinute % 10 + '0';
    buffer[5] = ':';
    buffer[6] = Time.wSecond / 10 + '0';
    buffer[7] = Time.wSecond % 10 + '0';
    buffer[8] = '\0';

    CConsole::WriteString( buffer );

    CConsole::RestoreCursor();

};


void CGame::Form1secTimer() {    

    if ( Game.State == gsRunning ) {

        // ���������� ������ � ����������� �� ������ ����.
        switch ( Game.Level ) {

            case 0: { Game.DrawFigure(); break; }

        }      

    } // if

};


void CGame::Form5secTimer() {

};

