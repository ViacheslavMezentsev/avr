$nocompile


' Конфигурация LCD дисплея

' Внимание:
' Не забудьте назначить выводы и задержи для управления ЖКИ,
' в коде ничего исправлять не нужно!

'LCD_DDR_NAME_En     alias DDRB.0
'LCD_PORT_NAME_En    alias PORTB.0

'LCD_DDR_NAME_RS     alias DDRC.6
'LCD_PORT_NAME_RS    alias PORTC.6

' Примечание. Если используется только режим записи и вывод RW ЖКИ притянут к земле,
' то можно здесь задать тот же пин порта, что и для вывода LCD Enable. Это не нарушит
' логику работы программы в этом случае.
'LCD_DDR_NAME_RW     alias LCD_DDR_NAME_En
'LCD_PORT_NAME_RW    alias LCD_PORT_NAME_En
'LCD_DDR_NAME_RW     alias DDRD.4
'LCD_PORT_NAME_RW    alias PORTD.4

'LCD_DDR_NAME_D4     alias DDRC.1
'LCD_PORT_NAME_D4    alias PORTC.1
'LCD_PIN_NAME_D4     alias PINC.1

'LCD_DDR_NAME_D5     alias DDRA.1
'LCD_PORT_NAME_D5    alias PORTA.1
'LCD_PIN_NAME_D5     alias PINA.1

'LCD_DDR_NAME_D6     alias DDRA.6
'LCD_PORT_NAME_D6    alias PORTA.6
'LCD_PIN_NAME_D6     alias PINA.6

'LCD_DDR_NAME_D7     alias DDRD.7
'LCD_PORT_NAME_D7    alias PORTD.7
'LCD_PIN_NAME_D7     alias PIND.7

' Интерфейс.
'Const LCD_DATA_LENGTH_4BIT = 1

Const TYPE_COMMAND = 0
Const TYPE_DATA = 1

Const LCD_DRIVER_NONE = 0
Const LCD_DRIVER_HD44780 = 1

' Внимание! Величины задержек нужно подстраивать вручную.
' TODO: Придумать как приручить функции Waitus и Waitms
Const LCD_DELAYS_COMMAND_US = 5
Const LCD_DELAYS_DATA_US = 5
Const LCD_DELAYS_CLEAR_MS = 2

Const LCD_HD44780_CMD_CLEAR         = &H01
Const LCD_HD44780_CMD_RETURN_HOME   = &H02

Const LCD_HD44780_CMD_ON_CTRL       = 3 ' DB3: turn lcd/cursor on
Const LCD_HD44780_CMD_ON_DISPLAY    = 2 ' DB2: turn display on
Const LCD_HD44780_CMD_ON_CURSOR     = 1 ' DB1: turn cursor on
Const LCD_HD44780_CMD_ON_BLINK      = 0 ' DB0: blinking cursor

' Адреса строк в DDRAM
Const LCD_LINE0_DDRAMADDR = &H00
Const LCD_LINE1_DDRAMADDR = &H40
Const LCD_LINE2_DDRAMADDR = &H14
Const LCD_LINE3_DDRAMADDR = &H54


Dim LDriver As Byte


' Функции для работы с LCD дисплеем

Declare Sub LCD_Initialization( ByVal ADriver As Byte )
Declare Sub LCD_Init_HD44780
Declare Sub LCD_Clear
Declare Sub LCD_Home
Declare Sub LCD_MoveTo( ByVal ALeft As Byte, ByVal ATop As Byte )
Declare Sub LCD_PutChar( ByVal AChar As Byte )
Declare Sub LCD_WriteString( AText As String )
Declare Sub LCD_WriteFlashString( ByVal APtr As Word )
Declare Sub LCD_HideCursor( ByVal ABlinking As Byte )
Declare Sub LCD_ShowCursor( ByVal ABlinking As Byte )
Declare Sub LCD_ShowCursorAt( ByVal ABlinking As Byte, ByVal ALeft As Byte, ByVal ATop As Byte )
Declare Function LCD_ReadAddress() As Byte
Declare Function LCD_IsBusy() As Byte
Declare Sub LCD_WriteStrobe( ByVal usec As DWORD )
Declare Sub LCD_WriteLow4bits( ByVal AValue As Byte )
Declare Function LCD_ReadLow4bits() As Byte
Declare Sub LCD_WriteData( ByVal ACmdType As Byte, ByVal AValue As Byte )
