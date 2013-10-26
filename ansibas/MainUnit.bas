'***********************************************************************************
'* Автор: Мезенцев Вячеслав
'* Дата:  26.10.2013 г.
'* CPU:  ATmega16
'* Тактовая частота:  8000000  Hz
'* Компилятор: BASCOM-AVR 2.0.7.1
'*
'* Комментарии: тестовая версия программы - работа с ANSI терминалом
'*
'***********************************************************************************


' /****************************
' *  К О Н Ф И Г У Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~~~~~
' ****************************/


$regfile = "m16def.dat"
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


$include "Delays_Config.bas"
$include "Console_Config.bas"
$include "MCU_Config.bas"


Declare Sub CommandShell_Prompt


' -=[ Переменные в ОЗУ ]=-

    Dim AText As String * 45


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    ' Инициализация всей периферии микроконтроллера
    MCU_Initialization

    ' Запуск основного потока программы
    'MCU_MainThreadProcedure

    'TODO: Как передать число > 255?
    'Console_Beep 255, 1

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll
    Console_MoveTo 1, 25

    AText = LookupStr( 0, CommandShellInfo )
    Console_WriteString AText
    Console_PutChar &H0D
    Console_PutChar &H0A

    Do

        CommandShell_Prompt

        Input AText

    Loop

End


Sub CommandShell_Prompt

    AText = LookupStr( 0, Prompt )

    ' Выводим приглашение на новой строке.
    Console_SetForegroundColor clLightGreen
    Console_WriteString AText

    Console_SetForegroundColor clLightGray
    Console_ClearLine cmFromCursorToEnd
    Console_CursorOn

End Sub


$include "Delays.bas"
$include "Console.bas"
$include "MCU.bas"


' /****************
' *  Д А Н Н Ы Е
' *  ~~~~~~~~~~~
' ****************/


Prompt:
    Data "[ATmega16]$ "

CommandShellInfo:
    Data "Командная оболочка, версия 0.1 (Bascom AVR)"

