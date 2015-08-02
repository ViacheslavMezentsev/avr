@echo off
rem See: for /?

if not exist %~dpn1 (

    echo Project %~dpn1 not found.
    goto Exit
)

cscript //NoLogo %~dp0versionfile-update.js %~dpn1\version

:Exit