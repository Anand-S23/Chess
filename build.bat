@echo off

:: libraries lib and include paths 
set SDL=W:\SDL2
set PRIM=W:\Primitive

set SDL_LIB=%SDL%\lib\solo_x64
set SDL_INC=%SDL%\include

if not exist build mkdir build 
pushd build 

cl -Zi -FC -MD /nologo ..\src\platform_chess.c -I%SDL_INC% -I%PRIM% %SDL_LIB%\SDL2.lib %SDL_LIB%\SDL2main.lib Shell32.lib -link /machine:x64 -subsystem:WINDOWS
popd