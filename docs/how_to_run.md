\# How to build and run locally (Windows)



\## Prerequisites

\- CMake installed

\- Visual Studio Build Tools 2022 (MSVC)

\- Python 3.11+



\## Build \& Test (Debug)

From repo root:



```bat

rmdir /s /q build

cmake -S . -B build

cmake --build build --config Debug

ctest --test-dir build -C Debug --output-on-failure



