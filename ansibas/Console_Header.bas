$nocompile


' Максимальные размеры окна.
Const MAX_X = 80
Const MAX_Y = 25

Const cp866 = 0
Const cp1251 = 1

Const ACS_ULCORNER = &HDA '/* upper left corner */
Const ACS_LLCORNER = &HC0 '/* lower left corner */
Const ACS_URCORNER = &HBF '/* upper right corner */
Const ACS_LRCORNER = &HD9 '/* lower right corner */
Const ACS_HLINE   = &HC4 '/* horizontal line */
Const ACS_VLINE   = &HB3 '/* vertical line */

Const ACS_DBL_ULCORNER = &HC9 '/* upper left corner */
Const ACS_DBL_LLCORNER = &HC8 '/* lower left corner */
Const ACS_DBL_URCORNER = &HBB '/* upper right corner */
Const ACS_DBL_LRCORNER = &HBC '/* lower right corner */
Const ACS_DBL_HLINE  = &HCD '/* horizontal line */
Const ACS_DBL_VLINE  = &HBA '/* vertical line */

Const atOff               = 0    '// Reset all attributes
Const atIntensityBold     = 1
Const atIntensityFaint    = 2    '// (not widely supported)
Const atItalicOn          = 3    '// (not widely supported)
Const atUnderlineSingle   = 4    '// (not widely supported)
Const atBlinkSlow         = 5
Const atBlinkRapid        = 6
Const atImageNegative     = 7
Const atConceal           = 8    '// (not widely supported)

Const atUnderlineDouble   = 21   '//
Const atIntensityNormal   = 22   '// not bold and not faint
Const atUnderlineNone     = 24   '//
Const atBlinkOff          = 25   '//
Const atImagePositive     = 27   '//
Const atReveal            = 28   '// conceal off


' Тёмные.
Const clBlack = 0
Const clRed = 1
Const clGreen = 2
Const clYellow = 3
Const clBlue = 4
Const clMagenta = 5
Const clCyan = 6
Const clWhite = 7

' Светлые.
Const clDarkGray = 8
Const clLightRed = 9
Const clLightGreen = 10
Const clLightYellow = 11
Const clLightBlue = 12
Const clLightMagenta = 13
Const clLightCyan = 14
Const clLightGray = 15


' Cursor Up: Moves the cursor up by the specified number of lines without
' changing columns. If the cursor is already on the top line, ANSI.SYS
' ignores this sequence.
Const mdUp = 0

' Cursor Down: Moves the cursor down by the specified number of lines
' without changing columns. If the cursor is already on the bottom line,
' ANSI.SYS ignores this sequence.
Const mdDown = 1

' Cursor Forward: Moves the cursor forward by the specified number of
' columns without changing lines. If the cursor is already in the
' rightmost column, ANSI.SYS ignores this sequence.
Const mdForward = 2

' Cursor Backward: Moves the cursor back by the specified number of
' columns without changing lines. If the cursor is already in the leftmost
' column, ANSI.SYS ignores this sequence.
Const mdBackward = 3


Const cmFromCursorToEnd = 0
Const cmFromBeginToCursor = 1
Const cmAll = 2


Dim AData(11) As Byte
Dim NewLine As Word At AData Overlay
Dim Command As String * 10 At AData Overlay


' Функции для работы с ANSI терминалом.

Declare Sub Console_PutChar( ByVal AChar As Byte, ByVal ACodePage As Byte )
Declare Sub Console_WriteData
Declare Sub Console_WriteString( AText As String, ByVal ACodePage As Byte )
Declare Sub Console_NewLine
Declare Sub Console_Beep( ByVal AFrequency As Word, ByVal ADuration As Byte  )
Declare Sub Console_ClearScreen( ByVal AMode As Byte )
Declare Sub Console_ClearLine( ByVal AMode As Byte )
Declare Sub Console_ClearForward( ByVal ACount As Byte )
Declare Sub Console_CursorOn
Declare Sub Console_CursorOff
Declare Sub Console_SaveCursor
Declare Sub Console_RestoreCursor
Declare Sub Console_SetForegroundColor( ByVal AColor As Byte )
Declare Sub Console_SetBackgroundColor( ByVal AColor As Byte )
Declare Sub Console_SetTextAttributes( ByVal Attributes As Byte )
Declare Sub Console_SetColor( ByVal AForegroundColor As Byte, ByVal ABackgroundColor As Byte )
Declare Sub Console_MoveTo( ByVal ALeft As Byte, ByVal ATop As Byte )
Declare Sub Console_Move( ByVal ADirection As Byte, ByVal ADelta As Byte )
Declare Sub Console_DrawFrame( ByVal ALeft As Byte, ByVal ATop As Byte, _
    ByVal AWidth As Byte, ByVal AHeight As Byte, ByVal AColor As Byte, _
    ByVal ABgColor As Byte, ACaption As String )