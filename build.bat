@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%
g++ -g -std=c++17 -DSFML_STATIC -IC:/msys64/ucrt64/include src/main.cpp -LC:/msys64/ucrt64/lib -static -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lbox2d -lfreetype -lopengl32 -lwinmm -lgdi32 -luser32 -ladvapi32 -o Workshop.exe
if %errorlevel% neq 0 (echo BUILD FAILED & exit /b 1)
echo Built Workshop.exe
