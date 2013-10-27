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

' ���������� ������������ ����� �������.
$include "Delays_Header.bas"
$include "Console_Header.bas"
$include "RTC_Header.bas"
$include "CommandShell_Header.bas"
$include "MemoryViewer_Header.bas"
$include "MCU_Header.bas"


Declare Sub PutByte( ByVal Addr As Word, ByVal AValue As Byte )
Declare Function GetByte( ByVal Addr As Word ) As Byte


'address of ds1307
Const Ds1307w = &HD0
Const Ds1307r = &HD1


' -=[ ���������� � ��� ]=-

Dim Temp As Byte
Dim Ptr As Word
Dim CurrTime(7) As Byte
Dim Caption As String * 45


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    ' ������������� ���� ��������� ����������������.
    MCU_Initialization

    ' ��������� TWI.
    I2cinit

    ' ��������� ��������������.
    CommandShell_Initialization

    'TODO: ��� �������� ����� > 255?
    'Console_Beep 255, 1

    'Console_SetColor clLightGray, clBlack
    'Console_ClearScreen cmAll

    ' ��������� ����.
    'Caption = LookupStr( 0, CommandShellInfo )
    'Console_MoveTo 1, 1
    'Console_SetColor clBlack, clWhite
    'Console_WriteString Caption, cp1251
    'Console_ClearLine cmFromCursorToEnd

    ' ����� ��� ���������.
    'Caption = ""
    'Console_DrawFrame 1, 2, 80, 23, clLightGray, clBlue, Caption

    ' ���� ���� � �������.
    'Console_DrawFrame 25, 10, 20, 4, clBlack, clWhite, Caption

    'Caption = LookupStr( 0, TimeStr )
    'Console_MoveTo 27, 11
    'Console_WriteString Caption, cp1251

    'Caption = LookupStr( 0, DateStr )
    'Console_MoveTo 27, 12
    'Console_WriteString Caption, cp1251

    'Ptr = &H90
    'Temp = &H39
    'PutByte Ptr, Temp

    'Temp = 0

    'Temp = GetByte( Ptr )
    'Console_MoveTo 1, 2
    'Console_PutChar Temp, cp866

    Do

        CommandShell_Info

        ' ����� ����������� ��������� ������.
        CommandShell_Prompt

        Input Caption

        RTC_GetDatetime CurrTime(1)

        if Caption = "time" then

            RTC_ShowTime CurrTime(1)

        elseif Caption = "date" then

            RTC_ShowDate CurrTime(1)

        elseif Caption = "ram" then

            MemoryViewer_FormActivate

            Do
            Loop Until Inkey() = &H1B

        end if

    Loop

End


Sub PutByte( ByVal Addr As Word, ByVal AValue As Byte )

$ASM

    LOADADR Addr, X
    LD R30, X+
    LD R31, X
    LOADADR AValue, X
    LD R24, X
    ST Z, R24

$END ASM

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


$include "Delays.bas"
$include "Console.bas"
$include "RTC.bas"
$include "CommandShell.bas"
$include "MemoryViewer.bas"
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

