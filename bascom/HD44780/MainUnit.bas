'*******************************************************************************
'* Автор: Мезенцев Вячеслав
'* Дата:  31.10.2013 г.
'* CPU:  ATmega16
'* Тактовая частота:  8000000 Гц
'* Компилятор: BASCOM-AVR 2.0.7.5
'*
'* Комментарии: Работа с ЖКИ (драйвер HD44780).
'*
'*******************************************************************************

$regfile = "m16def.dat"
$crystal = 8000000
$hwstack = 40
$swstack = 32
$framesize = 64
'$sim

' Скорость работы внутреннего USART
' 9600, 19200, 38400, 57600, 115200
$baud = 57600


' -=[ Переменные в ОЗУ ]=-

Dim Key As Byte
Dim Text As String * 32


' Точка входа в основную программу.
Declare Sub Main : Main : End


' /***********************
' *  Д Е К Л А Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/


' Подключаем заголовочные файлы модулей.
$include "Config_Header.bas"
$include "../base/Delays_Header.bas"
$include "../base/Console_Header.bas"
$include "../base/LCD_Header.bas"


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/

' Подключаем модули.
$include "../base/Delays.bas"
$include "../base/Console.bas"
$include "../base/LCD.bas"


Sub Main

    Local Ptr As Word

    Console_CursorOff
    Console_SetBackgroundColor clBlack
    Console_ClearScreen cmAll
    
    Text = "Индикатор"
    Console_DrawFrame 12, 9, 22, 6, clWhite, clGreen, Text
    
    Console_MoveTo 13, 10
    Console_CursorOn
    
    ' Инициализация индикатора.
    LCD_Initialization LCD_DRIVER_HD44780

    Ptr = LoadLabel( TextPinboardDemo )
    LCD_WriteFlashString Ptr

    LCD_MoveTo 0, 2

    'Ptr = LoadLabel( RussiaForever )
    'LCD_WriteFlashString Ptr
    
    LCD_ShowCursor 0
    
    Do

        Do : Key = Inkey() : Loop Until Key > &H30 Or Key = 13
        
        LCD_PutChar Key

    Loop

End Sub


' /****************
' *  Д А Н Н Ы Е
' *  ~~~~~~~~~~~
' ****************/


TextPinboardDemo:
    Data "Pinboard II Демо{013}Bascom AVR"

' Вывод на индикатор будет таким:
'
' +----------------+
' |Pinboard II Демо|
' |Bascom AVR      |
' +----------------+


RussiaForever:
    Data "Россия навсегда!{013}Ура, друзья!"

' Вывод на индикатор будет таким:
'
' +----------------+
' |Россия навсегда!|
' |Ура, друзья!    |
' +----------------+