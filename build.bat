@echo off

:: libraries lib and include paths 
set SDL=W:\SDL2

set SDL_INC=%SDL%\include
set SDL_LIB=%SDL%\lib\x64\SDL2.lib %SDL%\lib\x64\SDL2main.lib %SDL%\lib\x64\SDL2_image.lib 

if not exist build mkdir build 
pushd build 

cl -Zi -FC -MD /nologo ..\src\platform_chess.c -I%SDL_INC% %SDL_LIB% Shell32.lib -link /machine:x64 -subsystem:CONSOLE /out:Chess.exe
popd