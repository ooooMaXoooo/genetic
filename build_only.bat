@echo off
echo 1. Configuration de CMake...
:: Crée le dossier 'build' (s'il n'existe pas) et le configure
cmake -S . -B build
if %errorlevel% neq 0 (
    echo Erreur lors de la configuration CMake.
    pause
    exit /b %errorlevel%
)

echo 2. Compilation du projet (Release)...
:: Compile la version Release
cmake --build build --config Release
if %errorlevel% neq 0 (
    echo Erreur lors de la compilation.
    pause
    exit /b %errorlevel%
)

echo Compilation terminee. L'executable est dans '.\build\Release\genetic.exe'
pause