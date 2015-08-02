#pragma once

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
 * http://citforum.ru/operating_systems/freebsd/ansi.shtml
 * http://subscribe.ru/archive/comp.soft.prog.shellandawk/200301/27191228.html
 *
 */

// Максимальные размеры окна.
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


// Text attributes
enum EnAttributes {

    atOff               = 0,    // Reset all attributes
    atIntensityBold     = 1,
    atIntensityFaint    = 2,    // (not widely supported)
    atItalicOn          = 3,    // (not widely supported)
    atUnderlineSingle   = 4,    // (not widely supported)
    atBlinkSlow         = 5,
    atBlinkRapid        = 6,
    atImageNegative     = 7,
    atConceal           = 8,    // (not widely supported)

    atUnderlineDouble   = 21,   //
    atIntensityNormal   = 22,   // not bold and not faint
    atUnderlineNone     = 24,   //
    atBlinkOff          = 25,   //
    atImagePositive     = 27,   //
    atReveal            = 28    // conceal off

};


enum EnMoveDirection {

    // Cursor Up: Moves the cursor up by the specified number of lines without
    // changing columns. If the cursor is already on the top line, ANSI.SYS
    // ignores this sequence.
    mdUp = 0,

    // Cursor Down: Moves the cursor down by the specified number of lines
    // without changing columns. If the cursor is already on the bottom line,
    // ANSI.SYS ignores this sequence.
    mdDown,

    // Cursor Forward: Moves the cursor forward by the specified number of
    // columns without changing lines. If the cursor is already in the
    // rightmost column, ANSI.SYS ignores this sequence.
    mdForward,

    // Cursor Backward: Moves the cursor back by the specified number of
    // columns without changing lines. If the cursor is already in the leftmost
    // column, ANSI.SYS ignores this sequence.
    mdBackward

};


//  Цвета.
enum EnColor {

    // Тёмные.
    clBlack = 0,
    clRed,
    clGreen,
    clYellow,
    clBlue,
    clMagenta,
    clCyan,
    clWhite,

    // Светлые.
    clDarkGray,
    clLightRed,
    clLightGreen,
    clLightYellow,
    clLightBlue,
    clLightMagenta,
    clLightCyan,
    clLightGray

};


class CConsole {

private:


public:

    enum EnCodePage { cp866 = 0, cp1251 };

    enum EnClearMode {

        cmFromCursorToEnd = 0,
        cmFromBeginToCursor,
        cmAll

    };

public:

    static uint8_t GetChar();
    static void PutChar( uint8_t ch, EnCodePage CodePage = cp866 );
    static void WriteString( FCHAR_PTR Value, EnCodePage CodePage = cp866, uint8_t Length = 0 );
    static void WriteString( const char * Value, EnCodePage CodePage = cp866, uint8_t Length = 0 );
    static void Beep( uint16_t Frequency = 800U, uint8_t Duration = 1 );
    static void ClearScreen( EnClearMode Mode = cmAll );
    static void ClearLine( EnClearMode Mode = cmFromCursorToEnd );
    static void ClearForward( uint8_t Count );
    static void CursorOn();
    static void CursorOff();
    static void SaveCursor();
    static void RestoreCursor();
    static void SetForegroundColor( EnColor Color );
    static void SetBackgroundColor( EnColor Color );
    static void SetColor( EnColor ForegroundColor, EnColor BackgroundColor );
    static void SetTextAttributes( EnAttributes Attributes );
    static void MoveTo( uint8_t Left, uint8_t Top );
    static void Move( EnMoveDirection Direction, uint8_t Delta );
    static void DrawFrame( uint8_t Left, uint8_t Top, uint8_t Width, uint8_t Height,
        EnColor Color, EnColor bgColor, char * Caption = NULL );

};
