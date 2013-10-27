$nocompile


Sub CommandShell_Initialization

End Sub


Sub CommandShell_Info

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll
    Console_MoveTo 1, 25

    Caption = LookupStr( 0, CommandShellInfo )
    Console_WriteString Caption, cp1251

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
    Data "Командная оболочка, версия 0.1 (Bascom AVR)"