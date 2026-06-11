@echo off

echo ========================================
echo   CardMaster Backend Build Script
echo ========================================

echo.
echo [1/2] Compiling...
C:\mingw64\bin\gcc.exe main.c api.c auth.c data.c admin.c student.c mongoose.c -o server.exe -lws2_32

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo [2/2] Build success!
echo.
echo ========================================
echo   Starting server...
echo   Port: 4012
echo   Press Ctrl+C to stop
echo ========================================
echo.

server.exe
pause
