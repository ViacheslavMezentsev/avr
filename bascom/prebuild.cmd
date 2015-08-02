@echo off
rem See: for /?

if not exist %~dpn1 (

    echo Project %~dpn1 not found.
    goto Exit
)

del %~dpn1\Version_Header.bas

cscript //NoLogo %~dp0versionfile-gen.js %~dpn1\Version_Header.bas %~dpn1\version

:Exit
