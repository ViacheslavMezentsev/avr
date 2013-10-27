'*******************************************************************************
'* Автор: Мезенцев Вячеслав
'* Дата:  26.10.2013 г.
'* CPU:  ATmega16
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


$regfile = "m16def.dat"
$crystal = 8000000
'$sim

' Скорость работы внутреннего USART
'$baud = 9600
'$baud = 19200
'$baud = 38400
$baud = 57600
'$baud = 115200

$lib "i2c_twi.lbx"

Config Scl = PortC.0
Config Sda = PortC.1
Config Twi = 100000


' /***********************
' *  Д Е К Л А Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/


$include "Delays_Config.bas"
$include "Console_Config.bas"
$include "MCU_Config.bas"

Declare Sub RTC_GetDatetime( ATime As Byte )
Declare Sub CommandShell_ShowDate( ATime As Byte, ByVal ALeft As Byte, _
    ByVal ATop As Byte )
Declare Sub CommandShell_ShowTime( ATime As Byte, ByVal ALeft As Byte, _
    ByVal ATop As Byte )
Declare Sub CommandShell_Prompt


'address of ds1307
Const Ds1307w = &HD0
Const Ds1307r = &HD1


' -=[ Переменные в ОЗУ ]=-

Dim CurrTime(7) As Byte
Dim Caption As String * 45


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    ' Инициализация всей периферии микроконтроллера
    MCU_Initialization

    I2cinit

    ' Запуск основного потока программы
    'MCU_MainThreadProcedure

    'TODO: Как передать число > 255?
    'Console_Beep 255, 1

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll

    ' Заголовок окна.
    Caption = LookupStr( 0, CommandShellInfo )
    Console_MoveTo 1, 1
    Console_SetColor clBlack, clWhite
    Console_WriteString Caption, cp1251
    Console_ClearLine cmFromCursorToEnd

    ' Рамка без заголовка.
    Caption = ""
    Console_DrawFrame 1, 2, 80, 23, clLightGray, clBlue, Caption

    ' Окно даты и времени.
    Console_DrawFrame 25, 10, 20, 4, clBlack, clWhite, Caption

    Caption = LookupStr( 0, TimeStr )
    Console_MoveTo 27, 11
    Console_WriteString Caption, cp1251

    Caption = LookupStr( 0, DateStr )
    Console_MoveTo 27, 12
    Console_WriteString Caption, cp1251

    Do

        ' Вывод приглашения командной строки.
        CommandShell_Prompt

        _delay_ms 1000

        RTC_GetDatetime CurrTime(1)

        Console_CursorOff
        Console_SetColor clBlack, clWhite
        CommandShell_ShowTime CurrTime(1), 33, 11
        CommandShell_ShowDate CurrTime(1), 33, 12

    Loop

End


Sub RTC_GetDatetime( ATime As Byte )

  I2cstart
  I2cwbyte Ds1307w
  I2cwbyte 0

  I2cstart
  I2cwbyte Ds1307r
  I2crbyte ATime(1), Ack
  I2crbyte ATime(2), Ack
  I2crbyte ATime(3), Ack
  I2crbyte ATime(4), Ack
  I2crbyte ATime(5), Ack
  I2crbyte ATime(6), Ack
  I2crbyte ATime(7), Nack
  I2cstop

End Sub


Sub CommandShell_ShowDate( ATime As Byte, ByVal ALeft As Byte, _
    ByVal ATop As Byte )

    Local Ch As Byte
    Local AYear As Byte, AMonth As Byte, ADay As Byte

    Console_MoveTo ALeft, ATop

    ADay = ATime(5)
    ADay = MakeDec(ADay)

    AMonth = ATime(6)
    AMonth = MakeDec(AMonth)

    AYear = ATime(7)
    AYear = MakeDec(AYear)

    ' Вывод дней.
    ' Десятки.
    Ch = ADay \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ADay mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' Вывод месяца.
    ' Десятки.
    Ch = AMonth \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AMonth mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' Вывод года.
    ' Десятки.
    Ch = AYear \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AYear mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

End Sub


Sub CommandShell_ShowTime( ATime As Byte, ByVal ALeft As Byte, _
    ByVal ATop As Byte )

    Local Ch As Byte
    Local AHour As Byte, AMinutes As Byte, ASeconds As Byte

    Console_MoveTo ALeft, ATop

    ASeconds = ATime(1)
    ASeconds = MakeDec(ASeconds)

    AMinutes = ATime(2)
    AMinutes = MakeDec(AMinutes)

    AHour = ATime(3)
    AHour = MakeDec(AHour)

    ' Вывод часов.
    ' Десятки.
    Ch = AHour \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AHour mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' Вывод минут.
    ' Десятки.
    Ch = AMinutes \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AMinutes mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' Вывод секунд.
    ' Десятки.
    Ch = ASeconds \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ASeconds mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

End Sub


Sub CommandShell_Prompt

    Caption = LookupStr( 0, Prompt )

    Console_SetColor clLightGreen, clBlack
    Console_MoveTo 1, 25
    Console_WriteString Caption, cp1251

    Console_SetForegroundColor clLightGray
    Console_SaveCursor
    Console_ClearLine cmFromCursorToEnd
    Console_RestoreCursor

    Console_CursorOn

End Sub


$include "Delays.bas"
$include "Console.bas"
$include "MCU.bas"


' /****************
' *  Д А Н Н Ы Е
' *  ~~~~~~~~~~~
' ****************/
TimeStr:
    Data "Time: "

DateStr:
    Data "Date: "

TitleDS:
    Data "DS1307"

Prompt:
    Data "[ATmega16]$ "

CommandShellInfo:
    Data "Командная оболочка, версия 0.1 (Bascom AVR)"