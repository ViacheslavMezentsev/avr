$nocompile

'$include "CommandShell_Header.bas"


Sub CommandShell_Info

    Console_SetColor clLightGray, clBlack
    Console_ClearScreen cmAll
    Console_MoveTo 1, 25

    'Caption = LookupStr( 0, CommandShellInfo )
    'Console_WriteString Caption, cp1251

End Sub


Sub CommandShell_Prompt

    Caption = LookupStr( 0, Prompt )
    Console_SetForegroundColor clLightGreen
    Console_WriteString Caption, cp1251

    Console_SetForegroundColor clLightGray
    Console_ClearLine cmFromCursorToEnd
    Console_CursorOn

End Sub


Prompt:
    Data "[ATtiny2313]$ "

CommandShellInfo:
    Data "Command shell"
