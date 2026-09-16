@echo off
rem Launch the built application (no Qt environment required)
if not exist "%~dp0build\release\samp2_4.exe" (
  echo samp2_4.exe not found. Run build.bat first.
  pause
  exit /b 1
)
start "" "%~dp0build\release\samp2_4.exe"
