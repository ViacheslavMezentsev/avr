'*******************************************************************************
'* �����: �������� ��������
'* ����:  26.10.2013 �.
'* CPU:  ATmega16
'* �������� �������:  8000000  Hz
'* ����������: BASCOM-AVR 2.0.7.1
'*
'* �����������: �������� ������ ��������� - ������ � ANSI ����������
'*
'*******************************************************************************


' /****************************
' *  � � � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~~~~~
' ****************************/


$regfile = "m16def.dat"
$crystal = 8000000
'$sim

' �������� ������ ����������� USART
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
' *  � � � � � � � � � �
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


' -=[ ���������� � ��� ]=-

Dim CurrTime(7) As Byte
Dim Caption As String * 45


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    ' ������������� ���� ��������� ����������������
    MCU_Initialization

    I2cinit

    ' ������ ��������� ������ ���������
    'MCU_MainThreadProcedure

    'TODO: ��� �������� ����� > 255?
    'Console_Beep 255, 1

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll

    ' ��������� ����.
    Caption = LookupStr( 0, CommandShellInfo )
    Console_MoveTo 1, 1
    Console_SetColor clBlack, clWhite
    Console_WriteString Caption, cp1251
    Console_ClearLine cmFromCursorToEnd

    ' ����� ��� ���������.
    Caption = ""
    Console_DrawFrame 1, 2, 80, 23, clLightGray, clBlue, Caption

    ' ���� ���� � �������.
    Console_DrawFrame 25, 10, 20, 4, clBlack, clWhite, Caption

    Caption = LookupStr( 0, TimeStr )
    Console_MoveTo 27, 11
    Console_WriteString Caption, cp1251

    Caption = LookupStr( 0, DateStr )
    Console_MoveTo 27, 12
    Console_WriteString Caption, cp1251

    Do

        ' ����� ����������� ��������� ������.
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

    ' ����� ����.
    ' �������.
    Ch = ADay \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = ADay mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' ����� ������.
    ' �������.
    Ch = AMonth \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AMonth mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "-" )
    Console_PutChar Ch, cp866

    ' ����� ����.
    ' �������.
    Ch = AYear \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
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

    ' ����� �����.
    ' �������.
    Ch = AHour \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AHour mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' ����� �����.
    ' �������.
    Ch = AMinutes \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
    Ch = AMinutes mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ":" )
    Console_PutChar Ch, cp866

    ' ����� ������.
    ' �������.
    Ch = ASeconds \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' �������.
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
' *  � � � � � �
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
    Data "��������� ��������, ������ 0.1 (Bascom AVR)"