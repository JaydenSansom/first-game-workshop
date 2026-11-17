# Workshop Base

Minimal SFML + Box2D starter.

## Requirements

- Windows
- MSYS2 with the UCRT64 toolchain: https://www.msys2.org
  After installing, open the MSYS2 UCRT64 shell and run:
  `pacman -S mingw-w64-ucrt-x86_64-gcc`
  `pacman -S mingw-w64-ucrt-x86_64-box2d`
  'pacman -S mingw-w64-ucrt-x86_64-sfml'
  (`build.bat` expects g++ at `C:\msys64\ucrt64\bin`; edit the PATH line if you installed elsewhere.)

## Build and run

Double-click `build.bat`, or from a terminal in this folder:

```
build.bat
Workshop.exe
```

Alternatively in VS Code: `Ctrl+Shift+B` builds, and the "Run Workshop" task builds then launches.

## How the build works

`build.bat` runs one g++ command:

```
g++ -g -std=c++17 -DSFML_STATIC -IC:/msys64/ucrt64/include src/main.cpp -LC:/msys64/ucrt64/lib -static -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lbox2d -lfreetype -lopengl32 -lwinmm -lgdi32 -luser32 -ladvapi32 -o Workshop.exe
```

The result is `Workshop.exe` in this folder that runs and shows your game!

## Where to work

`src/main.cpp` is the only project file. Put your draw calls after the `Window.clear()` line in the main loop.
When adding more source files list them after `src/main.cpp` in `build.bat`.
