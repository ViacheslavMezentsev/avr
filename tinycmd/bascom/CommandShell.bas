$nocompile


Sub CommandShell_Info

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll
    Console_MoveTo 1, 25

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
    Data "[ATtiny2313]$ "
