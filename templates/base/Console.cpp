///////////////////////////////////////////////////////////
//  Console.cpp
//  Implementation of the Class CConsole
//  Created on:      18-���-2012 04:34:33
//  Original author: �����
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "Console.h"


// -=[ ���������� �� ����-������ ]=-

/* CP1251 (WIN) --> CP866 (ALT) */
// ������� ������������� �� CP1251 (WIN)
#ifdef __ICCAVR__
    __flash uint8_t CP1251_TO_CP866[ 128 ] = {
#elif defined __GNUC__
    const uint8_t _CP1251_TO_CP866[ 128 ] PROGMEM = {
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


// -=[ ���������� � ��� ]=-

uint8_t Data[11];

PR_BEGIN_EXTERN_C
    FIFO( 16 ) uart_rx_fifo;
PR_END_EXTERN_C


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


uint8_t CConsole::GetChar() {

    uint8_t ret = 0;

    __disable_interrupt();

    if ( !FIFO_IS_EMPTY( uart_rx_fifo ) ) {

        // ���� � ������ ���� ������, �� ��������� ��.
        ret = FIFO_FRONT( uart_rx_fifo );

        FIFO_POP( uart_rx_fifo );

    }

    __enable_interrupt();

    return ret;

}


void CConsole::PutChar( uint8_t ch, EnCodePage CodePage ) {

    // ��� ��������� �������� ����������� �����.
    while ( !( UCSRA & ( 1 << UDRE ) ) );

	// ������� ������.
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

    // ������� ��������� ��������.
    while ( !( UCSRA & ( 1 << TXC ) ) );

	UCSRA &= ~( ( 1 << TXC ) || ( 1 << UDRE ) );

}


void CConsole::WriteData() {

    Data[0] = '\033';
    Data[1] = '[';

    WriteString( ( const char * ) Data, cp866 );
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


/**
 * ����� �����.
 */
void CConsole::Beep( uint16_t Frequency, uint8_t Duration  ) {  

    // ��������� ���������� (���� ������������).
    Data[2] = ( Frequency / 100 ) + '0';

    Frequency %= 100;

    Data[3] = ( Frequency / 10 ) + '0';
    Data[4] = ( Frequency % 10 ) + '0';
    
    Data[5] = ';';

    Data[6] = ( Duration / 10 ) + '0';
    Data[7] = ( Duration % 10 ) + '0';

    Data[8] = 'B';

    // ����� ����� (BELL).
    Data[9] = '\a';
    Data[10] = '\0';

    WriteData();
}


/**
 * �������� �����.
 */
void CConsole::ClearScreen( EnClearMode Mode ) {

    switch ( Mode ) {

        // �������� �� ������� �� ����� ������.
        case cmFromCursorToEnd: { Data[2] = '0'; break; }

        // �������� �� ������ ������ �� �������.
        case cmFromBeginToCursor: { Data[2] = '1'; break; }

        // �������� ���� �����.
        case cmAll: { Data[2] = '2'; break; }

        default: Data[2] = '2';

    }

    Data[3] = 'J';
    Data[4] = '\0';

    WriteData();
}


/**
 * ������� ������.
 */
void CConsole::ClearLine( EnClearMode Mode ) {

    switch ( Mode ) {

        // �������� �� ������� �� ����� ������.
        case cmFromCursorToEnd: { Data[2] ='0'; break; }

        // �������� �� ������ ������ �� �������.
        case cmFromBeginToCursor: { Data[2] = '1'; break; }

        // �������� ��� ������.
        case cmAll: { Data[2] = '2'; break; }

        default: Data[2] = '0';

    }

    Data[3] = 'K';
    Data[4] = '\0';

    WriteData();
}


/**
 * �������� n ������ �� ������� �������.
 */
void CConsole::ClearForward( uint8_t Count ) {

    if ( Count == 0 ) return;

    Data[2] = ( Count / 10 ) + '0';
    Data[3] = ( Count % 10 ) + '0';
    Data[4] = 'X';
    Data[5] = '\0';

    WriteData();

}


/**
 * �������� ������.
 */
void CConsole::CursorOn() {

    WriteString( SPSTR( "\033[?25h" ) );

}


/**
 * �������� ������.
 */
void CConsole::CursorOff() {

    WriteString( SPSTR( "\033[?25l" ) );

}


/**
 * ��������� ��������� �������.
 */
void CConsole::SaveCursor() {

    WriteString( SPSTR( "\033[s" ) );

}


/**
 * ������������ ����������� ��������� �������.
 */
void CConsole::RestoreCursor() {

    WriteString( SPSTR( "\033[u" ) );

}


/**
 * ��������� ���������� ������.
 */
void CConsole::SetForegroundColor( EnColor Color ) {

    Data[2] = ( Color & 0x8 ) ? '1' : '2';
    Data[3] = 'm';
    Data[4] = '\0';

    WriteData();

    Data[2] = '3';
    Data[3] = ( Color & 0x07 ) + '0';
    Data[4] = 'm';
    Data[5] = '\0';

    WriteData();
}


/**
 * ��������� ���������� ����.
 */
void CConsole::SetBackgroundColor( EnColor Color ) {

    Data[2] = ( Color & 0x8 ) ? '5' : '6';
    Data[3] = 'm';
    Data[4] = '\0';

    WriteData();

    Data[2] = '4';
    Data[3] = ( Color & 0x07 ) + '0';
    Data[4] = 'm';
    Data[5] = '\0';

    WriteData();
}


/**
 * ��������� ���������� ������ � ����.
 */
void CConsole::SetColor( EnColor ForegroundColor, EnColor BackgroundColor ) {

    SetTextAttributes( atOff );
    SetForegroundColor( ForegroundColor );
    SetBackgroundColor( BackgroundColor );
}


/**
 * ��������� ���������.
 */
void CConsole::SetTextAttributes( EnAttributes Attributes ) {

    Data[2] = ( Attributes & 0x0F ) + '0';
    Data[3] = 'm';
    Data[4] = '\0';

    WriteData();
}


/**
 * ����������� � ������� Left � ������ Top.
 */
void CConsole::MoveTo( uint8_t Left, uint8_t Top ) {

    if ( Left == 0 || Top == 0 ) return;

    if ( Left > MAX_X || Top > MAX_Y ) return;

    Data[2] = ( Top / 10 ) + '0';
    Data[3] = ( Top % 10 ) + '0';
    Data[4] = ';';
    Data[5] = ( Left / 10 ) + '0';
    Data[6] = ( Left % 10 ) + '0';
    Data[7] = 'f';
    Data[8] = '\0';

    WriteData();
}


/**
 * ������������� ����������� ������� �� �����������.
 */
void CConsole::Move( EnMoveDirection Direction, uint8_t Delta ) {

    Data[2] = ( Delta / 10 ) + '0';
    Data[3] = ( Delta % 10 ) + '0';

    switch ( Direction ) {

        // ����� �� n �����.
        case mdUp: { Data[4] = 'A'; break; }

        // ���� �� n �����.
        case mdDown: { Data[4] = 'B'; break; }

        // ������ �� n �������.
        case mdForward: { Data[4] = 'C'; break; }

        // ����� �� n �������.
        case mdBackward: { Data[4] = 'D'; break; }

        default: Data[4] = 'C';

    }
    
    Data[5] = '\0';

    WriteData();
}


/**
 * ����� ���� � ������ � ����������.
 */
void CConsole::DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height,
        EnColor Color, EnColor bgColor, char * Caption ) {

    SetColor( Color, bgColor );
    MoveTo( Left, Top );

    PutChar( ACS_DBL_ULCORNER );

    // ������� �������.
    if ( Caption != NULL ) {

        uint8_t len = Width - strlen( Caption );

        len /= 2;

        for ( uint8_t i = 0; i < len; i++ ) PutChar( ACS_DBL_HLINE );

        PutChar( ' ' );

        WriteString( Caption );

        PutChar( ' ' );

    } else {

        PutChar( ACS_DBL_URCORNER );
    }

    PutChar( ACS_DBL_URCORNER );

    // ������������ �������.
    for ( uint8_t i = 0; i < Height; i++ ) {

        MoveTo( Left, Top + i + 1 );
        PutChar( ACS_DBL_VLINE );

        for ( uint8_t j = 0; j < Width; j++ ) PutChar( ' ' );

        PutChar( ACS_DBL_VLINE );

    }

    // ������ �������.
    MoveTo( Left, Top + Height + 1 );

    PutChar( ACS_DBL_LLCORNER );

    for ( uint8_t i = 0; i < Width; i++ ) PutChar( ACS_DBL_HLINE );

    PutChar( ACS_DBL_LRCORNER );

}