@echo off
DEICE.EXE
if ERRORLEVEL == 1 GOTO END
HT_BETA.EXE
if ERRORLEVEL == 1 GOTO ERROR
DEL HT_BETA.EXE
setup
goto END
:ERROR
echo Error installing HT_BETA.EXE!
:END

