@echo off
echo Lancement de l'executable (Release)...
echo -----------------------------------

:: Vérifie si l'exécutable existe avant de le lancer
if not exist ".\build\Release\genetic.exe" (
    echo Erreur : L'exécutable '.\build\Release\genetic.exe' n'a pas été trouvé.
    echo Veuillez d'abord lancer 'build_only.bat' ou 'build_and_run.bat'.
    pause
    exit /b 1
)

:: Lance l'exécutable depuis le dossier 'build\Release'
.\build\Release\genetic.exe
pause