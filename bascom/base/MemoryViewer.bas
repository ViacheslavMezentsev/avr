$nocompile

'$include "MemoryViewer_Header.bas"


Sub MemoryViewer_FormActivate

    'MemoryViewer_Initialization

    MemoryViewer_DrawTitle
    MemoryViewer_DrawMemory
    MemoryViewer_DrawFuncKeys

End Sub


Sub MemoryViewer_DrawTitle

    Local Ptr As Word
    
    Console_SetColor clLightGray, clBlue
    Console_ClearScreen cmAll
    Console_CursorOff

    Console_SetColor clWhite, clGreen
    Ptr = LoadLabel( ViewMemoryData )
    Console_WriteFlashString Ptr, cp1251
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
    Local _offset As Word, Ptr As Word, Addr As Word, HexPtr As Word

    _offset = &H60

    HexPtr = LoadLabel( HexCharsData )

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
            Ptr = HexPtr + Ptr           
            Ch = CPeek( Ptr )
            PrintBin Ch

        Loop Until J = 0

        Ch = Asc( ":" )
        PrintBin Ch
        PrintBin &H20

        Console_SetForegroundColor clWhite

        For J = 0 To 15

            Temp = Inp( Addr )
            Ch = Temp
            
            Shift Ch, Right, 4
            Ptr = HexPtr + Ch
            Ch = CPeek( Ptr )
            PrintBin Ch

            Ch = Temp and &H0F
            Ptr = HexPtr + Ch
            Ch = CPeek( Ptr )
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

            Temp = Inp( Addr )

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

    Exit Sub

HexCharsData:
    'Data "0123456789ABCDEF"
$ASM
    .DB &H30, &H31, &H32, &H33, &H34, &H35, &H36, &H37, &H38, &H39, &H41, &H42, &H43, &H44, &H45, &H46
$END ASM

End Sub


Sub MemoryViewer_DrawFuncKeys

    Console_SetTextAttributes atOff
    Console_MoveTo 1, 25

    Console_SetColor clBlack, clWhite

    ' Заполняем пустое пространство до конца строки.
    Console_ClearLine cmFromCursorToEnd

End Sub
