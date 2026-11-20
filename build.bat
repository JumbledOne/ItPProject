@echo off
setlocal

cd /d "%~dp0"

if not exist "build" mkdir "build"

rem Keep CMake's generated files outside build; build is reserved for app.exe.
cmake -S . -B .cmake-build
if errorlevel 1 exit /b %errorlevel%

cmake --build .cmake-build --config Release
if errorlevel 1 exit /b %errorlevel%

echo Build finished: build\app.exe
endlocal
