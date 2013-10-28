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

$version 0,0,4
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


Declare Sub ShowPinStates
Declare Sub PutByte( ByVal Addr As Word, ByVal AValue As Byte )
Declare Function GetByte( ByVal Addr As Word ) As Byte


' -=[ ���������� � ��� ]=-

Dim Temp As Byte
Dim Ptr As Word
Dim CurrTime(7) As Byte
Dim Caption As String * 46


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    ' ������������� ���� ��������� ����������������.
    MCU_Initialization

    ' ��������� RTC.
    RTC_Initialization

    ' ��������� ��������������.
    CommandShell_Initialization

    'TODO: ��� �������� ����� > 255?
    'Console_Beep 255, 1

    'Console_SetColor clLightGray, clBlack
    'Console_ClearScreen cmAll

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

    CommandShell_Info

    Do

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

                Temp = Inkey()

            Loop Until Temp = &H1B

            CommandShell_Info


        elseif Caption = "mcu" then

            Console_CursorOff

            For Temp = 0 To 22

                Console_NewLine

                Caption = LookupStr( Temp, MCUViewData )
                Console_WriteString Caption, cp866

            Next

            Console_NewLine

            DDRA = &HFF
            PORTA = &HAA

            DDRB = &HFF
            PORTB = &H0F

            ShowPinStates

            Do

                Temp = Inkey()

            Loop Until Temp = &H1B

            CommandShell_Info

        end if

    Loop

End


Sub ShowPinStates

    Local I As Byte, Tmp As Byte, Top As Byte
    Local Ch As Byte

    ' ��������� ����� B.
    For I = 0 To 7

        Top = 4 + I
        Console_MoveTo 16, Top

        Tmp = PINB
        Shift Tmp, Right, I
        Tmp = Tmp and &H01

        if Tmp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

    ' ��������� ����� D.
    For I = 0 To 7

        if I = 7 then

            Console_MoveTo 35, 23

        else

            Top = 17 + I
            Console_MoveTo 16, Top
        end if

        Tmp = PIND
        Shift Tmp, Right, I
        Tmp = Tmp and &H01

        if Tmp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

    ' ��������� ����� A.
    For I = 0 To 7

        Top = 4 + I
        Console_MoveTo 35, Top

        Tmp = PINA
        Shift Tmp, Right, I
        Tmp = Tmp and &H01

        if Tmp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

    ' ��������� ����� C.
    For I = 0 To 7

        Top = 22 - I
        Console_MoveTo 35, Top

        Tmp = PINC
        Shift Tmp, Right, I
        Tmp = Tmp and &H01

        if Tmp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

End Sub


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

MCUViewData:
    Data "                  ATmega16 - PDIP             ", _
        "                 +--------------+             ", _
        "   (XCK/T0) PB0 -|  1        40 |- PA0 (ADC0) ", _
        "       (T1) PB1 -|  2        39 |- PA1 (ADC1) ", _
        "(INT2/AIN0) PB2 -|  3        38 |- PA2 (ADC2) ", _
        " (OC0/AIN1) PB3 -|  4        37 |- PA3 (ADC3) ", _
        "      (~SS) PB4 -|  5        36 |- PA4 (ADC4) ", _
        "     (MOSI) PB5 -|  6        35 |- PA5 (ADC5) ", _
        "     (MISO) PB6 -|  7        34 |- PA6 (ADC6) ", _
        "      (SCK) PB7 -|  8        33 |- PA7 (ADC7) ", _
        "         ~RESET -|  9        32 |- AREF       ", _
        "            VCC -| 10        31 |- GND        ", _
        "            GND -| 11        30 |- AVCC       ", _
        "          XTAL2 -| 12        29 |- PC7 (TOSC2)", _
        "          XTAL1 -| 13        28 |- PC6 (TOSC1)", _
        "      (RXD) PD0 -| 14        27 |- PC5 (TDI)  ", _
        "      (TXD) PD1 -| 15        26 |- PC4 (TDO)  ", _
        "     (INT0) PD2 -| 16        25 |- PC3 (TMS)  ", _
        "     (INT1) PD3 -| 17        24 |- PC2 (TCK)  ", _
        "     (OC1B) PD4 -| 18        23 |- PC1 (SDA)  ", _
        "     (OC1A) PD5 -| 19        22 |- PC0 (SCL)  ", _
        "     (ICP1) PD6 -| 20        21 |- PD7 (OC2)  ", _
        "                 +--------------+             "
