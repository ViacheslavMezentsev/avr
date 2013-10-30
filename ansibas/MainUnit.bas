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

' -=[ Переменные в ОЗУ ]=-

Dim Key As Byte
Dim Ptr As Word
Dim Caption As String * 46


' Точка входа в основную программу.
Declare Sub Main : Main : End


' /***********************
' *  Д Е К Л А Р А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/


Declare Sub ShowPinStates
Declare Sub PutByte( ByVal Addr As Word, ByVal AValue As Byte )
Declare Function GetByte( ByVal Addr As Word ) As Byte


' Подключаем заголовочные файлы модулей.
$include "Config_Header.bas"
$include "Version_Header.bas"
$include "Delays_Header.bas"
$include "RTC_Header.bas"
$include "Console_Header.bas"
$include "CommandShell_Header.bas"
$include "MemoryViewer_Header.bas"
$include "MCU_Header.bas"

' Подключаем модули.
$include "Delays.bas"
$include "RTC.bas"
$include "CommandShell.bas"
$include "MemoryViewer.bas"
$include "Console.bas"
$include "MCU.bas"


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/

Sub Main

    ' Инициализация всей периферии микроконтроллера.
    MCU_Initialization

    ' Настройка RTC.
    RTC_Initialization RTC_DS1307

    ' Настройка интерпретатора.
    CommandShell_Initialization

    'TODO: Как передать число > 255?
    'Console_Beep 255, 1

    'Console_SetColor clLightGray, clBlack
    'Console_ClearScreen cmAll

    ' Рамка без заголовка.
    'Caption = ""
    'Console_DrawFrame 1, 2, 80, 23, clLightGray, clBlue, Caption

    ' Окно даты и времени.
    'Console_DrawFrame 25, 10, 20, 4, clBlack, clWhite, Caption

    'Caption = LookupStr( 0, TimeStr )
    'Console_MoveTo 27, 11
    'Console_WriteString Caption, cp1251

    'Caption = LookupStr( 0, DateStr )
    'Console_MoveTo 27, 12
    'Console_WriteString Caption, cp1251

    CommandShell_Info

    Do

        ' Вывод приглашения командной строки.
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

                Key = Inkey()

            Loop Until Key = &H1B

            CommandShell_Info


        elseif Caption = "mcu" then

            DDRA = &HFF
            PORTA = &HAA

            DDRB = &HFF
            PORTB = &H0F

            Console_CursorOff
            Console_ClearScreen cmAll
            ShowPinStates

            Do

                Key = Inkey()

            Loop Until Key = &H1B

            CommandShell_Info

        end if

    Loop

End Sub


Sub ShowPinStates

    Local I As Byte, Top As Byte, Temp As Byte

    ' Выводим изображение микроконтроллера.
    For Temp = 0 To 22

        Top = Temp + 2
        Console_MoveTo 1, Top

        Caption = LookupStr( Temp, MCUViewData )
        Console_WriteString Caption, cp866

    Next

    ' Состояние порта B.
    For I = 0 To 7

        Top = 4 + I
        Console_MoveTo 16, Top

        Temp = PINB
        Shift Temp, Right, I
        Temp = Temp and &H01

        if Temp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

    ' Состояние порта D.
    For I = 0 To 7

        if I = 7 then

            Console_MoveTo 35, 23

        else

            Top = 17 + I
            Console_MoveTo 16, Top
        end if

        Temp = PIND
        Shift Temp, Right, I
        Temp = Temp and &H01

        if Temp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

    ' Состояние порта A.
    For I = 0 To 7

        Top = 4 + I
        Console_MoveTo 35, Top

        Temp = PINA
        Shift Temp, Right, I
        Temp = Temp and &H01

        if Temp = 0 then

            Console_SetForegroundColor clBlue

        else

            Console_SetForegroundColor clRed
        end if

        PrintBin &HFE

    Next

    ' Состояние порта C.
    For I = 0 To 7

        Top = 22 - I
        Console_MoveTo 35, Top

        Temp = PINC
        Shift Temp, Right, I
        Temp = Temp and &H01

        if Temp = 0 then

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


' /****************
' *  Д А Н Н Ы Е
' *  ~~~~~~~~~~~
' ****************/

CommandShellInfo:
    Data "Командная оболочка (Bascom AVR), версия "

Prompt:
    Data "[ATmega16]$ "

ViewMemoryData:
    Data "Просмотр памяти: ОЗУ (CP1251)"

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