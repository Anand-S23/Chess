@echo off

mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\chess\src\win32_chess.cpp user32.lib
popd
