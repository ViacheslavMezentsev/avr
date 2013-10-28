$nocompile


Sub CommandShell_Initialization

End Sub


Sub CommandShell_Info

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll
    Console_MoveTo 1, 25

    Caption = LookupStr( 0, CommandShellInfo )
    Console_WriteString Caption, cp1251

    ' Вывод номера версии.
    Caption = Version(2)
    Console_WriteString Caption, cp1251

    Print " (";

    ' Вывод даты сборки.
    Caption = Version(1)
    Console_WriteString Caption, cp1251

    Print ")"

End Sub


Sub CommandShell_Prompt

    Console_NewLine

    Caption = LookupStr( 0, Prompt )
    Console_SetForegroundColor clLightGreen
    Console_WriteString Caption, cp1251

    Console_SetForegroundColor clLightGray
    Console_ClearLine cmFromCursorToEnd
    Console_CursorOn

End Sub


Prompt:
    Data "[ATmega16]$ "

CommandShellInfo:
    Data "Командная оболочка (Bascom AVR), версия "