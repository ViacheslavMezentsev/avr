$nocompile


Dim Ch As Byte


' ����� ������� �� ��� � ��������� CP866.
Sub Console_PutChar( ByVal Ch As Byte )

    if Ch.7 = 1 then

        Ch = Ch AND &H7F
        Ch = Lookup( Ch, CP1251_TO_CP866 )

    end if

    ' ������� ������, ������� ������������� ������� ������� CP866.
    PrintBin Ch

End Sub


' ������� ������ �� ���.
Sub Console_WriteString( Text As String )

    Dim Code As Byte
    Dim I As Byte
    Dim OneChar As String * 1

    ' ���� ��������� ����� ����, �� �������.
    If Len( Text ) = 0 Then Exit Sub

    ' ����� ������
    For I = 1 To Len( Text )

        OneChar = Mid( Text, I, 1 )

        Code = Asc( OneChar )

        Console_PutChar Code

    Next

End Sub


' ����� �����. ����������� ��������� ����� ����������� �������� ���� ������
' ��������� ������� ����������� ������� � ������������.
Sub Console_Beep( ByVal Frequency As Word, ByVal Duration As Byte  )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    ' �����.
    Ch = Frequency \ 100
    Ch = Ch + &H30
    Console_PutChar Ch

    ' �������.
    Frequency = Frequency mod 100

    Ch = Frequency \ 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' �������.
    Ch = Frequency mod 10
    Ch = Ch + &H30
    Console_PutChar Ch

    Ch = Asc( ";" )
    Console_PutChar Ch

    ' �������.
    Ch = Duration \ 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' �������.
    Ch = Duration mod 10
    Ch = Ch + &H30
    Console_PutChar Ch

    Ch = Asc( "B" )
    Console_PutChar Ch

    ' ����� ����� (BELL).
    Console_PutChar &H07

End Sub


' �������� �����.
Sub Console_ClearScreen( ByVal Value As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    select case Value

        ' �������� �� ������� �� ����� ������.
        case cmFromCursorToEnd: Console_PutChar &H30

        ' �������� �� ������ ������ �� �������.
        case cmFromBeginToCursor: Console_PutChar &H31

        ' �������� ���� �����.
        case cmAll: Console_PutChar &H32

        case else: Console_PutChar &H32

    end select

    ' -> "J"
    Console_PutChar &H4A

End Sub


' ������� ������.
Sub Console_ClearLine( ByVal Value As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    select case Value

        ' �������� �� ������� �� ����� ������.
        case cmFromCursorToEnd: Console_PutChar &H30

        ' �������� �� ������ ������ �� �������.
        case cmFromBeginToCursor: Console_PutChar &H31

        ' �������� ��� ������.
        case cmAll: Console_PutChar &H32

        case else: Console_PutChar &H30

    end select

    ' -> "K"
    Console_PutChar &H4B

End Sub


' �������� n ������ �� ������� �������.
Sub Console_ClearForward( ByVal Count As Byte )

    if Count = 0 then Exit Sub

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    ' �������.
    Ch = Count \ 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' �������.
    Ch = Count mod 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' -> "X"
    Console_PutChar &H58

End Sub


' �������� ������.
Sub Console_CursorOn

    ' -> "\033[?25h"
    Console_PutChar &H1B
    Console_PutChar &H5B
    Console_PutChar &H3F
    Console_PutChar &H32
    Console_PutChar &H35
    Console_PutChar &H68

End Sub


' �������� ������.
Sub Console_CursorOff

    ' -> "\033[?25l"
    Console_PutChar &H1B
    Console_PutChar &H5B
    Console_PutChar &H3F
    Console_PutChar &H32
    Console_PutChar &H35
    Console_PutChar &H6C

End Sub


' ��������� ��������� �������.
Sub Console_SaveCursor

    ' -> "\033[s"
    Console_PutChar &H1B
    Console_PutChar &H5B
    Console_PutChar &H73

End Sub


' ������������ ����������� ��������� �������.
Sub Console_RestoreCursor

    ' -> "\033[u"
    Console_PutChar &H1B
    Console_PutChar &H5B
    Console_PutChar &H75

End Sub


' ��������� ���������� ������.
Sub Console_SetForegroundColor( ByVal Color As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    if Color.3 = 1 then

        Console_PutChar &H31

    else

        Console_PutChar &H32

    end if

    Console_PutChar &H6D

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    Console_PutChar &H33
    Color = Color And &H07
    Color = Color + &H30
    Console_PutChar Color

    Console_PutChar &H6D

End Sub


' ��������� ���������� ����.
Sub Console_SetBackgroundColor( ByVal Color As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    if Color.3 = 1 then

        Console_PutChar &H35

    else

        Console_PutChar &H36

    end if

    Console_PutChar &H6D

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    Console_PutChar &H34
    Color = Color And &H07
    Color = Color + &H30
    Console_PutChar Color

    Console_PutChar &H6D

End Sub


' ��������� ���������.
Sub Console_SetTextAttributes( ByVal Attributes As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    Attributes = Attributes And &H0F
    Attributes = Attributes + &H30
    Console_PutChar Attributes

    Console_PutChar &H6D

End Sub


' ��������� ���������� ������ � ����.
Sub Console_SetColor( ByVal AForegroundColor As Byte, ByVal ABackgroundColor As Byte )

    Console_SetTextAttributes atOff
    Console_SetForegroundColor AForegroundColor
    Console_SetBackgroundColor ABackgroundColor

End Sub


' ����������� � ������� Left � ������ Top.
Sub Console_MoveTo( ByVal ALeft As Byte, ByVal ATop As Byte )

    if ALeft = 0 Or ATop = 0 then Exit Sub

    if ALeft > MAX_X Or ATop > MAX_Y then Exit Sub

    ' -> "\033[" (ESC)
    Console_PutChar &H1B
    Console_PutChar &H5B

    ' �������.
    Ch = ATop \ 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' �������.
    Ch = ATop mod 10
    Ch = Ch + &H30
    Console_PutChar Ch

    Ch = Asc( ";" )
    Console_PutChar Ch

    ' �������.
    Ch = ALeft \ 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' �������.
    Ch = ALeft mod 10
    Ch = Ch + &H30
    Console_PutChar Ch

    ' -> "f"
    Console_PutChar &H66

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