@echo off
setlocal ENABLEEXTENSIONS

REM Build all SmartXm tools with MinGW-w64 g++
REM - All targets use -mwindows
REM - USBMon additionally links with -lsetupapi

REM Ensure we're at the repo root
cd /d "%~dp0"

set "CC=g++"

REM Check for g++ availability
where %CC% >nul 2>&1
if errorlevel 1 (
  echo ERROR: g++ not found in PATH. Install MinGW-w64 and ensure g++ is available.
  exit /b 1
)

REM Optional: uncomment to see the actual commands
REM set "VERBOSE=1"

call :build_one "Tools\Keylogger\keylogger.cpp" "Tools\Keylogger\keylogger.exe" "-mwindows"
if errorlevel 1 goto :fail

call :build_one "Tools\ClipMon\clipmon.cpp"   "Tools\ClipMon\clipmon.exe"   "-mwindows"
if errorlevel 1 goto :fail

call :build_one "Tools\USBMon\usbmon.cpp"     "Tools\USBMon\usbmon.exe"     "-mwindows -lsetupapi"
if errorlevel 1 goto :fail

call :build_one "Tools\helper.cpp"             "Tools\helper.exe"             "-mwindows"
if errorlevel 1 goto :fail

echo.
echo SUCCESS: All tools built.
exit /b 0

:build_one
REM %1 = source, %2 = output, %3 = extra flags
set "SRC=%~1"
set "OUT=%~2"
set "FLAGS=%~3"

if not exist %SRC% (
  echo ERROR: Source not found: %SRC%
  exit /b 2
)

for %%P in ("%OUT%") do set "OUTDIR=%%~dpP"
if not exist "%OUTDIR%" (
  mkdir "%OUTDIR%" || (
    echo ERROR: Failed to create output directory: %OUTDIR%
    exit /b 3
  )
)

echo.
echo Building %OUT% ...
if defined VERBOSE (
  echo %CC% "%SRC%" -o "%OUT%" %FLAGS%
)
%CC% "%SRC%" -o "%OUT%" %FLAGS%
if errorlevel 1 (
  echo ERROR: Build failed for %OUT%
  exit /b 4
)
exit /b 0

:fail
exit /b %ERRORLEVEL%
