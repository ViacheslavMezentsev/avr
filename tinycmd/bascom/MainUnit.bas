'*******************************************************************************
'* �����: �������� ��������
'* ����:  28.10.2013 �.
'* CPU:  ATtiny2313
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

$version 0,0,21
$regfile = "attiny2313.dat"
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

' ���������� ������������ ����� �������.

$include "Console_Header.bas"
$include "CommandShell_Header.bas"


Declare Function GetByte( ByVal Addr As Word ) As Byte

' -=[ ���������� � ��� ]=-

Dim Caption As String * 16


' /***********************
' *  � � � � � � � � � �
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


    CommandShell_Info

    Do

        ' ����� ����������� ��������� ������.
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
' *  � � � � � �
' *  ~~~~~~~~~~~
' ****************/
