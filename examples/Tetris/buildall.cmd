@echo off
C:\MHVAVRTools\utils\bin\make.exe build all 2>&1 | C:\MHVAVRTools\utils\bin\sed.exe -e 's/\(\w\+\):\([0-9]\+\):/\1(\2):/'