'***********************************************************************************
'* �����: �������� ��������
'* ����:  26.10.2013 �.
'* CPU:  ATmega16
'* �������� �������:  8000000  Hz
'* ����������: BASCOM-AVR 2.0.7.1
'*
'* �����������: �������� ������ ��������� - ������ � ANSI ����������
'*
'***********************************************************************************


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
$baud = 38400
'$baud = 57600
'$baud = 115200


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/


$include "Delays_Config.bas"
$include "Console_Config.bas"
$include "MCU_Config.bas"


Declare Sub CommandShell_Prompt


' -=[ ���������� � ��� ]=-

    Dim AText As String * 45


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    ' ������������� ���� ��������� ����������������
    MCU_Initialization

    ' ������ ��������� ������ ���������
    'MCU_MainThreadProcedure

    'TODO: ��� �������� ����� > 255?
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

    ' ������� ����������� �� ����� ������.
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
' *  � � � � � �
' *  ~~~~~~~~~~~
' ****************/


Prompt:
    Data "[ATmega16]$ "

CommandShellInfo:
    Data "��������� ��������, ������ 0.1 (Bascom AVR)"

