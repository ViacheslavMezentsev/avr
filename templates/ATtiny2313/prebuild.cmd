@echo off
rem See: for /?
rm "%~dp0"Version.h
cscript //NoLogo "%~dp0"versionfile-gen.js "%~dp0"Version "%~dp0"version.txt