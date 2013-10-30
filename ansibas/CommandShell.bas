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
    'Caption = Version(2)
    Caption = Version_GetVersionString()
    Console_WriteString Caption, cp1251

    Print " (";

    ' Вывод даты сборки.
    'Caption = Version(1)
    Caption = Version_GetBuildDateString()
    Console_WriteString Caption, cp1251

    Print ")"

'    Goto CommandShell_Info_End

'CommandShellInfo:
'    'DATA "Командная оболочка (Bascom AVR), версия "
'$ASM
'    .DB "Командная оболочка (Bascom AVR), версия "
'$END ASM

'CommandShell_Info_End:

End Sub


Sub CommandShell_Prompt

    Console_NewLine

    Caption = LookupStr( 0, Prompt )
    Console_SetForegroundColor clLightGreen
    Console_WriteString Caption, cp1251

    Console_SetForegroundColor clLightGray
    Console_ClearLine cmFromCursorToEnd
    Console_CursorOn

'    Goto CommandShell_Prompt_End

'Prompt:
'$ASM
'    .DB "[ATmega16]$ "
'$END ASM

'CommandShell_Prompt_End:

End Sub
