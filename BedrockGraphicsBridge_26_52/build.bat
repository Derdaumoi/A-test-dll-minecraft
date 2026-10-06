@echo off
setlocal
where cmake >nul 2>nul
if errorlevel 1 (
  echo [ERROR] CMake not found.
  exit /b 1
)

if not defined VSCMD_VER (
  echo [ERROR] Run this from "x64 Native Tools Command Prompt for VS 2022".
  echo VS Code is an editor; it does not provide cl.exe by itself.
  exit /b 2
)

cmake -S . -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b 3

cmake --build build --config Release
if errorlevel 1 exit /b 4

echo.
echo Built: build\BedrockGraphicsBridge.dll
