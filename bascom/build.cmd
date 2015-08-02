@echo off

echo.

if not exist %~dpn1 (

    echo Project %~dpn1 not found.
    goto Exit
)

echo Project path: %~dpn1
echo Current directory: %~dp0

chcp
echo.

if not exist %~dp0bascomp\bascomp.exe (

    echo %~dp0bascomp\bascomp.exe not found.
    goto Exit
)

if exist %~dpn1\version call %~dp0prebuild %1
echo.

echo Compiling %~dpn1\MainUnit.bas [auto] ...
echo.

%~dp0bascomp\bascomp %~dpn1\MainUnit.bas auto
echo.

if not %errorlevel% == 0 ( 

    type %~dpn1\MainUnit.err 
    goto Exit	

)

if exist %~dpn1\version call %~dp0postbuild %1

:Exit

