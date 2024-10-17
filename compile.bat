::.\build\mingw64\bin\gcc.exe main.c -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
:: -mwindows
:: gcc main.c -o main -O1 -Wall -std=c99 -Wno-missing-braces -L ./lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
SET GCC=".\build\mingw64\bin\gcc.exe"
%GCC% main.c -o main.exe .\build\lib\libraylib.a -L .\build\lib -I .\build\include\ -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
PAUSE
