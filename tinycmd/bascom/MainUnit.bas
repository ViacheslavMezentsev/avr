'*******************************************************************************
'* �����: �������� ��������
'* ����:  28.10.2013 �.
'* MCU:  ATtiny2313
'* �������� �������:  8000000  Hz
'* ����������: BASCOM-AVR 2.0.7.1
'*
'* �����������: �������� ������ ��������� - ������ � ANSI ����������
'*
'*******************************************************************************


$regfile = "attiny2313.dat"
$crystal = 8000000
$hwstack = 40
$swstack = 16
$framesize = 32
'$sim

' �������� ������ ����������� USART
'$baud = 9600
'$baud = 19200
$baud = 38400
'$baud = 57600
'$baud = 115200


' -=[ ���������� � ��� ]=-
Dim Caption As String * 16


' ����� ����� � �������� ���������.
Declare Sub Main : Main : End


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ***********************/

Declare Function GetByte( ByVal Addr As Word ) As Byte

' ���������� ������������ ����� �������.

$include "Console_Header.bas"
$include "CommandShell_Header.bas"


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/

' ���������� ������.
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

        ' ����� ����������� ��������� ������.
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