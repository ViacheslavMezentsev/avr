///////////////////////////////////////////////////////////
//  LCD.cpp
//  Implementation of the Class CLCD
//  Created on:      16-���-2011 6:53:36
//  Original author: �����
///////////////////////////////////////////////////////////

#include "Defines.h"
#include "Configuration.h"
#include "LCD.h"

// ��������:
// �� �������� ��������� ������ � ������� ��� ���������� ���
// (��. ������ � LCD.h), � ���� ������ ���������� �� �����!
// ����� ������ ��� ������ ����������� ������������� ��������������
// ��� �� ����������.

// LCD display geometry
// change these definitions to adapt setting
//#define LCD_LINES           4     // visible lines
//#define LCD_LINE_LENGTH    20     // line length (in characters)

#define HD44780_CONFIG_PAUSE_START_MS   50
//#define HD44780_CONFIG_PAUSE_CMD_START_US   60

// ����� ������ ��� ����������� HD44780 (�� ��������)
// ������:
#define LCD_HD44780_CMD_CLEAR           0x01    // clear display
#define LCD_HD44780_CMD_RETURN_HOME     0x02    // return to home position

#define LCD_HD44780_CMD_ENTRY_MODE      2      // DB2: set entry mode
#define LCD_HD44780_CMD_ENTRY_INC       1      // DB1: increment
#define LCD_HD44780_CMD_ENTRY_SHIFT     0      // DB2: shift

#define LCD_HD44780_CMD_ON_CTRL         3      // DB3: turn lcd/cursor on
#define LCD_HD44780_CMD_ON_DISPLAY      2      // DB2: turn display on
#define LCD_HD44780_CMD_ON_CURSOR       1      // DB1: turn cursor on
#define LCD_HD44780_CMD_ON_BLINK        0      // DB0: blinking cursor

#define LCD_HD44780_CMD_MOVE            4      // DB4: move cursor/display
#define LCD_HD44780_CMD_MOVE_DISP       3      // DB3: move display (0-> move cursor)
#define LCD_HD44780_CMD_MOVE_RIGHT      2      // DB2: move right (0-> left)

#define LCD_HD44780_CMD_FUNCTION        5      // DB5: function set
#define LCD_HD44780_CMD_FUNCTION_8BIT   4      // DB4: set 8BIT mode (0->4BIT mode)
#define LCD_HD44780_CMD_FUNCTION_2LINES 3      // DB3: two lines (0->one line)
#define LCD_HD44780_CMD_FUNCTION_10DOTS 2      // DB2: 5x10 font (0->5x7 font)

#define LCD_HD44780_CMD_CGRAM           6      // DB6: set CG RAM address
#define LCD_HD44780_CMD_DDRAM           7      // DB7: set DD RAM address

// ������:
#define LCD_HD44780_BUSY_BIT            7      // DB7: LCD is busy

// �������������� ���������������� ��������
// ����������: ����� �������������� ����� ��������� ������� � ��� ��� ��� ������ 
// ������ LoadCustomChar(), ������ �������� �������� � ���� ������������� ������� 
// (�� ���� ��� ����� �������� � ������� LcdCustomChar[])
#define LCDCHAR_PROGRESS05          0       // 0/5 full progress block
#define LCDCHAR_PROGRESS15          1       // 1/5 full progress block
#define LCDCHAR_PROGRESS25          2       // 2/5 full progress block
#define LCDCHAR_PROGRESS35          3       // 3/5 full progress block
#define LCDCHAR_PROGRESS45          4       // 4/5 full progress block
#define LCDCHAR_PROGRESS55          5       // 5/5 full progress block
#define LCDCHAR_REWINDARROW         6       // rewind arrow
#define LCDCHAR_STOPBLOCK           7       // stop block
#define LCDCHAR_PAUSEBARS           8       // pause bars
#define LCDCHAR_FORWARDARROW        9       // fast-forward arrow
#define LCDCHAR_SCROLLUPARROW       10      // scroll up arrow
#define LCDCHAR_SCROLLDNARROW       11      // scroll down arrow
#define LCDCHAR_BLANK               12      // scroll down arrow
#define LCDCHAR_ANIPLAYICON0        13      // animated play icon frame 0
#define LCDCHAR_ANIPLAYICON1        14      // animated play icon frame 1
#define LCDCHAR_ANIPLAYICON2        15      // animated play icon frame 2
#define LCDCHAR_ANIPLAYICON3        16      // animated play icon frame 3

#define SCROLL_RIGHT 0
#define SCROLL_LEFT  1


// �������������� ����� ��������
#ifdef __ICCAVR__
    __flash uint8_t LcdCustomChar[136] = {
#elif defined __GNUC__
    PROGMEM uint8_t _LcdCustomChar[136] = {
#endif
// ������ ��� ������������ ������������������ ����, �����������
// ���������������� ������:

// ������ 5x8          ������: 2/5 full progress block

// 1 ����: ---00000    0x00    ---00000
// 2 ����: ---00000    0x1F    ---11111    *****
// 3 ����: ---00000    0x18    ---11000    **   
// 4 ����: ---00000    0x18    ---11000    **   
// 5 ����: ---00000    0x18    ---11000    **   
// 6 ����: ---00000    0x18    ---11000    **   
// 7 ����: ---00000    0x1F    ---11111    *****
// 8 ����: ---00000    0x00    ---00000

    0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // 0. 0/5 full progress block
    0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, // 1. 1/5 full progress block
    0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, // 2. 2/5 full progress block
    0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00, // 3. 3/5 full progress block
    0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00, // 4. 4/5 full progress block
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
    0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x00, // 6. rewind arrow
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 7. stop block
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x00, // 8. pause bars
    0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00, // 9. fast-forward arrow
    0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x1F, 0x00, // 10. scroll up arrow
    0x00, 0x1F, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x00, // 11. scroll down arrow
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 12. blank character
    0x00, 0x0E, 0x19, 0x15, 0x13, 0x0E, 0x00, 0x00, // 13. animated play icon frame 0
    0x00, 0x0E, 0x15, 0x15, 0x15, 0x0E, 0x00, 0x00, // 14. animated play icon frame 1
    0x00, 0x0E, 0x13, 0x15, 0x19, 0x0E, 0x00, 0x00, // 15. animated play icon frame 2
    0x00, 0x0E, 0x11, 0x1F, 0x11, 0x0E, 0x00, 0x00, // 16. animated play icon frame 3
};
#ifdef __GNUC__
    FU08T_PTR LcdCustomChar(_LcdCustomChar);
#endif


/*   +----------------+
     |������� ��������|
     |Windows (ANSI)  |
     +----------------+      +----------------+
     |0123456789ABCDEF|      |0123456789ABCDEF|
     +----------------+      +----------------+
0x00 |( ��������� )   | 0x80 |( ��������� )   |
0x10 |( ��������� )   | 0x90 |( ��������� )   |
0x20 | !"#$%&'()*+,-./| 0xA0 |( ��������� )   |
0x30 |0123456789:;<=>?| 0xB0 |( ��������� )   |
0x40 |@ABCDEFGHIJKLMNO| 0xC0 |����������������|
0x50 |PQRSTUVWXYZ[\]^_| 0xD0 |����������������|
0x60 |`abcdefghijklmno| 0xE0 |����������������|
0x70 |pqrstuvwxyz{|}~ | 0xF0 |����������������|
     +----------------+      +----------------+
*/

// ������� ������������� �� CP1251 (WIN)
#ifdef __ICCAVR__
    __flash uint8_t CP1251_TO_CPHD44780[ 256 ] = {
#elif defined __GNUC__
    PROGMEM uint8_t _CP1251_TO_CPHD44780[ 256 ] = {
#endif
//PROGMEM unsigned char CP1251_TO_CPHD44780[ 256 ] = {
      /* +---------------------------------------------------------------------------------------------------------------+
         |  00  |  01  |  02  |  03  |  04  |  05  |  06  |  07  |  08  |  09  |  0A  |  0B  |  0C  |  0D  |  0E  |  0F  | */
/* CGRAM:  (1)    (2)    (3)    (4)    (5)    (6)    (7)    (8)    (1)    (2)    (3)    (4)    (5)    (6)    (7)    (8)    */
/* 0x00 */ 0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0A , 0x0B , 0x0C , 0x0D , 0x0E , 0x0F , //

/* 0x10 */ 0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1A , 0x1B , 0x1C , 0x1D , 0x1E , 0x1F , //
/* 0x20 */ 0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 , 0x28 , 0x29 , 0x2A , 0x2B , 0x2C , 0x2D , 0x2E , 0x2F , //
/* 0x30 */ 0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 , 0x38 , 0x39 , 0x3A , 0x3B , 0x3C , 0x3D , 0x3E , 0x3F , //
/* 0x40 */ 0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 , 0x48 , 0x49 , 0x4A , 0x4B , 0x4C , 0x4D , 0x4E , 0x4F , //
/* 0x50 */ 0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 , 0x58 , 0x59 , 0x5A , 0x5B , 0x20 , 0x5D , 0x5E , 0x5F , //
/* 0x60 */ 0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 , 0x68 , 0x69 , 0x6A , 0x6B , 0x6C , 0x6D , 0x6E , 0x6F , //
/* 0x70 */ 0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 , 0x78 , 0x79 , 0x7A , 0x5B , 0x20 , 0x5D , 0xE9 , 0x7F , //
       /*|  00  |  01  |  02  |  03  |  04  |  05  |  06  |  07  |  08  |  09  |  0A  |  0B  |  0C  |  0D  |  0E  |  0F  | */
/* 0x80 */ 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , //
/* 0x90 */ 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , //
/* 0xA0 */ 0xCB , 0x20 , 0x49 , 0x69 , 0x04 , 0x20 , 0xFE , 0xEF , 0xA2 , 0xCC , 0x00 , 0xC9 , 0x20 , 0x20 , 0x20 , 0x02 , //
/* 0xB0 */ 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xB5 , 0x4E , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , //
/* 0xC0 */ 0x41 , 0xA0 , 0x42 , 0xA1 , 0xE0 , 0x45 , 0xA3 , 0xA4 , 0xA5 , 0xA6 , 0x4B , 0xA7 , 0x4D , 0x48 , 0x4F , 0xA8 , //
/* 0xD0 */ 0x50 , 0x43 , 0x54 , 0xA9 , 0xAA , 0x58 , 0xE1 , 0xAB , 0xAC , 0xE2 , 0xAD , 0xAE , 0x62 , 0xAF , 0xB0 , 0xB1 , //
/* 0xE0 */ 0x61 , 0xB2 , 0xB3 , 0xB4 , 0xE3 , 0x65 , 0xB6 , 0xB7 , 0xB8 , 0xB9 , 0xBA , 0xBB , 0xBC , 0xBD , 0x6F , 0xBE , //
/* 0xF0 */ 0x70 , 0x63 , 0xBF , 0x79 , 0xE4 , 0x78 , 0xE5 , 0xC0 , 0xC1 , 0xE6 , 0xC2 , 0xC3 , 0xC4 , 0xC5 , 0xC6 , 0xC7 , //
      /* |  00  |  01  |  02  |  03  |  04  |  05  |  06  |  07  |  08  |  09  |  0A  |  0B  |  0C  |  0D  |  0E  |  0F  |
         +---------------------------------------------------------------------------------------------------------------+ */
};
#ifdef __GNUC__
    FU08T_PTR CP1251_TO_CPHD44780(_CP1251_TO_CPHD44780);
#endif


// ����������� �������������� ���������� ���������� �����������
// ���� ������

// ���������� ����� ����������
uint8_t CLCD::Rows = LCD_LINES;

// ���������� �������� ����������
uint8_t CLCD::Cols = LCD_LINE_LENGTH;

// ��� ������������� ��������
CLCD::EnLCDDriverType CLCD::Driver = LCD_DRIVER_NONE;


/***********************
*  � � � � � � � � � �
*  ~~~~~~~~~~~~~~~~~~~ 
************************/

/**
 * ��������� LCD �������
 */
void CLCD::Initialization(EnLCDDriverType DriverType){

    SetLCDDriver( DriverType );

    switch ( DriverType ) {

        case LCD_DRIVER_NONE: {
            break;
        }

        case LCD_DRIVER_HD44780: {

            Init_HD44780();
            break;
        }

        case LCD_DRIVER_USER: {

            Init_User();
            break;
        }

        default:;

    };

}


/**
 * ������� �������
 */
void CLCD::Clear(){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    WriteData( TYPE_COMMAND, LCD_HD44780_CMD_CLEAR );

    _delay_ms( LCD_DELAYS_CLEAR_MS );

}


/**
 * ������� ASCIIZ-������ �� ����-������
 */
void CLCD::WriteLine(uint8_t Line, FCHAR_PTR pString){

    WriteString( pString, Line, 0 );

}


/**
 * ������� ASCIIZ-������ �� ���
 */
void CLCD::WriteLine(uint8_t Line, char * pString){

    WriteString( pString, Line, 0 );

}

/**
 * ����� ������ �� ����� E
 */
void CLCD::WriteStrobe(uint16_t usec){

    // ��������� �������� ����� ������
    sbi( LCD_DDR_NAME_En, LCD_PIN_En );
    sbi( LCD_PORT_NAME_En, LCD_PIN_En );
    nop();
    cbi( LCD_PORT_NAME_En, LCD_PIN_En );

    // �������� �� ���������� �������� ������
    // �� ����� 1,2 ���
    _delay_us(2);

    // ���������������� ��������
    while( usec-- ) _delay_us(1);

}

/**
 * ������ �������� ��������� ������
 */
void CLCD::WriteLow4bits(uint8_t data){

    // ��������! ����� LCD_RS, LCD_RW ������ ���� ����������� ��
    // �������� �� ����
   
    // ����������� ����� �� �����
    sbi( LCD_DDR_NAME_D4, LCD_PIN_D4 );
    sbi( LCD_DDR_NAME_D5, LCD_PIN_D5 );
    sbi( LCD_DDR_NAME_D6, LCD_PIN_D6 );
    sbi( LCD_DDR_NAME_D7, LCD_PIN_D7 );

    if ( data & 0x01 ) sbi( LCD_PORT_NAME_D4, LCD_PIN_D4 ); else cbi( LCD_PORT_NAME_D4, LCD_PIN_D4 );
    if ( data & 0x02 ) sbi( LCD_PORT_NAME_D5, LCD_PIN_D5 ); else cbi( LCD_PORT_NAME_D5, LCD_PIN_D5 );
    if ( data & 0x04 ) sbi( LCD_PORT_NAME_D6, LCD_PIN_D6 ); else cbi( LCD_PORT_NAME_D6, LCD_PIN_D6 );
    if ( data & 0x08 ) sbi( LCD_PORT_NAME_D7, LCD_PIN_D7 ); else cbi( LCD_PORT_NAME_D7, LCD_PIN_D7 );

}


/**
 * ������ ��������� ������
 */
uint8_t CLCD::ReadLow4bits(){
   
    uint8_t data = 0;
    
    // ���� ������������� ��������� ���������, �� ���������� ��
    if ( SFIOR & ( 1 << PUD ) ) cbi( SFIOR, PUD );
    
    // ����������� ����� �� ����
    cbi( LCD_DDR_NAME_D4, LCD_PIN_D4 );
    cbi( LCD_DDR_NAME_D5, LCD_PIN_D5 );
    cbi( LCD_DDR_NAME_D6, LCD_PIN_D6 );
    cbi( LCD_DDR_NAME_D7, LCD_PIN_D7 );

    sbi( LCD_PORT_NAME_D4, LCD_PIN_D4 );
    sbi( LCD_PORT_NAME_D5, LCD_PIN_D5 );
    sbi( LCD_PORT_NAME_D6, LCD_PIN_D6 );
    sbi( LCD_PORT_NAME_D7, LCD_PIN_D7 );

    _delay_us(1);

    if CHECKBIT( LCD_PIN_NAME_D4, LCD_PIN_D4 ) data |= 0x01;
    if CHECKBIT( LCD_PIN_NAME_D5, LCD_PIN_D5 ) data |= 0x02;
    if CHECKBIT( LCD_PIN_NAME_D6, LCD_PIN_D6 ) data |= 0x04;
    if CHECKBIT( LCD_PIN_NAME_D7, LCD_PIN_D7 ) data |= 0x08;
            
    return data;
    
}


/**
 * ����� ������� �� ���
 */
void CLCD::PutChar(uint8_t Data){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    char ch = CP1251_TO_CPHD44780[ Data ];

    // ���� ���� ������� �������, �� ��������� �� ��������� ������
    if ( ch == '\n' ) {

        Goto( 1, 0 );

    } else {

        WriteData( TYPE_DATA, ch );

    }

}

/**
 * ����� ������� �� ���
 */
void CLCD::PutChar(uint8_t Data, uint8_t Row, uint8_t Col){

    // ��������� � ������ �������
    Goto( Row, Col );

    PutChar( Data );

}

/**
 * ����� �����
 */
void CLCD::ShiftLeft(){

    Scroll( SCROLL_LEFT );

}

/**
 * ����� ������
 */
void CLCD::ShiftRight(){

    Scroll( SCROLL_RIGHT );
}

void CLCD::Scroll(uint8_t Direction){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    switch ( Direction ) {

        // shift display to right
        case SCROLL_RIGHT: {

            WriteData( TYPE_COMMAND, 0x1C );
            break;
        }

        // shift display to left
        case SCROLL_LEFT : {

            WriteData( TYPE_COMMAND, 0x18 );
            break;
        }
    }

}


/**
 * ��������� ����������������� �������� LCD �������
 */
void CLCD::Init_User(){

}

/**
 * ��������� HD44780 �������� LCD �������
 */
void CLCD::Init_HD44780(){

    // ������������� ������ ������� ����� LCD_RS (�������)
    sbi( LCD_DDR_NAME_RS, LCD_PIN_RS );
    cbi( LCD_PORT_NAME_RS, LCD_PIN_RS );

    #ifndef LCD_RW_UNUSED

        // ������������� ������ ������� ����� LCD_RW
        sbi( LCD_DDR_NAME_RW, LCD_PIN_RW );
        cbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

    #endif
    
    // �������� ����� 40 ��, ���� �� ���������� ����������
    // �������� �� ����� �������

    #ifdef HD44780_CONFIG_PAUSE_START_MS

        _delay_ms( HD44780_CONFIG_PAUSE_START_MS );

    #endif

    // ��������� ������� �����������
    // ...

    // ����� ����� �������� ������. ��� ��������� LCD ��� ����������.
    #ifdef HD44780_CONFIG_PAUSE_CMD_START_US

	    _delay_us( HD44780_CONFIG_PAUSE_CMD_START_US );
	
    #endif

    WriteLow4bits( 0x03 );
    WriteStrobe( 40 );          // -> 0x03
    WriteStrobe( 40 );          // -> 0x03

    WriteLow4bits( 0x02 );
    WriteStrobe( 40 );          // -> 0x02
    WriteStrobe( 40 );          // -> 0x02

    WriteLow4bits( 0x2C );
    WriteStrobe( 40 );          // -> 0x0C

    WriteData( TYPE_COMMAND, 0x06 ); // 1, I/D, SH

    Clear(); // -> 0x01

    WriteData( TYPE_COMMAND, 0x0C ); // 1, D, C, B

    Home(); // -> 0x02
    
    // set data address to 0
    Goto( 0, 0 );

    // ��������� ������ ���������������� ��������
    // ������ ��������: ������������� �������
    // ������ ��������: ����� �������� � �������
    // ������ ��������: � ����� ���������������� ������ ���������� ������
    LoadCustomChar( LcdCustomChar, LCDCHAR_PROGRESS05, 0 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_PROGRESS15, 1 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_PROGRESS25, 2 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_PROGRESS35, 3 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_PROGRESS45, 4 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_PROGRESS55, 5 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_REWINDARROW, 6 );
    LoadCustomChar( LcdCustomChar, LCDCHAR_STOPBLOCK, 7 );

}

/**
 * ���������� ���������� ��������
 */
void CLCD::SetCols(uint8_t Cols){

    CLCD::Cols = Cols;

}


/**
 * ���������� ���������� �����
 */
void CLCD::SetRows(uint8_t Rows){

    CLCD::Rows = Rows;

}


/**
 * ���������� ������������� ��������
 */
void CLCD::SetLCDDriver(EnLCDDriverType DriverType){

    Driver = DriverType;

}


/**
 * ����� ������ �� �������
 */
void CLCD::WriteData(EnLCDDataType Type, uint8_t Data){

    // ������� �������� ����� ������ �� ����

    sbi( LCD_DDR_NAME_RS, LCD_PIN_RS );

    switch ( Type ) {

        case TYPE_COMMAND: {

            // ������������� ������ ������� ����� LCD_RS
            cbi( LCD_PORT_NAME_RS, LCD_PIN_RS );
            break;

        }

        case TYPE_DATA: {

            // ������������� ������� ������� ����� LCD_RS
            sbi( LCD_PORT_NAME_RS, LCD_PIN_RS );
            break;

        }

    }

    #ifndef LCD_RW_UNUSED

        // ������������� ������ ������� ����� LCD_RW
        sbi( LCD_DDR_NAME_RW, LCD_PIN_RW );
        cbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

    #endif

#ifdef LCD_DATA_LENGTH_4BIT

    // ������� ������� 4 ���� ������
    WriteLow4bits( Data >> 4 );

    // ��������� ����� ������ �� ����� LCD_E
    switch ( Type ) {

        case TYPE_COMMAND: {

            WriteStrobe( LCD_DELAYS_COMMAND_US );
            break;

        }

        case TYPE_DATA: {

            WriteStrobe( LCD_DELAYS_DATA_US );
            break;

        }

    }

    // ������� ������� 4 ���� ������
    WriteLow4bits( Data );

    // ��������� ����� ������ �� ����� LCD_E
    switch ( Type ) {

        case TYPE_COMMAND: {

            WriteStrobe( LCD_DELAYS_COMMAND_US );
            break;

        }

        case TYPE_DATA: {

            WriteStrobe( LCD_DELAYS_DATA_US );
            break;

        }

    }

    // ��������� ������ ������� �� ���� ������ ��� ������ �� ������
    WriteLow4bits(0);

#elif defined LCD_DATA_LENGTH_8BIT

    LCD_PORT_D0D7 = Data;

    // ��������� ����� ������ �� ����� LCD_E
    switch ( Type ) {

        case TYPE_COMMAND: {

            WriteStrobe( LCD_DELAYS_COMMAND_US );
            break;

        }

        case TYPE_DATA: {

            WriteStrobe( LCD_DELAYS_DATA_US );
            break;

        }

    }

    // ��������� ������ ������� �� ���� ������ ��� ������ �� ������
    LCD_PORT_D0D7 = 0x00;

#endif

    if ( Type == TYPE_DATA ) cbi( LCD_PORT_NAME_RS, LCD_PIN_RS );

}


/**
 * ��������� ������� ������
 */
uint8_t CLCD::ReadAddress(void){
         
    // �������, ���� ������� ����� ���������� ���������� �������
    while ( IsBusy() );
         
    #ifdef LCD_DATA_LENGTH_4BIT
             
        // ������������� ������ ������� ����� LCD_RS (�������)
        sbi( LCD_DDR_NAME_RS, LCD_PIN_RS );
        cbi( LCD_PORT_NAME_RS, LCD_PIN_RS );

        // ������������� ������� ������� ����� LCD_RW (������)
        sbi( LCD_DDR_NAME_RW, LCD_PIN_RW );
        sbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

        // ��������� ����� �� ����� LCD E
        WriteStrobe(0);
        
        // ��������� ������� �������
        uint8_t data = ( ReadLow4bits() << 4 ) & 0x7F;
        
        // ��������� ����� �� ����� LCD E
        WriteStrobe(0);        

        // ��������� � ��� ������� ��� ����� ��������� (BF)
        data |= ReadLow4bits();
             
     #endif
   
    cbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

    return data;
    
}


/**
 * read the control byte from the display controller
 */
uint8_t CLCD::ReadData(void){
         
    // �������, ���� ������� ����� ���������� ���������� �������
    while ( IsBusy() );
         
    #ifdef LCD_DATA_LENGTH_4BIT
             
        // ������������� ������� ������� ����� LCD_RS (������)
        sbi( LCD_DDR_NAME_RS, LCD_PIN_RS );
        sbi( LCD_PORT_NAME_RS, LCD_PIN_RS );

        // ������������� ������� ������� ����� LCD_RW (������)
        sbi( LCD_DDR_NAME_RW, LCD_PIN_RW );
        sbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

        // ��������� ����� �� ����� LCD E
        WriteStrobe(0);

        // ��������� ������� �������
        uint8_t data = ReadLow4bits() << 4;

        // ��������� ����� �� ����� LCD E
        WriteStrobe(0);
        
        // ��������� � ��� �������
        data |= ReadLow4bits();
             
     #endif
   
    cbi( LCD_PORT_NAME_RS, LCD_PIN_RS );
    
    cbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

    return data;
    
}


/**
 * ����������� ������ �������
 */
void CLCD::Home(){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    WriteData( TYPE_COMMAND, LCD_HD44780_CMD_RETURN_HOME );

    _delay_ms( LCD_DELAYS_CLEAR_MS );

}


/**
 * ����������� ������ �� �����������
 */
void CLCD::Goto(uint8_t Row, uint8_t Col){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    uint8_t DDRAMAddr;

    // remap lines into proper order
    switch ( Row ) {

        case 0: {
            DDRAMAddr = LCD_LINE0_DDRAMADDR + Col;
            break;
        }

        case 1: {
            DDRAMAddr = LCD_LINE1_DDRAMADDR + Col;
            break;
        }

        case 2: {
            DDRAMAddr = LCD_LINE2_DDRAMADDR + Col;
            break;
        }

        case 3: {
            DDRAMAddr = LCD_LINE3_DDRAMADDR + Col;
            break;
        }

        default:
            DDRAMAddr = LCD_LINE0_DDRAMADDR + Col;

    }

    // set data address
    WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_DDRAM ) | DDRAMAddr );

}


/**
 * �������� ������
 */
void CLCD::HideCursor( bool Blinking ) {

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    uint8_t Value = ( 1 << LCD_HD44780_CMD_ON_CTRL )
                    | ( 1 << LCD_HD44780_CMD_ON_DISPLAY );
    
    Value |= Blinking ? ( 1 << LCD_HD44780_CMD_ON_BLINK ) : 0;
    
    WriteData( TYPE_COMMAND, Value );

}


/**
 * ���������� ������
 */
void CLCD::ShowCursor( bool Blinking ) {

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    uint8_t Value = ( 1 << LCD_HD44780_CMD_ON_CTRL )
                    | ( 1 << LCD_HD44780_CMD_ON_DISPLAY )
                    | ( 1 << LCD_HD44780_CMD_ON_CURSOR );
    
    Value |= Blinking ? ( 1 << LCD_HD44780_CMD_ON_BLINK ) : 0;
    
    WriteData( TYPE_COMMAND, Value );

}

/**
 * �������� ������ �� �����������
 */
void CLCD::ShowCursor( bool Blinking, uint8_t Row, uint8_t Col ) {

    // ���������� ������ �� �������
    Goto( Row, Col );

    // ��������� ������������
    ShowCursor( Blinking );

}


/**
 * ����������� ������ �����
 */
void CLCD::MoveCursorLeft( uint8_t Count ) {

    // TODO: �������� ��� ��� ���������� ��������

}


/**
 * ����������� ������ ������
 */
void CLCD::MoveCursorRight( uint8_t Count ) {

    // TODO: �������� ��� ��� ���������� ��������

}


/**
 * ��������� ���������������� ������
 */
void CLCD::LoadCustomChar(FU08T_PTR pCustomCharArray, uint8_t romCharNum, uint8_t lcdCharNum){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    // backup the current cursor position
    uint8_t saveDDRAMAddr = ReadAddress();

    // multiply the character index by 8
    lcdCharNum = ( lcdCharNum << 3 );   // each character occupies 8 bytes
    romCharNum = ( romCharNum << 3 );   // each character occupies 8 bytes

    // copy the 8 bytes into CG (character generator) RAM
    for ( uint8_t i = 0; i < 8; i++ ) {

        // set CG RAM address
        WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_CGRAM ) | ( lcdCharNum + i ) );
        
        WriteData( TYPE_DATA, pCustomCharArray[ romCharNum + i ] );
                
    }

    // restore the previous cursor position
    WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_DDRAM ) | saveDDRAMAddr );

}


/**
 * ��������� ���������������� ������
 */
void CLCD::LoadCustomChar(const uint8_t * pCustomCharArray, uint8_t romCharNum, uint8_t lcdCharNum){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    // ��������� ������� ������� �������
    uint8_t saveDDRAMAddr = ReadAddress();

    // multiply the character index by 8
    lcdCharNum = ( lcdCharNum << 3 );   // each character occupies 8 bytes
    romCharNum = ( romCharNum << 3 );   // each character occupies 8 bytes

    // copy the 8 bytes into CG (character generator) RAM
    for ( uint8_t i = 0; i < 8; i++ ) {

        // set CG RAM address
        WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_CGRAM ) | ( lcdCharNum + i ) );
        
        WriteData( TYPE_DATA, pCustomCharArray[ romCharNum + i ] );                
        
    }

    // restore the previous cursor position
    WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_DDRAM ) | saveDDRAMAddr );

}


/**
 * ���������� �������� �����
 */
void CLCD::Draw( uint8_t Buffer[], uint8_t Row, uint8_t Col ){
    
    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;
    
    // ��� 1. ��������� ����� � ���������������� �������
    
    //   Buffer[0]       Buffer[1]       Buffer[2]
    //BIN8(11111111), BIN8(11111111), BIN8(11110000), // 11111 11111 11111 11111 

    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001  _ _
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001 |   |
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001 
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001 
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001 
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001

    //  Buffer[24]      Buffer[25]      Buffer[26]
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001

    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001 |_ _|
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001
    //BIN8(10000000), BIN8(00000000), BIN8(00010000), // 10000 00000 00000 00001
    //BIN8(11111111), BIN8(11111111), BIN8(11110000), // 11111 11111 11111 11111
    
    uint8_t Data = 0, CharNum;
    
    // ��������� ������� ������� �������
    uint8_t saveDDRAMAddr = ReadAddress();
    
    for ( uint8_t j = 0; j < 8; j++ ) {
    
        // multiply the character index by 8
        // each character occupies 8 bytes
        CharNum = ( j << 3 );

        // copy the 8 bytes into CG (character generator) RAM
        for ( uint8_t i = 0; i < 8; i++ ) {

            // set CG RAM address
            WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_CGRAM ) | ( CharNum + i ) );
            
            switch (j) {
            
                case 0: {
                    Data = Buffer[ 3 * i ] >> 3;
                    break;
                }

                case 4: {
                    Data = Buffer[ 24 + 3 * i ] >> 3;
                    break;
                }
                                
                case 2: {
                    Data = ( Buffer[ 1 + 3 * i ] & BIN8(00111110) ) >> 1;
                    break;
                }
                
                case 6: {
                    Data = ( Buffer[ 25 + 3 * i ] & BIN8(00111110) ) >> 1;
                    break;
                }
                
                case 1: {
                    Data = ( Buffer[ 3 * i ] & BIN8(00000111) ) << 2;
                    Data += Buffer[ 3 * i + 1 ] >> 6;
                    
                    break;
                }

                case 5: {
                    Data = ( Buffer[ 24 + 3 * i ] & BIN8(00000111) ) << 2;
                    Data += Buffer[ 24 + 3 * i + 1 ] >> 6;

                    break;
                }

                case 3: {
                    Data = ( Buffer[ 3 * i + 1 ] & BIN8(00000001) ) << 4;
                    Data += Buffer[ 3 * i + 2 ] >> 4;

                    break;
                }
                
                case 7: {
                    Data = ( Buffer[ 24 + 3 * i + 1 ] & BIN8(00000001) ) << 4;
                    Data += Buffer[ 24 + 3 * i + 2 ] >> 4;

                    break;
                }

            }
            
            WriteData( TYPE_DATA, Data );                
            
        }
        
    }
    
    // ��� 2. ���������� �� �� ������
    for ( uint8_t i = 0; i < 4; i++ ) { 
        
        PutChar( i, Row, Col + i );
        PutChar( i + 4, Row + 1, Col + i );
    }    
    
    // ��������������� ������� �������
    WriteData( TYPE_COMMAND, ( 1 << LCD_HD44780_CMD_DDRAM ) | saveDDRAMAddr );
    
}


/**
 * ������� ������ �� flash �� ���������
 */
void CLCD::WriteString( FCHAR_PTR pString ){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    // ���� ��������� ����� ����, �� �������
    if ( pString == 0 ) return;

    // ����� ������
    while ( * pString != 0 ) PutChar( ( uint8_t ) * ( pString++ ) );

}


/**
 * ������� ������ �� flash �� ���������
 */
void CLCD::WriteString( FCHAR_PTR pString, uint8_t Row, uint8_t Col ){

    // ��������� � ������ �������
    Goto( Row, Col );

    WriteString( pString );

}


/**
 * ������� ������ �� ��� �� ���������
 */
void CLCD::WriteString( const char * pString ){

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    // ���� ��������� ����� ����, �� �������
    if ( pString == 0 ) return;

    // ����� ������
    while ( * pString != 0 ) PutChar( ( uint8_t ) * ( pString++ ) );

}

/**
 * ������� ������ �� ��� �� ���������
 */
void CLCD::WriteString( const char * pString, uint8_t Row, uint8_t Col ){

    // ��������� � ������ �������
    Goto( Row, Col );

    WriteString( pString );

}


/**
 * �������, ���� �� ����� ������� ���� ��������� � LCD
 */
bool CLCD::IsBusy(){

    // When the busy flag is 1, the controller LSI is in the internal operation mode, and the next
    // instruction will not be accepted. When RS=0 and R/W=1, the busy flag is output to DB7. The
    // next instruction must be written after ensuring that the busy flag is 0.

    // +----+----+-------------------------------------------------------------+
    // | RS |R/W |                 Operation                                   �
    // +----+----+-------------------------------------------------------------+
    // | 0  | 0  | IR write as an internal operation (display clear, etc.)     |
    // | 0  | 1  | Read busy flag (DB7) and address counter (DB0 to DB7)       |
    // | 1  | 0  | Write data to DDRAM or CGRAM (DR to DDRAM or CGRAM)         |
    // | 1  | 1  | Read data from DDRAM or CGRAM (DDRAM or CGRAM to DR)        |
    // +----+----+-------------------------------------------------------------+
    
    bool Busy = false;
    
    // ���� ������������� ��������� ���������, �� ���������� ��
    if CHECKBIT( SFIOR, PUD ) cbi( SFIOR, PUD );
    
    // ������������� ������ ������� ����� LCD_RS (�������)
    sbi( LCD_DDR_NAME_RS, LCD_PIN_RS );
    cbi( LCD_PORT_NAME_RS, LCD_PIN_RS );
    
    // ������������� ������� ������� ����� LCD_RW (������)
    sbi( LCD_DDR_NAME_RW, LCD_PIN_RW );
    sbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

    // ������ ���� ���������, ���������� ����� �� ���� ;)         
    cbi( LCD_DDR_NAME_D7, LCD_PIN_D7 );
    sbi( LCD_PORT_NAME_D7, LCD_PIN_D7 );
    
    // ������ ��������� ����� LCD DB7
    Busy = CHECKBIT( LCD_PIN_NAME_D7, LCD_PIN_D7 );
    
    cbi( LCD_PORT_NAME_RW, LCD_PIN_RW );

    return Busy;
    
}


/**
 * ���������� ������� ��������� �� ������� �����������
 */
void CLCD::ShowProgressBar( uint8_t Progress, uint8_t MaxProgress, uint8_t Length ) {

    // ��������, ���� ������� �� ������
    if ( Driver == LCD_DRIVER_NONE ) return;

    // TODO: �������� ��� ��� ����������� �������� ����

}


/**
 * ���������� ������� ��������� �� �����������
 */
void CLCD::ShowProgressBar( uint8_t Progress, uint8_t MaxProgress, uint8_t Length, uint8_t Row, uint8_t Col ) {

    // ���������� ������ �� �������
    Goto( Row, Col );

    // ���������� ������� ���������
    ShowProgressBar( Progress, MaxProgress, Length );

}

