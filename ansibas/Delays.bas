$nocompile


'/**
' * Задержка в микросекундах
' */

Sub _delay_us( ByVal usec as DWORD)

    While usec > 0

        Waitus 1
        Decr usec

    WEnd

end sub


'/**
' * Задержка в миллисекундах
' */

Sub _delay_ms( ByVal msec as DWORD)


    While msec > 0

        Waitms 1
        Decr msec

    WEnd

end sub