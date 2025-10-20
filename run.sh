#!/bin/bash
echo "Lancement de l'executable..."
echo "-----------------------------------"

if [ ! -f "./build/genetic" ]; then
    echo "Erreur : L'exécutable './build/genetic' n'a pas été trouvé."
    echo "Veuillez d'abord lancer './build_only.sh' ou './build_and_run.sh'."
    exit 1
fi

./build/genetic