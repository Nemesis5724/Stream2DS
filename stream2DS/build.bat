@echo off
set DEVKITPRO=C:\devkitPro
set DEVKITARM=C:\devkitARM

echo ================================
echo Building Stream2DS...
echo ================================

make clean
make > output.log 2>&1
type output.log
pause
