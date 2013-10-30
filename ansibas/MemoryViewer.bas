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

'    Goto MemoryViewer_DrawTitle_End

'ViewMemoryData:
'$ASM
'    .DB "Просмотр памяти: ОЗУ (CP1251)"
'$END ASM

'MemoryViewer_DrawTitle_End:

End Sub


Sub MemoryViewer_DrawMemory

    Local I As Byte, J As Byte
    Local Temp As Byte, Ch As Byte
    Local _offset As Word, Ptr As Word, Addr As Word
    Local OneChar As String * 1

    _offset = &H60

    HexChars = LookupStr( 0, HexCharsData )

    For I = 0 To 22

        Addr = I * 16
        Addr = Addr + _offset

        Temp = I + 2
        Console_MoveTo 1, Temp

        Console_SetColor clYellow, clBlue

        J = 8

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
            PrintBin Ch

        Loop Until J = 0

        Ch = Asc(":")
        PrintBin Ch
        PrintBin &H20

        Console_SetForegroundColor clWhite

        For J = 0 To 15

            Temp = GetByte( Addr )
            Ch = Temp
            Shift Ch, Right, 4
            Incr Ch
            OneChar = Mid( HexChars, Ch, 1 )
            Ch = Asc( OneChar )
            PrintBin Ch

            Ch = Temp and &H0F
            Incr Ch
            OneChar = Mid( HexChars, Ch, 1 )
            Ch = Asc( OneChar )
            PrintBin Ch

            Incr Addr

            PrintBin &H20

            if J = 7 then

                Ch = Asc("-")
                PrintBin Ch
                PrintBin &H20

            end if

        Next

        PrintBin ACS_VLINE
        PrintBin &H20

        ' Отображаем данные.
        Addr = I * 16
        Addr = Addr + _offset

        For J = 0 To 15

            Temp = GetByte( Addr )

            if Temp > Asc( " " ) then

                Console_PutChar Temp, cp1251

            else

                Ch = Asc( "." )
                PrintBin Ch

            end if

            Incr Addr

        Next

        Console_ClearLine cmFromCursorToEnd

    Next

    Goto MemoryViewer_DrawMemory_End

HexCharsData:
    'Data "0123456789ABCDEF"
$ASM
    .DB &H30, &H31, &H32, &H33, &H34, &H35, &H36, &H37, &H38, &H39, &H41, &H42, &H43, &H44, &H45, &H46, &H00
$END ASM

MemoryViewer_DrawMemory_End:

End Sub


Sub MemoryViewer_DrawFuncKeys

    Console_SetTextAttributes atOff
    Console_MoveTo 1, 25

    Console_SetColor clBlack, clWhite

    ' Заполняем пустое пространство до конца строки.
    Console_ClearLine cmFromCursorToEnd

End Sub
