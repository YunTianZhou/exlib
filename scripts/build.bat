@echo off
cd /d "%~dp0"

cd ..
mkdir build
cd build
cmake .. -A x64
cmake --build . --config Release
cd ../scripts
