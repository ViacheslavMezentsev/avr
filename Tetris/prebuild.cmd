@echo off
rm %1Version.h
cscript //NoLogo %1versionfile-gen.js %1Version %1version.txt