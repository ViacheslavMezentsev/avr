@echo off

"C:\Program Files\MCS Electronics\BASCOM-AVR\bascomp.exe" "%~dp0MainUnit.bas" SS=40 HW=16 FR=32
if %errorlevel% == 2 type "%~dp0MainUnit.err"
