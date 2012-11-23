/*
    ui.c
    User interface functions.
    Part of MicroVGA CONIO library / demo project
    Copyright (c) 2008-9 SECONS s.r.o., http://www.MicroVGA.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Defines.h"
#include "Configuration.h"
#include "Version.h"

#include "kbd.h"
#include "SystemTime.h"
#include "Console.h"
#include "ui.h"


// -=[ ������� ������ ]=-
extern char Version[16];
extern SYSTEMTIME Time;


// -=[ ���������� �� ����-������ ]=-

// ����������� ���������� �� ������
FLASHSTR_DECLARE( char, uvga_logo,

    "        ##    ##   ####     #   "
    "        ##    ##  ##  ##   ###  "
    "        ##    ## ##    #  ## ## "
    " ##  ## ##    ## ##      ##   ##"
    " ##  ## ##    ## ##      ##   ##"
    " ##  ## ##    ## ## #### #######"
    " ##  ## ##    ## ##   ## ##   ##"
    " ##  ##  ##  ##  ##   ## ##   ##"
    " #####    ####    ##  ## ##   ##"
    " ##        ##      ### # ##   ##"
    " ##    MicroVGA demo application"
    "##                              "
    "www.MicroVGA.com, www.secons.com"

);

FLASHSTR_DECLARE( char, ascii_art_str,

"\033[2J\033[1m���\033[42m��\033[32m\033[47m���\033[40m�����������������������������������������������������������\033[0m\r\n"
"\033[1m\033[32m\033[47m�\033[37m\033[42m���\033[32m\033[40m��������\033[0m \033[1m����\033[46m�۲��۲\033[36m\033[47m���\033[40m��������������\033[0m \033[1m\033[32m���������������������������������\033[0m\r\n"
"\033[1m\033[42m��\033[32m\033[40m���\033[0m \033[1m����\033[46m����\033[36m\033[47m��\033[40m�������\033[46m������\033[40m�����������������������\033[0m \033[1m\033[32m�������������������������\033[0m\r\n"
"\033[1m\033[32m\033[47m�\033[40m��\033[37m ��\033[46m�\033[36m\033[47m��\033[40m��\033[46m�\033[40m���\033[0m\033[36m�\033[37m\033[6C\033[1m\033[31m������������� \033[37m����\033[47m�\033[46m��۲\033[36m\033[47m���\033[40m�������������������������\033[0m\033[36m������\033[37m\r\n"
"\033[1m\033[32m�\033[37m ��\033[46m�\033[36m\033[47m���\033[46m�\033[0m\033[36m�\033[37m  \033[1m\033[31m��������������������������������\033[0m \033[1m�����\033[36m\033[47m�\033[37m\033[46m�\033[36m\033[40m��������������������������\033[0m\033[36m�\033[37m\r\n"
"\033[1m\033[32m�\033[36m�\033[37m�\033[36m\033[47m�\033[40m��\033[46m�\033[0m\033[36m�\033[37m \033[36m��\033[37m   \033[1m\033[31m������������������������������������\033[0m  \033[1m���\033[36m\033[47m��\033[40m���\033[37m  ������\033[36m\033[47m�\033[40m���������\033[0m\033[36m�\033[37m\r\n"
"\033[1m\033[32m \033[36m�\033[37m�\033[36m���\033[0m\033[36m���\033[1m\033[46m�\033[40m���������������\033[0m \033[1m\033[31m��������������������\033[0m  \033[1m\033[35m���\033[0m\033[31m���\033[1m\033[36m \033[0m \033[1m��\033[36m\033[47m�\033[40m����\033[37m \033[32m�\033[37m ��\033[36m\033[47m�\033[40m��\033[0m \033[1m\033[32m������\033[0m\r\n"
"\033[1m\033[32m \033[36m�\033[47m�\033[40m����������������������\033[0m \033[1m\033[31m�������������������\033[0m \033[1m�\033[35m\033[47m�\033[40m���\033[0m\033[31m�����\033[33m�\033[37m \033[1m��\033[46m�\033[36m\033[40m���\033[32m  \033[37m�\033[36m\033[47m�\033[40m��\033[0m \033[1m\033[32m��������\033[0m\r\n"
"\033[1m\033[32m \033[36m����\033[45m�\033[35m\033[40m�\033[0m  \033[1m\033[31m�������\033[0m \033[1m\033[36m�������\033[0m\033[36m�\033[37m \033[1m\033[31m������������������\033[0m\033[31m�\033[37m \033[1m�\033[35m\033[47m�\033[40m���\033[36m \033[30m\033[41mtif\033[0m\033[31m�\033[33m\033[41m�\033[40m��\033[37m \033[1m��\033[36m���\033[37m �\033[35m\033[46m�\033[36m\033[40m��\033[0m \033[1m\033[32m��������\033[0m\r\n"
"\033[1m\033[32m \033[36m��\033[45m���\033[35m\033[40m�\033[0m \033[1m\033[31m������������������������������������\033[41m�\033[0m \033[1m��\033[35m\033[47m�\033[40m��\033[0m \033[1m\033[36m \033[30m\033[41moada\033[0m\033[31m�\033[33m��\033[1m\033[37m��\033[46m�\033[36m\033[45m��\033[40m�\033[37m\033[47m�\033[35m\033[46m��\033[36m\033[45m�\033[0m \033[1m\033[32m��������\033[0m\r\n"
"\033[1m\033[34m�\033[37m \033[36m�\033[45m���\033[35m\033[46m�\033[40m�\033[36m \033[0m\033[31m�\033[1m\033[41m��\033[40m��������������������������������\033[41m�\033[0m\033[31m�\033[37m \033[1m�\033[35m\033[47m�\033[40m����\033[36m \033[0m\033[31m�\033[1m\033[30m\033[41mm\033[0m\033[31m���\033[33m�\033[37m \033[1m�\033[47m�\033[36m\033[45m�\033[35m\033[46m��\033[36m\033[45m��\033[35m\033[46m�\033[40m�\033[32m���������\033[0m\r\n"
"\033[1m\033[34m��\033[37m \033[0m\033[35m�\033[1m\033[47m�\033[46m�\033[40m���\033[0m\033[35m�\033[1m\033[36m \033[0m\033[31m��\033[1m\033[41m���\033[40m�����������������������������\033[0m\033[31m�\033[37m \033[1m�\033[35m\033[47m�\033[40m�\033[45m�\033[40m��\033[45m�\033[0m\033[35m�\033[37m \033[31m���\033[33m�\033[37m \033[1m\033[35m�\033[46m��\033[40m��\033[45m�\033[40m���\033[32m�������\033[43m�\033[33m\033[42m�\033[0m\r\n"
"\033[1m\033[34m����\033[0m \033[35m�\033[1m�����\033[45m�\033[40m�\033[0m\033[35m�\033[1m\033[36m \033[0m\033[31m����\033[1m\033[41m�\033[40m���������\033[36m \033[37m�\033[47m�\033[45m��\033[35m\033[47m�\033[40m�\033[36m \033[31m�������\033[41m \033[0m\033[31m�\033[37m \033[1m�\033[35m\033[47m�\033[40m��\033[0m\033[35m�\033[1m\033[45m�\033[40m���\033[45m�\033[40m�\033[0m\033[35m�\033[37m  \033[1m\033[35m�����\033[45m�\033[40m���\033[32m�������\033[33m\033[42m��\033[0m\r\n"
"\033[1m\033[34m������\033[36m \033[0m\033[35m�\033[1m\033[45m�\033[40m�\033[47m��\033[37m\033[45m����\033[35m�\033[40m��\033[0m\033[35m�\033[1m\033[36m \033[0m\033[31m�����\033[1m\033[41m���\033[36m\033[40m \033[37m��\033[45m��\033[35m\033[47m��\033[40m��\033[36m \033[31m����\033[41m \033[0m\033[31m�\033[37m \033[1m�\033[45m��\033[35m\033[40m��\033[32m \033[0m \033[35m�\033[1m�\033[45m�\033[40m����\033[45m��\033[40m���\033[0m\033[35m�\033[1m\033[45m��\033[40m��\033[32m�����\033[43m�\033[33m\033[42m���\033[0m\r\n"
"\033[1m\033[34m���������\033[0m\033[35m�\033[1m�\033[45m�\033[37m��\033[35m\033[40m�\033[37m\033[45m����\033[35m\033[40m����\033[45m�\033[40m���\033[0m\033[35m�\033[1m\033[36m \033[0m\033[31m�\033[37m  \033[1m��\033[47m�\033[35m�\033[40m����\033[0m \033[31m��\033[37m  \033[1m�\033[46m�\033[45m�\033[0m\033[35m�\033[1m�\033[0m \033[1m\033[32m���\033[0m   \033[35m�\033[1m������\033[45m�\033[0m\033[35m��\033[1m���\033[32m ��\033[43m�\033[33m\033[42m�\033[40m����\033[0m\r\n"
"\033[1m\033[34m�������������\033[36m \033[0m\033[35m�\033[1m�\033[45m�\033[47m����\033[40m�����������\033[45m��\033[47m�\033[45m�\033[40m������ \033[32m��� \033[37m\033[45m�\033[32m\033[40m ���������������������\033[33m\033[42m���\033[40m���\033[37m�\033[0m\r\n"
"\033[1m\033[34m�������������������\033[36m \033[0m\033[35m�\033[1m����\033[45m�\033[40m������\033[45m�\033[40m��\033[0m\033[35m�\033[1m�����\033[45m�\033[0m\033[35m�\033[1m\033[32m  �����������������\033[33m\033[42m���\033[40m��������\033[0m\r\n"
"\033[1m\033[34m\033[44m�\033[40m�������������������\033[44m����\033[0m\033[34m������������\033[1m\033[35m���\033[45m�\033[0m\033[35m��\033[1m \033[32m�\033[33m\033[42m��\033[32m\033[40m������������\033[33m\033[42m�\033[40m�������\033[37m�\033[0m\r\n"
"\033[1m\033[34m���\033[42m���\033[40m������������������\033[44m� \033[0m\033[34m������\033[37m    \033[1m\033[35m����\033[0m\033[35m�\033[37m  \033[1m\033[33m��\033[32m����������\033[33m\033[42m�\033[40m����\033[0m\r\n"
"\033[1m\033[34m   ��������\033[42m���\033[40m������������\033[0m\033[34m�\033[37m \033[1m. ****** login already\033[32m��\033[43m��\033[33m\033[47m�\033[40m�\033[0m\r\n"
"\033[9C\033[1m\033[34m��������\033[42m�\033[40m��������\033[0m\033[34m� \033[1m\033[37m|\033[0m\033[20C\033[1m\033[30m  \033[0m  \033[1m�\033[0m\r\n"
"\033[16C\033[1m\033[34m����\033[42m�\033[40m�����\033[0m  \033[1m`--> \033[0m\033[31m\033[47m  HOLY **** I'M \033[5mCOMING\033[0m\033[31m\033[47m  \033[37m\033[40m \033[1m----,\033[0m"
"\033[20C\033[1m\033[34m�\033[47m�\033[43m��\033[30m\033[40m�\033[0m\033[37C\033[1m|\033[0m"
"\033[22C\033[1m \033[0m\033[14C\033[1mrekcufrehtomdrowssap\033[0m \033[1m<---'\033[0m"
"\033[52C-- TIFoaDAM (Ascii-art courtesy of http://sixteencolors.net/)"

);


// -=[ ���������� � ��� ]=-

uint8_t key;

//const char * nc_fkeys[10] = {
//
//    "Help",
//    "Menu",
//    "View",
//    "Edit",
//    "Copy",
//    "RenMov",
//    "Mkdir",
//    "Delete",
//    "Setup",
//    "Quit"
//
//};

const char * memview_fkeys[ 10 ] = {

    "Help", 0, 0, 0, "DynMem", 0, 0, 0, 0, "Quit"

};

const char * mainmenu[] = {

    "1. �������� ������",
    "2. ������� � �������",
    "3. ���� ��������",
    "4. ��������� ����",
    "5. �������������",
    "6. �������������",
    "7. ���� �����",
    "8. ���� � �����",
    "9. ������� ��������",
    0

};

const char * submenu1[] = {

    "[Europe] - Final Countdown",
    "[Corona] - Rythm of The Night",
    "[Jean Michel Jarre] - Oxygene 10",
    "[Titanic] - Theme",
    0

};

char hexchars[ 17 ] = "0123456789ABCDEF";

char buff[ 40 ];


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~
************************/


void DoPeriods( uint16_t Periods ) {

    do nop(); while ( Periods-- );
}


void Sound( uint16_t Pulses, uint16_t Periods ) {

    sbi( DDRD, DDD7 );

    do {

        sbi( PORTD, PD7 );
        DoPeriods( Periods );

        cbi( PORTD, PD7 );
        DoPeriods( Periods );

    } while ( Pulses-- );

}


uint8_t runmenu( uint8_t x, uint8_t y, const char * menu[], uint8_t defaultitem ) {

    uint8_t i, j, itemno;
    uint8_t nitems, width;
    const char * s;

    itemno = defaultitem - 1;
    width = 20;

    width = 10;
    nitems = 0;

    while ( menu[ nitems ] != 0 ) {

        for ( j = 0; menu[ nitems ][j]; j++ );

        if ( j > width ) width = j;

        nitems++;

    }

    width += 2;

    if ( itemno > nitems ) itemno = 0;

    while ( true ) {

        CConsole::CursorOff();
        CConsole::SetTextAttr( GREEN << 4 | WHITE );
        CConsole::GotoXY( x, y );
        CConsole::PutChar( ACS_ULCORNER );

        for ( i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

        CConsole::PutChar( ACS_URCORNER );

        for ( i = 0; i < nitems; i++ ) {

            CConsole::GotoXY( x, y + i + 1 );

            CConsole::PutChar( ACS_VLINE );	
            CConsole::PutChar( ' ' );

            if ( i == itemno ) CConsole::SetTextAttr( YELLOW );

            s = 0;

            for ( j = 0; j < width; j++ ) {

                if ( s && * s ) {

                    CConsole::PutChar( * s++, CConsole::cp1251 );

                } else {

                    CConsole::PutChar( ' ' );
                }

                if ( s == 0 ) s = ( const char * ) menu[i];

            }

            CConsole::SetTextAttr( GREEN << 4 | WHITE );
            CConsole::PutChar( ' ' );
            CConsole::PutChar( ACS_VLINE );

        }

        CConsole::GotoXY( x, y + nitems + 1 );
        CConsole::PutChar( ACS_LLCORNER );

        for ( i = 0; i < width + 2; i++ ) { CConsole::PutChar( ACS_HLINE ); }

        CConsole::PutChar( ACS_LRCORNER );

        // ������� �����
        do _delay_ms( 10 ); while ( !( key = CConsole::GetChar() ) );

        switch ( key ) {

            case 0x31: { itemno = 0; break; }
            case 0x32: { itemno = 1; break; }
            case 0x33: { itemno = 2; break; }
            case 0x34: { itemno = 3; break; }
            case 0x35: { itemno = 4; break; }
            case 0x36: { itemno = 5; break; }
            case 0x37: { itemno = 6; break; }
            case 0x38: { itemno = 7; break; }
            case 0x39: { itemno = 8; break; }

            case KB_ESC: {

                // ��������� ������ ����
                key = CConsole::GetChar();

                // ������ ������� ESC
                if ( key == 0 ) {

                    CConsole::CursorOn();
                    return 0;

                }

                if ( key == '[' ) {

                    // ��������� ����������� ��� �������
                    key = CConsole::GetChar();

                    switch ( key ) {

            case KB_UP: {

                if ( itemno > 0 ) itemno--;
                else itemno = nitems - 1;
                break;

                        }

            case KB_DOWN: {

                itemno++;
                itemno %= nitems;
                break;

                          }

                    }

                }

                break;

                         }

                         // ��������� ��� ��������� ZOC/Pro 5.05
                         //case 0xAB: {
                         //
                         //    if ( itemno > 0 ) itemno--;
                         //    else itemno = nitems - 1;
                         //    break;

                         //}

                         //case 0x2B: {
                         //
                         //    itemno++;
                         //    itemno %= nitems;
                         //    break;

                         //}

            case 0x0D: {

                CConsole::CursorOn();
                return itemno + 1;

                       }

            case KB_ENTER: {

                CConsole::CursorOn();
                return itemno + 1;

                           }

        }

    }

}


void drawfkeys( const char * fkeys[] ) {

    const char * s;

    CConsole::GotoXY( 1, 25 );

    for ( uint8_t i = 0; i < 10; i++ ) {

        CConsole::SetTextColor( WHITE );
        CConsole::SetTextBackground( BLACK );

        if ( i > 0 && i < 9 ) CConsole::PutChar( ' ' );

        if ( i == 9 ) {

            CConsole::PutChar( '1' );
            CConsole::PutChar( '0' );

        } else {

            CConsole::PutChar( ( i % 10 ) + '1' );
        }

        CConsole::SetTextColor( WHITE );
        CConsole::SetTextBackground( GREEN );

        s = fkeys[i] ? fkeys[i] : 0;

        for ( uint8_t j = 0; j < 6; j++ ) {

            if ( s && * s ) {

                CConsole::PutChar( * s++ );

            } else {

                CConsole::PutChar( ' ' );
            }

        }

    }

}


void drawframe( uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color ) {

    CConsole::SetTextAttr( color );
    CConsole::GotoXY( x, y );

    CConsole::PutChar( ACS_ULCORNER );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_URCORNER );

    for ( uint8_t i = 0; i < height; i++ ) {

        CConsole::GotoXY( x, y + i + 1 );
        CConsole::PutChar( ACS_VLINE );
        CConsole::PutChar( ' ' );

        for ( uint8_t j = 0; j < width; j++ ) CConsole::PutChar( ' ' );

        CConsole::PutChar( ' ' );

        CConsole::PutChar( ACS_VLINE );

    }

    CConsole::GotoXY( x, y + height + 1 );

    CConsole::PutChar( ACS_LLCORNER );

    for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

    CConsole::PutChar( ACS_LRCORNER );

}


void drawmem( const unsigned char * mem ) {

    const unsigned char * mptr;
    uint8_t i, j;

    for ( i = 0; i < 23; i++ ) {

        mptr = & mem[ i * 16 ];

        CConsole::GotoXY( 1, i + 2 );

        CConsole::SetTextColor( YELLOW );

        CConsole::SetTextBackground( BLUE );

        j = 8;

        do {

            j--;
            CConsole::PutChar( hexchars[ ( ( ( uint16_t ) mptr ) >> ( j * 4 ) ) & 0xF ] );

        } while ( j > 0 );

        CConsole::WriteString( ":  " );

        CConsole::SetTextColor( WHITE );

        for ( j = 0; j < 16; j++ ) {

            CConsole::PutChar( hexchars[ * mptr >> 4 ] );
            CConsole::PutChar( hexchars[ * mptr & 0xF ] );
            mptr++;
            CConsole::PutChar( ' ' );

            if ( j == 7 ) {

                CConsole::PutChar( '-' );
                CConsole::PutChar( ' ' );

            }

        }

        CConsole::PutChar( ACS_VLINE );
        CConsole::PutChar( ' ' );

        mptr = & mem[ i * 16 ];

        for ( j = 0; j < 16; j++ ) {

            if ( * mptr >= ' ' ) {

                CConsole::PutChar( * mptr, CConsole::cp1251 );

            } else {

                CConsole::PutChar( '.' );
            }

            mptr++;

        }

        CConsole::ClearEndOfLine();

    }

}


void MemViewer( void * start ) {

    FLASHSTR_DECLARE( char, szViewMemory, "�������� ������: ��� (CP1251)" );

    uint16_t key;
    const unsigned char * mem;

    CConsole::SetTextAttr( LIGHTGRAY | BLUE << 4 );
    CConsole::ClearScreen();

    CConsole::SetTextColor( WHITE );
    CConsole::SetTextBackground( GREEN );
    CConsole::WriteString( szViewMemory, CConsole::cp1251 );
    CConsole::ClearEndOfLine();

    CConsole::CursorOff();
    drawfkeys( memview_fkeys );

    mem = ( const unsigned char * ) start;

    for (;;) {

        drawmem( mem );

        do _delay_ms( 10 ); while ( !( key = CConsole::GetChar() ) );

        switch ( key ) {

            case KB_LEFT: {

                if ( ( uint16_t ) mem > 0 ) ( uint16_t ) mem--;
                break;
                          }

            case KB_RIGHT: {

                mem++;
                break;
                           }

            case KB_UP: {

                if ( mem >= ( unsigned char const * ) 16 ) mem -= 16;
                break;
                        }

            case KB_DOWN: {

                mem += 16;
                break;
                          }

            case KB_PGUP: {

                if ( mem >= ( unsigned char const * ) ( 16 * 23 ) )  mem -= 16 * 23;
                break;

                          }

            case KB_PGDN: {

                mem += 16 * 23;
                break;

                          }

            case KB_F10: {

                CConsole::CursorOn();
                return;

                         }

            case KB_ESC: {

                CConsole::CursorOn();
                return;

                         }

        }

    }

}


void DebugDemo() {

    FLASHSTR_DECLARE( char, szDebugMode, "����� �������:\r\n" );
    FLASHSTR_DECLARE( char, szKeyPressed, "�� ������ �������: " );

    uint8_t key;

    CConsole::SetTextAttr( LIGHTGRAY );
    CConsole::ClearScreen();

    CConsole::WriteString( szDebugMode, CConsole::cp1251 );

    while ( true ) {

        do _delay_ms( 10 ); while ( !( key = CConsole::GetChar() ) );

        if ( key == KB_ESC ) break;

        CConsole::WriteString( szKeyPressed, CConsole::cp1251 );

        CConsole::PutChar( key );

        CConsole::WriteString( "\r\n" );

    }

}


void Command() {

    FLASHSTR_DECLARE( char, szInterpreterName, "�������������, ������ " );
    FLASHSTR_DECLARE( char, szBuildDate, "���� ������ �������: " );
    FLASHSTR_DECLARE( char, szInterpreterAuthor, "\r\n�����: �������� �������� (unihomelab@ya.ru)\r\n\r\n" );
    FLASHSTR_DECLARE( char, szCommandPrompt, "[ATmega16]$ " );
    FLASHSTR_DECLARE( char, szAvailableCommands, "\r\n��������� �������:\r\n" );
    FLASHSTR_DECLARE( char, szHelpDescription, " - (help) ����� ���������.\r\n" );
    FLASHSTR_DECLARE( char, szQuitDescription, " - (quit) �����.\r\n" );
    FLASHSTR_DECLARE( char, szCommandIsNotSupported, "\r\n������� �� ��������������. ������� " );
    FLASHSTR_DECLARE( char, szForHelp, " (help) ��� ������.\r\n" );

    char * cmd;

    CConsole::SetTextAttr( LIGHTGRAY );
    CConsole::ClearScreen();

    CConsole::GotoXY( 1, 25 );

    CConsole::WriteString( szInterpreterName, CConsole::cp1251 );
    CConsole::WriteString( Version );
    CConsole::WriteString( "\r\n" );

    CConsole::WriteString( szBuildDate, CConsole::cp1251 );
    CConsole::WriteString( CVersion::GetBuildDateString(), CConsole::cp1251 );

    CConsole::WriteString( szInterpreterAuthor, CConsole::cp1251 );

    while ( true ) {

        // ������� �����������
        CConsole::SetTextAttr( GREEN );
        CConsole::WriteString( szCommandPrompt );

        // ��������� ���� ������������
        CConsole::SetTextAttr( LIGHTGRAY );
        cmd = CConsole::ReadString( buff );

        // ���� ������ �������, �� ��������� �� ��������� ������
        if ( cmd[0] == 0 ) {

            CConsole::WriteString( "\r\n" );

        // ������� �������
        } else if ( ( cmd[0] == 'h' ) && ( cmd[1] == 0 ) ) {

            CConsole::SetTextAttr( WHITE );
            CConsole::WriteString( szAvailableCommands, CConsole::cp1251 );

            CConsole::SetTextAttr( LIGHTRED );
            CConsole::PutChar( 'h' );

            CConsole::SetTextAttr( WHITE );
            CConsole::WriteString( szHelpDescription, CConsole::cp1251 );

            CConsole::SetTextAttr( LIGHTRED );
            CConsole::PutChar( 'q' );

            CConsole::SetTextAttr( WHITE );
            CConsole::WriteString( szQuitDescription, CConsole::cp1251 );

        // ������� �� ��������������
        } else if ( ( cmd[0] == 'q' ) && ( cmd[1] == 0 ) ) {

            return;

        // ������� ��������� � ���������������� �������
        } else {

            CConsole::SetTextAttr( WHITE );
            CConsole::WriteString( szCommandIsNotSupported, CConsole::cp1251 );

            CConsole::SetTextAttr( LIGHTRED );
            CConsole::WriteString( "h" );

            CConsole::SetTextAttr( WHITE );
            CConsole::WriteString( szForHelp, CConsole::cp1251 );

        }

    }

}


void SpeedTest() {

    FLASHSTR_DECLARE( char, szTestPrompt, "TEST50=>" );
    FLASHSTR_DECLARE( char, szTestStr, "\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08" );

    CConsole::GotoXY( 1, 1 );

    CConsole::SetTextAttr( GREEN << 4 | YELLOW );

    for ( uint16_t i = 0; i < 50000; i++ ) {

        CConsole::WriteString( szTestPrompt );
        CConsole::PutChar( ( ( i / 10000 ) % 10 ) + '0' );
        CConsole::PutChar( ( ( i / 1000 ) % 10 ) + '0' );
        CConsole::WriteString( szTestStr );
    }

}


void SubmenuTest() {

    uint8_t item = 0;

    item = runmenu( 8, 10, submenu1, item );

    switch ( item ) {

        case 1: {

            // Europe :: Final Countdown
            Sound( 124U, 675U );    // H2(1/16)
            Sound( 110U, 758U );    // A2(1/16)
            Sound( 248U, 675U );    // H2(1/8)
            _delay_ms( 125U );      // P(1/16)
            Sound( 165U, 1011U );   // E2(1/8)
            _delay_ms( 250U );      // P(1/8)
            _delay_ms( 125U );      // P(1/16)
            Sound( 131U, 637U );    // C3(1/16)
            Sound( 124U, 675U );    // H2(1/16)
            Sound( 131U, 637U );    // C3(1/16)
            _delay_ms( 125U );      // P(1/16)
            Sound( 124U, 675U );    // H2(1/16)
            _delay_ms( 125U );      // P(1/16)
            Sound( 220U, 758U );    // A2(1/8)
            _delay_ms( 125U );      // P(1/16)
            _delay_ms( 250U );      // P(1/8)
            Sound( 131U, 637U );    // C3(1/16)
            Sound( 124U, 675U );    // H2(1/16)
            Sound( 262U, 637U );    // C3(1/8)
            _delay_ms( 125U );      // P(1/16)
            Sound( 165U, 1011U );   // E2(1/8)
            _delay_ms( 250U );      // P(1/8)
            _delay_ms( 125U );      // P(1/16)
            Sound( 110U, 758U );    // A2(1/16)
            Sound( 98U, 850U );     // G2(1/16)
            Sound( 110U, 758U );    // A2(1/16)
            _delay_ms( 125U );      // P(1/16)
            Sound( 98U, 850U );     // G2(1/16)
            _delay_ms( 125U );      // P(1/16)
            Sound( 92U, 901U );     // Fis2(1/16)
            _delay_ms( 125U );      // P(1/16)
            Sound( 110U, 758U );    // A2(1/16)
            _delay_ms( 125U );      // P(1/16)
            Sound( 196U, 850U );    // G2(1/8)

            break;
                }

    };

}


void AsciiArt() {

    CConsole::SetTextAttr( LIGHTGRAY );
    CConsole::ClearScreen();
    CConsole::WriteString( ascii_art_str );

    do _delay_ms( 10 ); while ( CConsole::GetChar() != KB_ESC );

}


void TestCGetS() {

    FLASHSTR_DECLARE( char, szEnterYourName, "������� ��� ���:" );
    FLASHSTR_DECLARE( char, szYourName, "���� ���:\n" );
    FLASHSTR_DECLARE( char, szYouDidntEnterAName, "�� �� ����� ���!" );
    FLASHSTR_DECLARE( char, szPressEscForExit, "������� ESC ��� ������" );

    char * out;
    uint8_t i;

    //draw input dialog (frame)
    drawframe( 10, 10, 40, 9, WHITE  | RED << 4 );
    CConsole::GotoXY( 12, 12 );
    CConsole::WriteString( szEnterYourName, CConsole::cp1251 );

    CConsole::GotoXY( 12, 14 );
    CConsole::SetTextAttr( WHITE  | BLACK << 4 );

    for ( i = 0; i < 40; i++ ) CConsole::PutChar( ' ' );

    //read data
    CConsole::GotoXY( 12,14 );

    out = CConsole::ReadString( buff );

    // write read data
    CConsole::GotoXY( 12, 16 );
    CConsole::SetTextAttr( WHITE | RED << 4 );

    if ( strlen( out ) == 0 ) {

        CConsole::WriteString( szYouDidntEnterAName, CConsole::cp1251 );

    } else {

        CConsole::WriteString( szYourName, CConsole::cp1251 );

        CConsole::GotoXY( 12, 17 );
        CConsole::WriteString( out );

    }

    CConsole::CursorOff();

    CConsole::GotoXY( 23, 19 );
    CConsole::SetTextAttr( WHITE | RED << 4 );
    CConsole::WriteString( szPressEscForExit, CConsole::cp1251 );

    // wait for ESC press
    do _delay_ms( 10 ); while ( CConsole::GetChar() != KB_ESC );

}


void DateTime() {

    FLASHSTR_DECLARE( char, szDateTime, "���� � �����" );
    FLASHSTR_DECLARE( char, szDate, "���� (��:��:����)       " );
    FLASHSTR_DECLARE( char, szTime, "����� (��:��:��)          " );

    uint8_t width = 34;

    CConsole::SetTextAttr( LIGHTGRAY | BLUE << 4 );
    CConsole::ClearScreen();

    CConsole::SetTextColor( WHITE );
    CConsole::SetTextBackground( GREEN );
    CConsole::WriteString( szDateTime, CConsole::cp1251 );
    CConsole::ClearEndOfLine();

    while ( true ) {

        CSystemTime::GetTimeAsSystemTime( & Time );

        CConsole::CursorOff();
        CConsole::SetTextAttr( GREEN << 4 | WHITE );
        CConsole::GotoXY( 4, 4 );
        CConsole::PutChar( ACS_ULCORNER );

        for ( uint8_t i = 0; i < width + 2; i++ ) CConsole::PutChar( ACS_HLINE );

        CConsole::PutChar( ACS_URCORNER );

        CConsole::GotoXY( 4, 5 );
        CConsole::PutChar( ACS_VLINE );	
        CConsole::PutChar( ' ' );
        CConsole::WriteString( szDate, CConsole::cp1251 );

        CConsole::PutChar( Time.wDay / 10 + '0' );
        CConsole::PutChar( Time.wDay % 10 + '0' );

        CConsole::PutChar( '-' );

        CConsole::PutChar( Time.wMonth / 10 + '0' );
        CConsole::PutChar( Time.wMonth % 10 + '0' );

        CConsole::PutChar( '-' );

        uint16_t tmp = Time.wYear;
        CConsole::PutChar( ( uint8_t ) ( tmp / 1000U ) + '0' );

        tmp %= 1000U;
        CConsole::PutChar( ( uint8_t ) ( tmp / 100 ) + '0' );

        tmp %= 100;
        CConsole::PutChar( ( uint8_t ) ( tmp / 10 ) + '0' );
        CConsole::PutChar( ( uint8_t ) ( tmp % 10 ) + '0' );

        CConsole::PutChar( ' ' );
        CConsole::PutChar( ACS_VLINE );	

        CConsole::GotoXY( 4, 6 );
        CConsole::PutChar( ACS_VLINE );	
        CConsole::PutChar( ' ' );
        CConsole::WriteString( szTime, CConsole::cp1251 );

        CConsole::PutChar( Time.wHour / 10 + '0' );
        CConsole::PutChar( Time.wHour % 10 + '0' );
        CConsole::PutChar( ':' );
        CConsole::PutChar( Time.wMinute / 10 + '0' );
        CConsole::PutChar( Time.wMinute % 10 + '0' );
        CConsole::PutChar( ':' );
        CConsole::PutChar( Time.wSecond / 10 + '0' );
        CConsole::PutChar( Time.wSecond % 10 + '0' );

        CConsole::PutChar( ' ' );
        CConsole::PutChar( ACS_VLINE );

        CConsole::GotoXY( 4, 7 );
        CConsole::PutChar( ACS_LLCORNER );

        for ( uint8_t i = 0; i < width + 2; i++ ) { CConsole::PutChar( ACS_HLINE ); }

        CConsole::PutChar( ACS_LRCORNER );

        if ( CConsole::GetChar() == KB_ESC ) return; else _delay_ms( 1000U );

    }

}


void ShowTable() {

    drawframe( 10, 5, 34, 15, DARKGRAY );
	
    do _delay_ms( 10 ); while ( CConsole::GetChar() != KB_ESC );

}


void UIMain() {

    FLASHSTR_DECLARE( char, szTitle, "Pinboard II, MicroVGA, ATmega16 @ 16 ���, ������ ������: " );
    FLASHSTR_DECLARE( char, szAuthor, "����� ������: �������� �. �. (unihomelab@ya.ru)" );
    FLASHSTR_DECLARE( char, szProjectURL, "������: https://mysvn.ru/avr/examples/uVGA/" );
    FLASHSTR_DECLARE( char, szGCC, " (gnu)" );
    FLASHSTR_DECLARE( char, szIAR, " (iar)" );

    uint8_t i, j;
    uint8_t item = 0;

    do {

        CConsole::SetTextBackground( BLUE );
        CConsole::ClearScreen();

        CConsole::SetTextColor( WHITE );
        CConsole::SetTextBackground( GREEN );
        CConsole::WriteString( szTitle, CConsole::cp1251 );
        CConsole::WriteString( Version );

#ifdef __GNUC__
        CConsole::WriteString( szGCC );
#elif defined __ICCAVR__
        CConsole::WriteString( szIAR );
#endif

        CConsole::ClearEndOfLine();
	
        CConsole::SetTextColor( DARKGRAY );
        CConsole::SetTextBackground( BLUE );
        CConsole::GotoXY( 1, 24 );
        CConsole::WriteString( szAuthor, CConsole::cp1251 );
        CConsole::GotoXY( 1, 25 );
        CConsole::WriteString( szProjectURL, CConsole::cp1251 );

        // draw "logo"
        for ( i = 0; i < 13; i++ ) {

            CConsole::GotoXY( 46, 11 + i );
	
            for ( j = 0; j < 32; j++ ) {
	
                ( uvga_logo[ i * 32 + j ] == '#' ) ? CConsole::PutChar( 219 ) : CConsole::PutChar( uvga_logo[ i * 32 + j ] );
            }

	    }

	    item = runmenu( 5, 5, mainmenu, item );
	
        switch ( item ) {
	
            case 1: { MemViewer(0);     break; }
            case 2: { DebugDemo();      break; }
            case 3: { SpeedTest();      break; }
            case 4: { SubmenuTest();    break; }
            case 5: { Command();        break; }
            case 6: { AsciiArt();       break; }
            case 7: { TestCGetS();      break; }
            case 8: { DateTime();       break; }
            case 9: { ShowTable();      break; }

	    }

    } while ( true );

}