$nocompile

' /****************************
' *  К О Н Ф И Г У Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~~~~~
' ****************************/


Const Console_Cursor_Enabled = 1
Const Console_DrawFrame_Enabled = 1 ' +/- 752 байта
Const Console_Beep_Enabled = 0 ' +/- 284 байта

Const Version_Enabled = 0 ' +/- ... байта

' Конфигурация LCD дисплея

' Внимание:
' Не забудьте назначить выводы и задержи для управления ЖКИ,
' в коде ничего исправлять не нужно!

LCD_DDR_NAME_En     alias DDRB.0
LCD_PORT_NAME_En    alias PORTB.0

LCD_DDR_NAME_RS     alias DDRC.6
LCD_PORT_NAME_RS    alias PORTC.6

' Примечание. Если используется только режим записи и вывод RW ЖКИ притянут к земле,
' то можно здесь задать тот же пин порта, что и для вывода LCD Enable. Это не нарушит
' логику работы программы в этом случае.
LCD_DDR_NAME_RW     alias LCD_DDR_NAME_En
LCD_PORT_NAME_RW    alias LCD_PORT_NAME_En
'LCD_DDR_NAME_RW     alias DDRD.4
'LCD_PORT_NAME_RW    alias PORTD.4

LCD_DDR_NAME_D4     alias DDRC.1
LCD_PORT_NAME_D4    alias PORTC.1
LCD_PIN_NAME_D4     alias PINC.1

LCD_DDR_NAME_D5     alias DDRA.1
LCD_PORT_NAME_D5    alias PORTA.1
LCD_PIN_NAME_D5     alias PINA.1

LCD_DDR_NAME_D6     alias DDRA.6
LCD_PORT_NAME_D6    alias PORTA.6
LCD_PIN_NAME_D6     alias PINA.6

LCD_DDR_NAME_D7     alias DDRD.7
LCD_PORT_NAME_D7    alias PORTD.7
LCD_PIN_NAME_D7     alias PIND.7

' Интерфейс.
Const LCD_DATA_LENGTH_4BIT = 1
