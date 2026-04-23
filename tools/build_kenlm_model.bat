@echo off
setlocal

if "%~1"=="" goto usage

set "ARPA_FILE=%~1"
if "%~2"=="" (
  set "OUTPUT_FILE=%~dpn1.binary"
) else (
  set "OUTPUT_FILE=%~2"
)

if not exist "%ARPA_FILE%" (
  echo ARPA file not found: %ARPA_FILE%
  exit /b 1
)

set "BUILD_BINARY_EXE="
if not defined KENLM_ROOT (
  if exist "%~dp0..\..\kenlm\CMakeLists.txt" set "KENLM_ROOT=%~dp0..\..\kenlm"
)
if defined KENLM_ROOT (
  if exist "%KENLM_ROOT%\bin\build_binary.exe" set "BUILD_BINARY_EXE=%KENLM_ROOT%\bin\build_binary.exe"
  if exist "%KENLM_ROOT%\build\bin\Release\build_binary.exe" set "BUILD_BINARY_EXE=%KENLM_ROOT%\build\bin\Release\build_binary.exe"
  if exist "%KENLM_ROOT%\build\bin\build_binary.exe" set "BUILD_BINARY_EXE=%KENLM_ROOT%\build\bin\build_binary.exe"
)
if not defined BUILD_BINARY_EXE set "BUILD_BINARY_EXE=build_binary.exe"

echo Using build_binary: %BUILD_BINARY_EXE%
echo Input ARPA: %ARPA_FILE%
echo Output binary: %OUTPUT_FILE%

"%BUILD_BINARY_EXE%" %3 %4 %5 %6 %7 %8 %9 "%ARPA_FILE%" "%OUTPUT_FILE%"
exit /b %errorlevel%

:usage
echo Usage: build_kenlm_model.bat model.arpa [output.binary] [extra build_binary args...]
echo.
echo Examples:
echo   build_kenlm_model.bat zh.arpa
echo   build_kenlm_model.bat zh.arpa zh.binary -T temp
echo.
echo Set KENLM_ROOT to point to a KenLM install/build tree if build_binary.exe is not on PATH.
exit /b 1
