'*******************************************************************************
'* Автор: Мезенцев Вячеслав
'* Дата:  28.10.2013 г.
'* MCU:  ATtiny2313
'* Тактовая частота:  8000000  Hz
'* Компилятор: BASCOM-AVR 2.0.7.1
'*
'* Комментарии: тестовая версия программы - работа с ANSI терминалом
'*
'*******************************************************************************


$regfile = "attiny2313.dat"
$crystal = 8000000
$hwstack = 40
$swstack = 16
$framesize = 32
'$sim

' Скорость работы внутреннего USART
'$baud = 9600
'$baud = 19200
$baud = 38400
'$baud = 57600
'$baud = 115200


' -=[ Переменные в ОЗУ ]=-
Dim Caption As String * 16


' Точка входа в основную программу.
Declare Sub Main : Main : End


' /***********************
' *  Д Е К Л А Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/

Declare Function GetByte( ByVal Addr As Word ) As Byte

' Подключаем заголовочные файлы модулей.

$include "Console_Header.bas"
$include "CommandShell_Header.bas"


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/

' Подключаем модули.
$include "Console.bas"
$include "CommandShell.bas"


Sub Main

    $ASM
        ldi r16, &B00000110
        out $03, r16
    $END ASM

    CommandShell_Info
    'Console_NewLine

    Enable Interrupts

    Do

        ' Вывод приглашения командной строки.
        CommandShell_Prompt

        Input Caption

    Loop

End Sub


Function GetByte( ByVal Addr As Word ) As Byte

$ASM

    LOADADR Addr, X
    LD R30, X+
    LD R31, X
    LOADADR GetByte, X
    LD R24, Z
    ST X, R24

$END ASM

End Function