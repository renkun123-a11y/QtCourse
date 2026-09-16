@echo off
setlocal
rem ---- Adjust these two paths if your Qt / MinGW are installed elsewhere ----
set QTDIR=E:\Qt\6.7.2\mingw_64
set MINGWDIR=E:\Qt\Tools\mingw1310_64
set PATH=%MINGWDIR%\bin;%QTDIR%\bin;%PATH%

if not exist "%~dp0build" mkdir "%~dp0build"
cd /d "%~dp0build"

echo [1/4] qmake ...
"%QTDIR%\bin\qmake.exe" ..\samp2_4App\samp2_4.pro
if errorlevel 1 goto err

echo [2/4] mingw32-make ...
"%MINGWDIR%\bin\mingw32-make.exe" -f Makefile.Release -j4
if errorlevel 1 goto err

echo [3/4] windeployqt ...
"%QTDIR%\bin\windeployqt.exe" --release --no-translations --no-system-d3d-compiler --no-opengl-sw release\samp2_4.exe

echo [4/4] copy MinGW runtime DLLs ...
copy /Y "%MINGWDIR%\bin\libgcc_s_seh-1.dll" release\ >nul
copy /Y "%MINGWDIR%\bin\libstdc++-6.dll"   release\ >nul
copy /Y "%MINGWDIR%\bin\libwinpthread-1.dll" release\ >nul

echo.
echo Build OK  --^>  build\release\samp2_4.exe
exit /b 0

:err
echo.
echo Build FAILED
exit /b 1
