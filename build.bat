@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%
g++ -g -std=c++17 -Iinclude src/main.cpp src/glad.c -Llib -lglfw3dll -o Workshop.exe
if %errorlevel% neq 0 (echo BUILD FAILED & exit /b 1)
echo Built Workshop.exe
