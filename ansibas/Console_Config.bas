$nocompile


' Максимальные размеры окна.
Const MAX_X = 80
Const MAX_Y = 25


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


Const cmFromCursorToEnd = 0
Const cmFromBeginToCursor = 1
Const cmAll = 2


' Функции для работы с ANSI терминалом.

Declare Sub Console_PutChar( ByVal Ch As Byte )
Declare Sub Console_WriteString( Text As String )
Declare Sub Console_Beep( ByVal Frequency As Word, ByVal Duration As Byte  )
Declare Sub Console_ClearScreen( ByVal Value As Byte )
Declare Sub Console_ClearLine( ByVal Value As Byte )
Declare Sub Console_ClearForward( ByVal Count As Byte )
Declare Sub Console_CursorOn
Declare Sub Console_CursorOff
Declare Sub Console_SaveCursor
Declare Sub Console_RestoreCursor
Declare Sub Console_SetForegroundColor( ByVal Color As Byte )
Declare Sub Console_SetBackgroundColor( ByVal Color As Byte )
Declare Sub Console_SetTextAttributes( ByVal Attributes As Byte )
Declare Sub Console_SetColor( ByVal AForegroundColor As Byte, ByVal ABackgroundColor As Byte )
Declare Sub Console_MoveTo( ByVal ALeft As Byte, ByVal ATop As Byte )