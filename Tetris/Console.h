#ifndef _CONSOLE_H_
#define _CONSOLE_H_

/**
 * Класс CConsole
 * ~~~~~~~~~~~~~~
 * 
 * Назначение:
 * Обёртка для работы с потоком ввода/вывода.
 * 
 * Автор: Мезенцев Вячеслав
 * 
 * Почта: mailto:unihomelab@ya.ru
 * 
 */

#define BLINK 128

//defines max coordinates for checking overflow
#define MAX_X 80
#define MAX_Y 25

// Compatible with Unix Curses
#define ACS_ULCORNER	(0xDA)	/* upper left corner */
#define ACS_LLCORNER	(0xC0)	/* lower left corner */
#define ACS_URCORNER	(0xBF)	/* upper right corner */
#define ACS_LRCORNER	(0xD9)	/* lower right corner */
#define ACS_HLINE		(0xC4)	/* horizontal line */
#define ACS_VLINE		(0xB3)	/* vertical line */

#define ACS_DBL_ULCORNER	(0xC9)	/* upper left corner */
#define ACS_DBL_LLCORNER	(0xC8)	/* lower left corner */
#define ACS_DBL_URCORNER	(0xBB)	/* upper right corner */
#define ACS_DBL_LRCORNER	(0xBC)	/* lower right corner */
#define ACS_DBL_HLINE		(0xCD)	/* horizontal line */
#define ACS_DBL_VLINE		(0xBA)	/* vertical line */

#define ACS_LTEE	(acs_map['t'])	/* tee pointing right */
#define ACS_RTEE	(acs_map['u'])	/* tee pointing left */
#define ACS_BTEE	(acs_map['v'])	/* tee pointing up */
#define ACS_TTEE	(acs_map['w'])	/* tee pointing down */
#define ACS_PLUS	(acs_map['n'])	/* large plus or crossover */
#define ACS_S1		(acs_map['o'])	/* scan line 1 */
#define ACS_S9		(acs_map['s'])	/* scan line 9 */
#define ACS_DIAMOND	(acs_map['`'])	/* diamond */
#define ACS_CKBOARD	(acs_map['a'])	/* checker board (stipple) */
#define ACS_DEGREE	(acs_map['f'])	/* degree symbol */
#define ACS_PLMINUS	(acs_map['g'])	/* plus/minus */
#define ACS_BULLET	(acs_map['~'])	/* bullet */
/* Teletype 5410v1 symbols begin here */
#define ACS_LARROW	(acs_map[','])	/* arrow pointing left */
#define ACS_RARROW	(acs_map['+'])	/* arrow pointing right */
#define ACS_DARROW	(acs_map['.'])	/* arrow pointing down */
#define ACS_UARROW	(acs_map['-'])	/* arrow pointing up */
#define ACS_BOARD	(acs_map['h'])	/* board of squares */
#define ACS_LANTERN	(acs_map['i'])	/* lantern symbol */
#define ACS_BLOCK	(acs_map['0'])	/* solid square block */

//  Цвета
/* dark colors */
#define BLACK           0          
#define RED             1 
#define GREEN           2
#define BROWN           3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define LIGHTGRAY       7
/* light colors */
#define DARKGRAY        8       
#define LIGHTRED        9
#define LIGHTGREEN      10
#define YELLOW          11
#define LIGHTBLUE       12
#define LIGHTMAGENTA    13
#define LIGHTCYAN       14
#define WHITE           15

class CConsole {

public:

    enum EnCodePage { cp866 = 0, cp1251 };

    static uint8_t GetChar();
    static void PutChar( uint8_t ch, EnCodePage CodePage = cp866 );
    static char * ReadString( char * s );
    static void WriteString( FCHAR_PTR Value, EnCodePage CodePage = cp866, uint8_t Length = 0 );
    static void WriteString( const char * Value, EnCodePage CodePage = cp866, uint8_t Length = 0 );
    static void ClearScreen();
    static void ClearEndOfLine();
    static void CursorOn();
    static void CursorOff();
    static void SetTextColor( uint8_t color );
    static void SetTextBackground( uint8_t color );
    static void SetTextAttr( uint8_t attr );
    static void MoveTo( uint8_t Left, uint8_t Top );

};

#endif // _CONSOLE_H_