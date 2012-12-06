///////////////////////////////////////////////////////////
//  Console.cpp
//  Implementation of the Class CConsole
//  Created on:      18-ноя-2012 04:34:33
//  Original author: Слава
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "kbd.h"
#include "Console.h"


// -=[ Постоянные во флеш-памяти ]=-

/* CP1251 (WIN) --> CP866 (ALT) */
// Таблица перекодировки из CP1251 (WIN)
#ifdef __ICCAVR__
    __flash uint8_t CP1251_TO_CP866[ 128 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _CP1251_TO_CP866[ 128 ] = {
#endif
//PROGMEM unsigned char CP1251_TO_CP866[ 128 ] = {
/*         .0   .1   .2   .3   .4   .5   .6   .7   .8   .9   .A   .B   .C   .D   .E   .F   */
/* 8. */  0x20,0x20,0x2C,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3C,0x20,0x20,0x20,0x20,
/* 9. */  0x20,0x27,0x27,0x22,0x22,0xF9,0x2D,0x2D,0x20,0x20,0x20,0x3E,0x20,0x20,0x20,0x20,
/* A. */  0xFF,0x20,0x20,0x6A,0x20,0x20,0x7C,0x20,0xF0,0x20,0x20,0x22,0x20,0x20,0x20,0x20,
/* B. */  0xF8,0x20,0x49,0x69,0x20,0x20,0x20,0xFA,0xF1,0x23,0x20,0x22,0x6A,0x53,0x73,0x20,
/* C. */  0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
/* D. */  0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
/* E. */  0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
/* F. */  0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF
};
#ifdef __GNUC__
    FU08T_PTR CP1251_TO_CP866(_CP1251_TO_CP866);
#endif

FLASHSTR_DECLARE( char, ESC, "\033[" );


// -=[ Переменные в ОЗУ ]=-

FIFO( 16 ) uart_rx_fifo;


/***********************
*  Р Е А Л И З А Ц И Я
*  ~~~~~~~~~~~~~~~~~~~
************************/


uint8_t CConsole::GetChar() {

    uint8_t ret = 0;

    // запрещаем прерывания
    __disable_interrupt();

    if ( !FIFO_IS_EMPTY( uart_rx_fifo ) ) {

        // если в буфере есть данные, то извлекаем их
        ret = FIFO_FRONT( uart_rx_fifo );

        FIFO_POP( uart_rx_fifo );

    }

    // разрешаем прерывания
    __enable_interrupt();

    return ret;

}


void CConsole::PutChar( uint8_t ch, EnCodePage CodePage ) {

    // ждем окончания передачи предыдущего байта
    while ( !( UCSRA & ( 1 << UDRE ) ) );

	// send character
    switch ( CodePage ) {

        case cp866: {

            UDR = ch;
            break;
        }

        case cp1251: {

            UDR = ( ch & 0x80 ) ? CP1251_TO_CP866[ ch & 0x7F ] : ch;
            break;
        }

    }

    // wait for char to be send
    while ( !( UCSRA & ( 1 << TXC ) ) );

	UCSRA &= ~( ( 1 << TXC ) || ( 1 << UDRE ) );

}


char * CConsole::ReadString( char * s ) {

    uint8_t len = 0;
    uint8_t ch;
    char * p = s;

    while ( true ) {

        do _delay_ms( 10 ); while ( !( ch = GetChar() ) );

        // enter hit, end of input
        if ( ( ch == KB_ENTER ) || ( ch == 0x0D ) ) break;

        if ( ch == KB_BACK ) {

            if ( len > 0 ) {

                p[ len-- ] = 0;

                // delete char and go back (if some chars left)
                PutChar( KB_BACK );
                PutChar( ' ' );
                PutChar( KB_BACK );

            }

            continue;

        }

        if ( ( ( ch > 0xAF ) && ( ch < 0xE0 ) ) || ( ch < ' ' ) || ( ch > 0xF3 ) ) continue;

        // print back to screen
        PutChar( ch );

        p[ len++ ] = ch;

    }

    p[ len ] = 0;

    return s;

}


void CConsole::WriteString( FCHAR_PTR s, EnCodePage CodePage, uint8_t Length ) {

    uint8_t index = 0;

    while ( ( s[ index ] != 0 ) | ( index == ( Length - 1 ) ) ) {

        PutChar( ( uint8_t ) s[ index ], CodePage );

        index++;

    }

}


void CConsole::WriteString( const char * s, EnCodePage CodePage, uint8_t Length ) {

    uint8_t index = 0;

    while ( ( s[ index ] != 0 ) | ( index == ( Length - 1 ) ) ) {

        PutChar( ( uint8_t ) s[ index ], CodePage );

        index++;

    }

}


void CConsole::ClearScreen() {

    WriteString( SPSTR( "\033[2J" ) );

}


void CConsole::ClearEndOfLine() {

    WriteString( SPSTR( "\033[K" ) );

}


void CConsole::CursorOn() {

    WriteString( SPSTR( "\033[?25h" ) );

}


void CConsole::CursorOff() {

    WriteString( SPSTR( "\033[?25l" ) );

}


void CConsole::SaveCursor() {

    WriteString( SPSTR( "\033[s" ) );

}


void CConsole::RestoreCursor() {

    WriteString( SPSTR( "\033[u" ) );

}


void CConsole::SetForegroundColor( EnColor Color ) {

    if ( Color & 0x8 ) {
        
        WriteString( ESC );
        PutChar( '1' );
        PutChar( 'm' );

    } else {

        WriteString( ESC );
        PutChar( '2' );
        PutChar( 'm' );
    }

    WriteString( ESC );
    PutChar( '3' );
    PutChar( ( Color & 0x07 ) + '0' );
    PutChar( 'm' );

}


void CConsole::SetBackgroundColor( EnColor Color ) {

    if ( Color & 0x8 ) {
        
        WriteString( ESC );
        PutChar( '5' );
        PutChar( 'm' );

    } else {

        WriteString( ESC );
        PutChar( '6' );
        PutChar( 'm' );
    }

    WriteString( ESC );
    PutChar( '4' );
    PutChar( ( Color & 0x07 ) + '0' );
    PutChar( 'm' );

}


void CConsole::SetTextAttributes( EnAttributes Attributes ) {

    WriteString( ESC );
    PutChar( ( Attributes & 0x0F ) + '0' );
    PutChar( 'm' );

}


void CConsole::MoveTo( uint8_t Left, uint8_t Top ) {

    if ( Left == 0 || Top == 0 ) return;

    if ( Left > MAX_X || Top > MAX_Y ) return;

    WriteString( ESC );
    PutChar( ( Top / 10 ) + '0' );
    PutChar( ( Top % 10 ) + '0' );
    PutChar( ';' );
    PutChar( ( Left / 10 ) + '0' );
    PutChar( ( Left % 10 ) + '0' );
    PutChar( 'f' );

}


void CConsole::Move( EnMoveDirection Direction, uint8_t Delta ) {

    WriteString( ESC );

    PutChar( ( Delta / 10 ) + '0' );
    PutChar( ( Delta % 10 ) + '0' );

    switch ( Direction ) {

        case mdUp: { PutChar( 'A' ); break; }

        case mdDown: { PutChar( 'B' ); break; }

        case mdForward: { PutChar( 'C' ); break; }

        case mdBackward: { PutChar( 'D' ); break; }

        default: PutChar( 'C' );

    }

}