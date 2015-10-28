@echo off
rem See: for /?

echo Project path: %~dpn1
echo Current directory: %~dp0

if not exist %~dpn1 (

    echo Project %~dpn1 not found.
    goto Exit
)

chcp
echo.

if not exist C:\avr-gcc\utils\bin\make.exe (

    echo C:\avr-gcc\utils\bin\make.exe not found.
    goto Exit
)

cd %~dpn1
C:\avr-gcc\utils\bin\make.exe clean

:Exit