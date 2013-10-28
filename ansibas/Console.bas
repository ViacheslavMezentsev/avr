$nocompile

'*******************************************************************************
'*
'* �����: �������� �������� ����������
'* ������������: viacheslavmezentsev@gmail.com
'* skype: viacheslavmezentsev
'* ����:  28.10.2013 �.
'*
'* ����������: BASCOM-AVR 2.0.7.1
'* ��������: ������ ��� ������ � ANSI ����������.
'*
'*******************************************************************************


'$include "Console_Header.bas"


' ����� ������� �� ��� � ��������� ACodePage.
Sub Console_PutChar( ByVal AChar As Byte, ByVal ACodePage As Byte )

    select case ACodePage

        case cp866:

        case cp1251:

            if AChar.7 = 1 then

                AChar = AChar AND &H7F
                AChar = Lookup( AChar, CP1251_TO_CP866 )

            end if

    end select

    ' ������� ������, ������� ������������� ������� ������� CP866.
    PrintBin AChar

End Sub


' ������� ������ �� ���.
Sub Console_WriteData

    Local I As Byte, AByte As Byte

    ' -> "\033[" (ESC)
    AData(1) = &H1B
    AData(2) = &H5B

    I = 1

    AByte = AData(I)

    While AByte > 0

        PrintBin AByte
        Incr I
        AByte = AData(I)

    WEnd

    'Console_WriteString Command, cp866

End Sub


' ������� ������ �� ���.
Sub Console_WriteString( AText As String, ByVal ACodePage As Byte )

    Local I As Byte, Temp As Byte, Count As Byte
    Local Ptr As Word

    Count = Len( AText )

    ' ���� ����� ������, �� �������.
    If Count = 0 Then Exit Sub

    Ptr = VarPtr(AText)

    ' ����� ������
    For I = 1 To Count

        Temp = GetByte( Ptr )

        Incr Ptr

        Console_PutChar Temp, ACodePage

    Next

End Sub


' ������� �� ����� ������.
Sub Console_NewLine

    PrintBin &H0D
    PrintBin &H0A

End Sub


' ����� �����. ����������� ��������� ����� ����������� �������� ���� ������
' ��������� ������� ����������� ������� � ������������.
Sub Console_Beep( ByVal AFrequency As Word, ByVal ADuration As Byte  )

    Local Temp As Byte

    ' �����.
    Temp = AFrequency \ 100
    AData(3) = Temp + &H30

    ' �������.
    AFrequency = AFrequency mod 100

    Temp = AFrequency \ 10
    AData(4) = Temp + &H30

    ' �������.
    Temp = AFrequency mod 10
    AData(5) = Temp + &H30

    AData(6) = Asc( ";" )

    ' �������.
    Temp = ADuration \ 10
    AData(7) = Temp + &H30

    ' �������.
    Temp = ADuration mod 10
    AData(8) = Temp + &H30

    AData(9) = Asc( "B" )

    ' ����� ����� (BELL).
    AData(10) = &H07
    AData(11) = &H00

    Console_WriteData

End Sub


' �������� �����.
Sub Console_ClearScreen( ByVal AMode As Byte )

    Local Temp As Byte

    select case AMode

        ' �������� �� ������� �� ����� ������.
        case cmFromCursorToEnd: Temp = Asc("0")

        ' �������� �� ������ ������ �� �������.
        case cmFromBeginToCursor: Temp = Asc("1")

        ' �������� ���� �����.
        case cmAll: Temp = Asc("2")

        case else: Temp = Asc("2")

    end select

    AData(3) = Temp
    ' -> "J"
    AData(4) = &H4A
    AData(5) = &H00

    Console_WriteData

End Sub


' ������� ������.
Sub Console_ClearLine( ByVal AMode As Byte )

    Local Temp As Byte

    select case AMode

        ' �������� �� ������� �� ����� ������.
        case cmFromCursorToEnd: Temp = Asc("0")

        ' �������� �� ������ ������ �� �������.
        case cmFromBeginToCursor: Temp = Asc("1")

        ' �������� ��� ������.
        case cmAll: Temp = Asc("2")

        case else: Temp = Asc("0")

    end select

    AData(3) = Temp
    ' -> "K"
    AData(4) = &H4B
    AData(5) = &H00

    Console_WriteData

End Sub


' �������� n ������ �� ������� �������.
Sub Console_ClearForward( ByVal ACount As Byte )

    Local Temp As Byte

    if ACount = 0 then Exit Sub

    ' �������.
    Temp = ACount \ 10
    AData(3) = Temp + &H30

    ' �������.
    Temp = ACount mod 10
    AData(4) = Temp + &H30

    ' -> "X"
    AData(5) = &H58
    AData(6) = &H00

    Console_WriteData

End Sub


' �������� ������.
Sub Console_CursorOn

    AData(3) = &H3F
    AData(4) = &H32
    AData(5) = &H35
    AData(6) = &H68
    AData(7) = &H00

    Console_WriteData

End Sub


' �������� ������.
Sub Console_CursorOff

    AData(3) = &H3F
    AData(4) = &H32
    AData(5) = &H35
    AData(6) = &H6C
    AData(7) = &H00

    Console_WriteData

End Sub


' ��������� ��������� �������.
Sub Console_SaveCursor

    AData(3) = &H73
    AData(4) = &H00

    Console_WriteData

End Sub


' ������������ ����������� ��������� �������.
Sub Console_RestoreCursor

    AData(3) = &H75
    AData(4) = &H00

    Console_WriteData

End Sub


' ��������� ���������� ������.
Sub Console_SetForegroundColor( ByVal AColor As Byte )

    AData(3) = &H32 - AColor.3
    AData(4) = &H6D

    ' -> "\033[" (ESC)
    AData(5) = &H1B
    AData(6) = &H5B
    AData(7) = &H33
    AColor = AColor And &H07
    AData(8) = AColor + &H30
    AData(9) = &H6D
    AData(10) = &H00

    Console_WriteData

End Sub


' ��������� ���������� ����.
Sub Console_SetBackgroundColor( ByVal AColor As Byte )

    AData(3) = &H36 - AColor.3
    AData(4) = &H6D

    ' -> "\033[" (ESC)
    AData(5) = &H1B
    AData(6) = &H5B
    AData(7) = &H34
    AColor = AColor And &H07
    AData(8) = AColor + &H30
    AData(9) = &H6D
    AData(10) = &H00

    Console_WriteData

End Sub


' ��������� ���������.
Sub Console_SetTextAttributes( ByVal Attributes As Byte )

    Attributes = Attributes And &H0F
    AData(3) = Attributes + &H30
    AData(4) = &H6D
    AData(5) = &H00

    Console_WriteData

End Sub


' ��������� ���������� ������ � ����.
Sub Console_SetColor( ByVal AForegroundColor As Byte, ByVal ABackgroundColor As Byte )

    Console_SetTextAttributes atOff
    Console_SetForegroundColor AForegroundColor
    Console_SetBackgroundColor ABackgroundColor

End Sub


' ����������� � ������� Left � ������ Top.
Sub Console_MoveTo( ByVal ALeft As Byte, ByVal ATop As Byte )

    Local Temp As Byte

    if ALeft = 0 Or ATop = 0 then Exit Sub

    if ALeft > MAX_X Or ATop > MAX_Y then Exit Sub

    ' �������.
    Temp = ATop \ 10
    AData(3) = Temp + &H30

    ' �������.
    Temp = ATop mod 10
    AData(4) = Temp + &H30

    AData(5) = Asc( ";" )

    ' �������.
    Temp = ALeft \ 10
    AData(6) = Temp + &H30

    ' �������.
    Temp = ALeft mod 10
    AData(7) = Temp + &H30

    ' -> "f"
    AData(8) = &H66
    AData(9) = &H00

    Console_WriteData

End Sub


' ������������� ����������� ������� �� �����������.
Sub Console_Move( ByVal ADirection As Byte, ByVal ADelta As Byte )

    Local Temp As Byte

    ' �������.
    Temp = ADelta \ 10
    AData(3) = Temp + &H30

    ' �������.
    Temp = ADelta mod 10
    AData(4) = Temp + &H30

    select case ADirection

        ' ����� �� n �����.
        case mdUp: Temp = Asc("A")

        ' ���� �� n �����.
        case mdDown: Temp = Asc("B")

        ' ������ �� n �������.
        case mdForward: Temp = Asc("C")

        ' ����� �� n �������.
        case mdBackward: Temp = Asc("D")

        case else: Temp = Asc("C")

    end select

    AData(5) = Temp
    AData(6) = &H00

    Console_WriteData

End Sub


' ����� ���� � ������ � ����������.
Sub Console_DrawFrame( ByVal ALeft As Byte, ByVal ATop As Byte, _
    ByVal AWidth As Byte, ByVal AHeight As Byte, ByVal AColor As Byte, _
    ByVal ABgColor As Byte, ACaption As String )

    Local I As Byte, J As Byte, Temp As Byte
    Local ALen As Byte, ALeftLen As Byte, ARightLen As Byte

    Console_SetColor AColor, ABgColor
    Console_MoveTo ALeft, ATop

    ALen = Len( ACaption )

    AWidth = AWidth - 2

    ' ������� �������.
    PrintBin ACS_DBL_ULCORNER

    if ALen = 0 then

        For I = 1 To AWidth: PrintBin ACS_DBL_HLINE: Next

    else

        ALeftLen = AWidth - ALen

        Shift ALeftLen, Right, 1

        Decr ALeftLen

        For I = 1 To ALeftLen: PrintBin ACS_DBL_HLINE: Next

        PrintBin &H20

        Console_WriteString ACaption, cp1251

        PrintBin &H20

        ARightLen = AWidth - ALeftLen
        ARightLen = ARightLen - ALen
        ARightLen = ARightLen - 2

        For I = 1 To ARightLen: PrintBin ACS_DBL_HLINE: Next

    end if

    PrintBin ACS_DBL_URCORNER

    ' ������������ �������.
    Decr AHeight

    For I = 1 To AHeight

        Temp = ATop + I
        Console_MoveTo ALeft, Temp

        PrintBin ACS_DBL_VLINE

        For J = 1 To AWidth: PrintBin &H20: Next

        PrintBin ACS_DBL_VLINE

    Next

    ' ������ �������.
    Temp = ATop + AHeight
    Console_MoveTo ALeft, Temp

    PrintBin ACS_DBL_LLCORNER

    For I = 1 To AWidth: PrintBin ACS_DBL_HLINE: Next

    PrintBin ACS_DBL_LRCORNER

End Sub


' /****************
' *  � � � � � �
' *  ~~~~~~~~~~~
' ****************/

CP1251_TO_CP866:
    Data &H20,&H20,&H2C,&H20,&H20,&H20,&H20,&H20,&H20,&H20,&H20,&H3C,&H20,&H20,&H20,&H20
    Data &H20,&H27,&H27,&H22,&H22,&HF9,&H2D,&H2D,&H20,&H20,&H20,&H3E,&H20,&H20,&H20,&H20
    Data &HFF,&H20,&H20,&H6A,&H20,&H20,&H7C,&H20,&HF0,&H20,&H20,&H22,&H20,&H20,&H20,&H20
    Data &HF8,&H20,&H49,&H69,&H20,&H20,&H20,&HFA,&HF1,&H23,&H20,&H22,&H6A,&H53,&H73,&H20
    Data &H80,&H81,&H82,&H83,&H84,&H85,&H86,&H87,&H88,&H89,&H8A,&H8B,&H8C,&H8D,&H8E,&H8F
    Data &H90,&H91,&H92,&H93,&H94,&H95,&H96,&H97,&H98,&H99,&H9A,&H9B,&H9C,&H9D,&H9E,&H9F
    Data &HA0,&HA1,&HA2,&HA3,&HA4,&HA5,&HA6,&HA7,&HA8,&HA9,&HAA,&HAB,&HAC,&HAD,&HAE,&HAF
    Data &HE0,&HE1,&HE2,&HE3,&HE4,&HE5,&HE6,&HE7,&HE8,&HE9,&HEA,&HEB,&HEC,&HED,&HEE,&HEF