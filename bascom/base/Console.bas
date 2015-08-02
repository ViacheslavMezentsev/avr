$nocompile

'*******************************************************************************
'*
'* Автор: Мезенцев Вячеслав Николаевич
'* Электропочта: viacheslavmezentsev@gmail.com
'* skype: viacheslavmezentsev
'* Дата:  28.10.2013 г.
'*
'* Компилятор: BASCOM-AVR 2.0.7.5
'* Описание: Модуль для работы с ANSI терминалом.
'*
'*******************************************************************************

'$include "Console_Header.bas"


' /***********************
' *  Р Е А Л И З А Ц И Я
' *  ~~~~~~~~~~~~~~~~~~~
' ************************/


' Вывод символа из ОЗУ.
Sub Console_PutChar( ByVal AChar As Byte, ByVal ACodePage As Byte )

    Local Ptr As Word

    select case ACodePage

        case cp866:

        case cp1251:

            if AChar.7 = 1 then

                AChar = AChar AND &H7F

                Ptr = LoadLabel( CP1251_TO_CP866 )
                Ptr = Ptr + AChar
                AChar = CPeek( Ptr )

            end if

    end select

    ' Выводим символ, который соответствует кодовой таблице CP866.
    PrintBin AChar

    Exit Sub

CP1251_TO_CP866:

$ASM
    .DB &H20,&H20,&H2C,&H20,&H20,&H20,&H20,&H20,&H20,&H20,&H20,&H3C,&H20,&H20,&H20,&H20
    .DB &H20,&H27,&H27,&H22,&H22,&HF9,&H2D,&H2D,&H20,&H20,&H20,&H3E,&H20,&H20,&H20,&H20
    .DB &HFF,&H20,&H20,&H6A,&H20,&H20,&H7C,&H20,&HF0,&H20,&H20,&H22,&H20,&H20,&H20,&H20
    .DB &HF8,&H20,&H49,&H69,&H20,&H20,&H20,&HFA,&HF1,&H23,&H20,&H22,&H6A,&H53,&H73,&H20
    .DB &H80,&H81,&H82,&H83,&H84,&H85,&H86,&H87,&H88,&H89,&H8A,&H8B,&H8C,&H8D,&H8E,&H8F
    .DB &H90,&H91,&H92,&H93,&H94,&H95,&H96,&H97,&H98,&H99,&H9A,&H9B,&H9C,&H9D,&H9E,&H9F
    .DB &HA0,&HA1,&HA2,&HA3,&HA4,&HA5,&HA6,&HA7,&HA8,&HA9,&HAA,&HAB,&HAC,&HAD,&HAE,&HAF
    .DB &HE0,&HE1,&HE2,&HE3,&HE4,&HE5,&HE6,&HE7,&HE8,&HE9,&HEA,&HEB,&HEC,&HED,&HEE,&HEF
$END ASM

End Sub


' Вывести данные из ОЗУ.
Sub Console_WriteData

    ' -> "\033[" (ESC)
    CData(1) = Asc( "{027}" )
    CData(2) = Asc( "[" )

    Print CCommand;

End Sub


' Вывести строку из ОЗУ.
Sub Console_WriteString( AText As String, ByVal ACodePage As Byte )

    Local Temp As Byte
    Local Ptr As Word

    Ptr = VarPtr( AText )
    Temp = Inp( Ptr )

    While Temp > 0

        Console_PutChar Temp, ACodePage
        Incr Ptr
        Temp = Inp( Ptr )

    WEnd

End Sub


' Вывести строку из Flash.
Sub Console_WriteFlashString( ByVal APtr As Word, ByVal ACodePage As Byte )

    Local Temp As Byte

    Temp = CPeek( APtr )

    While Temp > 0

        Console_PutChar Temp, ACodePage
        Incr APtr
        Temp = CPeek( APtr )

    WEnd

End Sub


#if Console_Beep_Enabled

' Вывод звука. Современные терминалы могут проигрывать звуковой файл вместо
' звукового сигнала определённой частоты и длительности.
Sub Console_Beep( ByVal AFrequency As Word, ByVal ADuration As Byte  )

    ' Сотни.
    CData(3) = AFrequency \ 100
    CData(3) = CData(3) + &H30

    ' Десятки.
    AFrequency = AFrequency mod 100

    CData(4) = AFrequency \ 10
    CData(4) = CData(4) + &H30

    ' Единицы.
    CData(5) = AFrequency mod 10
    CData(5) = CData(5) + &H30

    CData(6) = Asc( ";" )

    ' Десятки.
    CData(7) = ADuration \ 10
    CData(7) = CData(7) + &H30

    ' Единицы.
    CData(8) = ADuration mod 10
    CData(8) = CData(8) + &H30

    CData(9) = Asc( "B" )

    ' Вывод звука (BELL).
    CData(10) = Asc( "{007}" )
    CData(11) = 0

    Console_WriteData

End Sub

#endif


' Очистить экран.
Sub Console_ClearScreen( ByVal AMode As Byte )

    CData(3) = Asc("0")
    CData(3) = CData(3) + AMode

    CData(4) = Asc( "J" )
    CData(5) = 0

    Console_WriteData

End Sub


' Очистка строки.
Sub Console_ClearLine( ByVal AMode As Byte )

    CData(3) = Asc( "0" )
    CData(3) = CData(3) + AMode

    CData(4) = Asc( "K" )
    CData(5) = 0

    Console_WriteData

End Sub


' Очистить n знаков от позиции курсора.
Sub Console_ClearForward( ByVal ACount As Byte )

    if ACount = 0 then Exit Sub

    ' Десятки.
    CData(3) = ACount \ 10
    CData(3) = CData(3) + &H30

    ' Единицы.
    CData(4) = ACount mod 10
    CData(4) = CData(4) + &H30

    CData(5) = Asc( "X" )
    CData(6) = 0

    Console_WriteData

End Sub


#if Console_Cursor_Enabled

' Показать курсор.
Macro Console_CursorOn : Print "{027}[?25h"; : End Macro

' Спрятать курсор.
Macro Console_CursorOff : Print "{027}[?25l" : End Macro

' Запомнить положение курсора.
Macro Console_SaveCursor : Print "{027}[s"; : End Macro

' Восстановить запомненное положение курсора.
Macro Console_RestoreCursor: Print "{027}[u"; : End Macro

#endif

' Переход на новую строку.
Macro Console_NewLine : Print : End Macro


' Установка параметров текста.
Sub Console_SetForegroundColor( ByVal AColor As Byte )

    CData(3) = &H32 - AColor.3
    CData(4) = Asc( "m" )
    CData(5) = 0

    Console_WriteData

    CData(3) = &H33
    AColor = AColor And &H07
    CData(4) = AColor + &H30
    CData(5) = Asc( "m" )
    CData(6) = 0

    Console_WriteData

End Sub


' Установка параметров фона.
Sub Console_SetBackgroundColor( ByVal AColor As Byte )

    CData(3) = &H36 - AColor.3
    CData(4) = Asc( "m" )
    CData(5) = 0

    Console_WriteData

    CData(3) = &H34
    AColor = AColor And &H07
    CData(4) = AColor + &H30
    CData(5) = Asc( "m" )
    CData(6) = 0

    Console_WriteData

End Sub


' Изменение атрибутов.
Sub Console_SetTextAttributes( ByVal Attributes As Byte )

    Attributes = Attributes And &H0F
    CData(3) = Attributes + &H30
    CData(4) = Asc( "m" )
    CData(5) = 0

    Console_WriteData

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

    ' Десятки.
    CData(3) = ATop \ 10
    CData(3) = CData(3) + &H30

    ' Единицы.
    CData(4) = ATop mod 10
    CData(4) = CData(4) + &H30

    CData(5) = Asc( ";" )

    ' Десятки.
    CData(6) = ALeft \ 10
    CData(6) = CData(6) + &H30

    ' Единицы.
    CData(7) = ALeft mod 10
    CData(7) = CData(7) + &H30

    CData(8) = Asc( "f" )
    CData(9) = 0

    Console_WriteData

End Sub


' Относительное перемещение курсора по направлению.
Sub Console_Move( ByVal ADirection As Byte, ByVal ADelta As Byte )

    ' Десятки.
    CData(3) = ADelta \ 10
    CData(3) = CData(3) + &H30

    ' Единицы.
    CData(4) = ADelta mod 10
    CData(4) = CData(4) + &H30

    CData(5) = Asc("A")
    CData(5) = CData(5) + ADirection

    CData(6) = 0

    Console_WriteData

End Sub


#if Console_DrawFrame_Enabled

' Вывод окна с рамкой и заголовком.
Sub Console_DrawFrame( ByVal ALeft As Byte, ByVal ATop As Byte, _
    ByVal AWidth As Byte, ByVal AHeight As Byte, ByVal AColor As Byte, _
    ByVal ABgColor As Byte, ACaption As String )

    Local I As Byte, J As Byte, Temp As Byte
    Local ALen As Byte, ALeftLen As Byte, ARightLen As Byte

    Console_SetColor AColor, ABgColor
    Console_MoveTo ALeft, ATop

    ALen = Len( ACaption )

    AWidth = AWidth - 2

    ' Верхняя граница.
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

    ' Вертикальные границы.
    AHeight = AHeight - 2

    For I = 1 To AHeight

        Temp = ATop + I
        Console_MoveTo ALeft, Temp

        PrintBin ACS_DBL_VLINE

        For J = 1 To AWidth: PrintBin &H20: Next

        PrintBin ACS_DBL_VLINE

    Next

    Incr AHeight

    ' Нижняя граница.
    Temp = ATop + AHeight
    Console_MoveTo ALeft, Temp

    PrintBin ACS_DBL_LLCORNER

    For I = 1 To AWidth: PrintBin ACS_DBL_HLINE: Next

    PrintBin ACS_DBL_LRCORNER

End Sub

#endif