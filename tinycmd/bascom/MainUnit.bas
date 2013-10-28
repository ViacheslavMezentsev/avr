'*******************************************************************************
'* Автор: Мезенцев Вячеслав
'* Дата:  28.10.2013 г.
'* CPU:  ATtiny2313
'* Тактовая частота:  8000000  Hz
'* Компилятор: BASCOM-AVR 2.0.7.1
'*
'* Комментарии: тестовая версия программы - работа с ANSI терминалом
'*
'*******************************************************************************


' /****************************
' *  К О Н Ф И Г У Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~~~~~
' ****************************/

$version 0,0,21
$regfile = "attiny2313.dat"
$crystal = 8000000
'$sim

' Скорость работы внутреннего USART
'$baud = 9600
'$baud = 19200
$baud = 38400
'$baud = 57600
'$baud = 115200


' /***********************
' *  Д Е К Л А Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/

' Подключаем заголовочные файлы модулей.

$include "Console_Header.bas"
$include "CommandShell_Header.bas"


Declare Function GetByte( ByVal Addr As Word ) As Byte

' -=[ Переменные в ОЗУ ]=-

Dim Caption As String * 16


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    CommandShell_Info

    Do

        ' Вывод приглашения командной строки.
        CommandShell_Prompt

        Input Caption

    Loop

End


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


$include "Console.bas"
$include "CommandShell.bas"


' /****************
' *  Д А Н Н Ы Е
' *  ~~~~~~~~~~~
' ****************/
