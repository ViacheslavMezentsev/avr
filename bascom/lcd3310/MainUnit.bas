'-------------------------------------------------------------------------------
' Nokia-3310 LCD library
' BASCOM-AVR
' Mrshilov 2013 (Сергей Шилов)
'-------------------------------------------------------------------------------

$regfile = "m8def.dat"
$crystal = 8000000
$hwstack = 32
$swstack = 16
$framesize = 24

' Скорость работы внутреннего USART
' 9600, 19200, 38400, 57600, 115200
'$baud = 57600

$lib "glcd-Nokia3310.lib"
Config Graphlcd = 128x64sed, A0 = Portd.5, Si = Portd.6, Sclk = Portd.7

Config Portd.1 = Output
Config Portd.2 = Output
Config Portd.3 = Output
Config Portd.4 = Output


Const Negative_lcd = 1
Const Mirror_y = 0
Const Mirror_x = 0


' -=[ Переменные в ОЗУ ]=-


' Точка входа в основную программу.
Declare Sub Main : Main : End


' /***********************
' *  Д Е К Л А Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/

Declare Sub LCD1Select
Declare Sub LCD2Select
Declare Sub LCD3Select
Declare Sub LCD4Select


Sub Main

    Waitms 100

    LCD1Select : Cls
    LCD2Select : Cls
    LCD3Select : Cls
    LCD4Select : Cls

    SetFont Font6x8

    LCD1Select
    LcdAt 3, 24, "Первый"
    LcdAt 4, 23, "дисплей"

    LCD2Select
    LcdAt 3, 24, "Второй"
    LcdAt 4, 23, "дисплей"

    LCD3Select
    ShowPic 0, 0, Picture

    LCD4Select
    SetFont Font12x16dig
    LcdAt 2, 10, "24:16"

End Sub


Sub LCD1Select : Portd.1 = 0 : Portd.2 = 1 : Portd.3 = 1 : Portd.4 = 1 : End Sub

Sub LCD2Select : Portd.1 = 1 : Portd.2 = 0 : Portd.3 = 1 : Portd.4 = 1 : End Sub

Sub LCD3Select : Portd.1 = 1 : Portd.2 = 1 : Portd.3 = 0 : Portd.4 = 1 : End Sub

Sub LCD4Select : Portd.1 = 1 : Portd.2 = 1 : Portd.3 = 1 : Portd.4 = 0 : End Sub


Picture:
$bgf "walker.bgf"

$include "font6x8.font"
$include "font12x16dig.font"
