$nocompile


Dim HexChars As String * 17


Sub MemoryViewer_FormActivate

    'MemoryViewer_Initialization

    MemoryViewer_DrawTitle
    MemoryViewer_DrawMemory
    MemoryViewer_DrawFuncKeys

End Sub


Sub MemoryViewer_DrawTitle

    Console_SetColor clLightGray, clBlue
    Console_ClearScreen cmAll
    Console_CursorOff

    Console_SetColor clWhite, clGreen
    Caption = LookupStr( 0, ViewMemoryData )
    Console_WriteString Caption, cp1251
    Console_ClearLine cmFromCursorToEnd

End Sub


Sub MemoryViewer_DrawMemory

    Local I As Byte, J As Byte
    Local Tmp As Byte, Ch As Byte
    Local _offset As Word, Ptr As Word, Addr As Word
    Local OneChar As String * 1

    _offset = &H60

    HexChars = LookupStr( 0, HexCharsData )

    For I = 0 To 22

        Addr = I * 16
        Addr = Addr + _offset

        Tmp = I + 2
        Console_MoveTo 1, Tmp

        Console_SetColor clYellow, clBlue

        J = 4

        Do

            Decr J
            Ptr = Addr

            Shift Ptr, Right, J
            Shift Ptr, Right, J
            Shift Ptr, Right, J
            Shift Ptr, Right, J

            Ptr = Ptr And &H0F

            Incr Ptr

            OneChar = Mid( HexChars, Ptr, 1 )
            Ch = Asc( OneChar )

            Console_PutChar Ch, cp866

        Loop Until J = 0

        Ch = Asc(":")
        Console_PutChar Ch, cp866
        Console_PutChar &H20, cp866

        Console_SetForegroundColor clWhite

        For J = 0 To 15

            Tmp = GetByte( Addr )
            Ch = Tmp
            Shift Ch, Right, 4
            Incr Ch
            OneChar = Mid( HexChars, Ch, 1 )
            Ch = Asc( OneChar )
            Console_PutChar Ch, cp866

            Ch = Tmp and &H0F
            Incr Ch
            OneChar = Mid( HexChars, Ch, 1 )
            Ch = Asc( OneChar )
            Console_PutChar Ch, cp866

            Incr Addr

            Console_PutChar &H20, cp866

            if J = 7 then

                Ch = Asc("-")
                Console_PutChar Ch, cp866
                Console_PutChar &H20, cp866

            end if

        Next

        Console_PutChar ACS_VLINE, cp866
        Console_PutChar &H20, cp866

        ' Отображаем данные.
        Addr = I * 16
        Addr = Addr + _offset

        For J = 0 To 15

            Tmp = GetByte( Addr )

            if Tmp > Asc( " " ) then

                Console_PutChar Tmp, cp1251

            else

                Ch = Asc( "." )
                Console_PutChar Ch, cp866

            end if

            Incr Addr

        Next

        Console_ClearLine cmFromCursorToEnd

    Next


End Sub


Sub MemoryViewer_DrawFuncKeys

    Console_SetTextAttributes atOff
    Console_MoveTo 1, 25

    Console_SetColor clBlack, clWhite

    ' Заполняем пустое пространство до конца строки.
    Console_ClearLine cmFromCursorToEnd

End Sub


ViewMemoryData:
    Data "Просмотр памяти: ОЗУ (CP1251)"

HexCharsData:
    Data "0123456789ABCDEF"
