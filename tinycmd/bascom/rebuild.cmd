@echo off

"C:\Program Files\MCS Electronics\BASCOM-AVR\bascomp.exe" "%~dp0MainUnit.bas" SS=40 HW=16 FR=32
if %errorlevel% == 2 goto ShowError

cscript //NoLogo "%~dp0showrpt.js" "%~dp0MainUnit.rpt"
goto Exit

:ShowError
  type "%~dp0MainUnit.err"

:Exit
