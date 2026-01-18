@echo off
cls
echo ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
echo ÿÿÿInstall program for WW2 in Europe v1.07
echo ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
echo ÿ
if "%1" == "" goto howto
if "%1" == "c" goto howto
if "%1" == "C" goto howto
if "%1" == "d" goto howto
if "%1" == "D" goto howto
if "%1" == "a:" goto howto
if "%1" == "A:" goto howto
if "%1" == "b:" goto howto
if "%1" == "B:" goto howto
if not exist ww2.zip goto howto
if not exist install.bat goto howto
echo ÿ
echo WW2 in Europe installation
echo This will create a directory called \ww2e on the '%1' disk
echo and copy the game files into it. It will NOT make any other
echo changes to your system.
echo ÿ
echo Hit Ctrl-C if you do NOT wish to continue
pause
echo Creating directory '\ww2e' on '%1' drive
mkdir %1\ww2e
echo ÿ
echo Copying files...
copy *.* %1\ww2e
echo Changing to WW2 Directory...
%1
cd\ww2e
unzip ww2.zip
del *.zip
echo WW2 in Europe is now installed !
echo To play the game type:
echo WW2
goto done4
:howto
echo Place Master Disk in drive and go to that drive and type
echo "INSTALL DRIVE:" where DRIVE: is the TARGET drive.
echo :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo NOTE THAT YOU MUST INCLUDE THE COLON AS PART OF THE DRIVE DESIGNATION
echo :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo ÿ
echo EXAMPLE:
echo To copy the game from the A: drive to the C: drive,
echo you would type
echo            A:          [enter]
echo            INSTALL C:  [enter]
echo If your target disk is a drive other than C: use its letter instead.
:done4
