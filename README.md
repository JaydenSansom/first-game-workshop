# Workshop Base

Minimal GLFW + glad starter. Opens an 800x800 window with an OpenGL context and clears it each frame

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
g++ -g -std=c++17 -Iinclude src/main.cpp src/glad.c -Llib -lglfw3dll -o Workshop.exe -static
```

| Part | What it does |
|------|--------------|
| `g++` | The compiler. MSYS2 installs it to `C:\msys64\ucrt64\bin`, which `build.bat` adds to PATH first. |
| `-g` | Include debug symbols so you can step through in a debugger. |
| `-std=c++17` | Use the C++17 language standard. |
| `-Iinclude` | Look in `include/` for `#include <GLFW/glfw3.h>` and `<glad/glad.h>`. |
| `src/main.cpp src/glad.c` | The two files that get compiled. `glad.c` loads OpenGL function pointers at runtime. |
| `-Llib -lglfw3dll` | Look in `lib/` and link against `libglfw3dll.a`, the import library for `glfw3.dll`. |
| `-o Workshop.exe` | Name of the output. |

The result is `Workshop.exe` in this folder that runs and shows your game!

## Where to work

`src/main.cpp` is the only project file. Put your OpenGL calls after the `Your rendering goes here` comment in the main loop.
When adding more source files list them after `src/main.cpp` in `build.bat`.

## Layout

```
src/main.cpp        your code
src/glad.c          OpenGL loader (don't edit)
include/            GLFW and glad headers
lib/                GLFW import library
glfw3.dll           needed next to the exe at runtime
```
