@echo off

if not exist %~dpn1 (

    echo Project %~dpn1 not found.
    goto Exit
)

del %~dpn1\*.BM  2> nul
del %~dpn1\*.lst 2> nul
del %~dpn1\*.rpt 2> nul
del %~dpn1\*.dbg 2> nul
del %~dpn1\*.obj 2> nul
del %~dpn1\*.hex 2> nul
del %~dpn1\*.log 2> nul
del %~dpn1\*.bin 2> nul
del %~dpn1\*.err 2> nul

:Exit
