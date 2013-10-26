$nocompile


Dim Ch As Byte


' Вывод символа из ОЗУ в кодировке CP866.
Sub Console_PutChar( ByVal AChar As Byte, ByVal ACodePage As Byte )

    select case ACodePage

        case cp866:

        case cp1251:

            if AChar.7 = 1 then

                AChar = AChar AND &H7F
                AChar = Lookup( AChar, CP1251_TO_CP866 )

            end if

    end select

    ' Выводим символ, который соответствует кодовой таблице CP866.
    PrintBin AChar

End Sub


' Вывести данные из ОЗУ.
Sub Console_WriteString( AText As String, ByVal ACodePage As Byte )

    Dim Code As Byte
    Dim I As Byte
    Dim OneChar As String * 1

    ' Если указатель равен нулю, то выходим.
    If Len( AText ) = 0 Then Exit Sub

    ' Вывод данных
    For I = 1 To Len( AText )

        OneChar = Mid( AText, I, 1 )

        Code = Asc( OneChar )

        Console_PutChar Code, ACodePage

    Next

End Sub


' Вывод звука. Современные терминалы могут проигрывать звуковой файл вместо
' звукового сигнала определённой частоты и длительности.
Sub Console_Beep( ByVal AFrequency As Word, ByVal ADuration As Byte  )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    ' Сотни.
    Ch = AFrequency \ 100
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Десятки.
    AFrequency = AFrequency mod 100

    Ch = AFrequency \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = AFrequency mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ";" )
    Console_PutChar Ch, cp866

    ' Десятки.
    Ch = ADuration \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ADuration mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( "B" )
    Console_PutChar Ch, cp866

    ' Вывод звука (BELL).
    Console_PutChar &H07, cp866

End Sub


' Очистить экран.
Sub Console_ClearScreen( ByVal AMode As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    select case AMode

        ' Очистить от курсора до конца экрана.
        case cmFromCursorToEnd: Ch = Asc("0")

        ' Очистить от начала экрана до курсора.
        case cmFromBeginToCursor: Ch = Asc("1")

        ' Очистить весь экран.
        case cmAll: Ch = Asc("2")

        case else: Ch = Asc("2")

    end select

    Console_PutChar Ch, cp866

    ' -> "J"
    Console_PutChar &H4A, cp866

End Sub


' Очистка строки.
Sub Console_ClearLine( ByVal AMode As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    select case AMode

        ' Очистить от курсора до конца строки.
        case cmFromCursorToEnd: Ch = Asc("0")

        ' Очистить от начала строки до курсора.
        case cmFromBeginToCursor: Ch = Asc("1")

        ' Очистить всю строку.
        case cmAll: Ch = Asc("2")

        case else: Ch = Asc("0")

    end select

    Console_PutChar Ch, cp866

    ' -> "K"
    Console_PutChar &H4B, cp866

End Sub


' Очистить n знаков от позиции курсора.
Sub Console_ClearForward( ByVal ACount As Byte )

    if ACount = 0 then Exit Sub

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    ' Десятки.
    Ch = ACount \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ACount mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' -> "X"
    Console_PutChar &H58, cp866

End Sub


' Показать курсор.
Sub Console_CursorOn

    ' -> "\033[?25h"
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866
    Console_PutChar &H3F, cp866
    Console_PutChar &H32, cp866
    Console_PutChar &H35, cp866
    Console_PutChar &H68, cp866

End Sub


' Спрятать курсор.
Sub Console_CursorOff

    ' -> "\033[?25l"
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866
    Console_PutChar &H3F, cp866
    Console_PutChar &H32, cp866
    Console_PutChar &H35, cp866
    Console_PutChar &H6C, cp866

End Sub


' Запомнить положение курсора.
Sub Console_SaveCursor

    ' -> "\033[s"
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866
    Console_PutChar &H73, cp866

End Sub


' Восстановить запомненное положение курсора.
Sub Console_RestoreCursor

    ' -> "\033[u"
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866
    Console_PutChar &H75, cp866

End Sub


' Установка параметров текста.
Sub Console_SetForegroundColor( ByVal AColor As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    if AColor.3 = 1 then Ch = Asc("1") else Ch = Asc("2")

    Console_PutChar Ch, cp866
    Console_PutChar &H6D, cp866

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    Console_PutChar &H33, cp866
    AColor = AColor And &H07
    AColor = AColor + &H30
    Console_PutChar AColor, cp866

    Console_PutChar &H6D, cp866

End Sub


' Установка параметров фона.
Sub Console_SetBackgroundColor( ByVal AColor As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    if AColor.3 = 1 then Ch = Asc("5") else Ch = Asc("6")

    Console_PutChar Ch, cp866
    Console_PutChar &H6D, cp866

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    Console_PutChar &H34, cp866
    AColor = AColor And &H07
    AColor = AColor + &H30
    Console_PutChar AColor, cp866

    Console_PutChar &H6D, cp866

End Sub


' Изменение атрибутов.
Sub Console_SetTextAttributes( ByVal Attributes As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    Attributes = Attributes And &H0F
    Attributes = Attributes + &H30
    Console_PutChar Attributes, cp866

    Console_PutChar &H6D, cp866

End Sub


' Установка параметров текста и фона.
Sub Console_SetColor( ByVal AForegroundColor As Byte, ByVal ABackgroundColor As Byte )

    Console_SetTextAttributes atOff
    Console_SetForegroundColor AForegroundColor
    Console_SetBackgroundColor ABackgroundColor

End Sub


' Переместить в позицию Left и строку Top.
Sub Console_MoveTo( ByVal ALeft As Byte, ByVal ATop As Byte )

    if ALeft = 0 Or ATop = 0 then Exit Sub

    if ALeft > MAX_X Or ATop > MAX_Y then Exit Sub

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    ' Десятки.
    Ch = ATop \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ATop mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    Ch = Asc( ";" )
    Console_PutChar Ch, cp866

    ' Десятки.
    Ch = ALeft \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ALeft mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' -> "f"
    Console_PutChar &H66, cp866

End Sub


' Относительное перемещение курсора по направлению.
Sub Console_Move( ByVal ADirection As Byte, ByVal ADelta As Byte )

    ' -> "\033[" (ESC)
    Console_PutChar &H1B, cp866
    Console_PutChar &H5B, cp866

    ' Десятки.
    Ch = ADelta \ 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    ' Единицы.
    Ch = ADelta mod 10
    Ch = Ch + &H30
    Console_PutChar Ch, cp866

    select case ADirection

        ' Вверх на n строк.
        case mdUp: Ch = Asc("A")

        ' Вниз на n строк.
        case mdDown: Ch = Asc("B")

        ' Вправо на n позиций.
        case mdForward: Ch = Asc("C")

        ' Влево на n позиций.
        case mdBackward: Ch = Asc("D")

        case else: Ch = Asc("C")

    end select

    Console_PutChar Ch, cp866

End Sub


' Вывод окна с рамкой и заголовком.
Sub Console_DrawFrame( ByVal ALeft As Byte, ByVal ATop As Byte, _
    ByVal AWidth As Byte, ByVal AHeight As Byte, ByVal AColor As Byte, _
    ByVal ABgColor As Byte, ACaption As String )

    Dim ALen As Byte
    Dim ALeftLen As Byte
    Dim ARightLen As Byte
    Dim J As Byte


    Console_SetColor AColor, ABgColor
    Console_MoveTo ALeft, ATop

    Console_PutChar ACS_DBL_ULCORNER, cp866

    ALen = Len( ACaption )

    AWidth = AWidth - 2

    ' Верхняя граница.
    if ALen <> 0 then

        ALeftLen = AWidth - ALen

        Shift ALeftLen, Right, 1

        For I = 1 To ALeftLen: Console_PutChar ACS_DBL_HLINE, cp866: Next

        Console_PutChar &H20, cp866

        Console_WriteString ACaption, cp1251

        Console_PutChar &H20, cp866

        ARightLen = AWidth - ALeftLen
        ARightLen = ARightLen - ALen
        ARightLen = ARightLen - 2

        For I = 1 To ARightLen: Console_PutChar ACS_DBL_HLINE, cp866: Next

    else

        Console_PutChar ACS_DBL_URCORNER, cp866

    end if

    Console_PutChar ACS_DBL_URCORNER, cp866

    ' Вертикальные границы.
    Decr AHeight

    For I = 1 To AHeight

        Ch = ATop + I
        Console_MoveTo ALeft, Ch

        Console_PutChar ACS_DBL_VLINE, cp866

        For J = 1 To AWidth: Console_PutChar &H20, cp866: Next

        Console_PutChar ACS_DBL_VLINE, cp866

    Next

    ' Нижняя граница.
    Ch = ATop + AHeight
    Console_MoveTo ALeft, Ch

    Console_PutChar ACS_DBL_LLCORNER, cp866

    For I = 1 To AWidth: Console_PutChar ACS_DBL_HLINE, cp866: Next

    Console_PutChar ACS_DBL_LRCORNER, cp866

End Sub


' /****************
' *  Д А Н Н Ы Е
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