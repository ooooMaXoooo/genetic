#!/bin/bash
set -e # Arrête le script si une commande échoue

# 1. Configuration de CMake (en mode Release)
echo "1. Configuration de CMake..."
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 2. Compilation
echo "2. Compilation du projet..."
cmake --build build

# 3. Lancement
echo "3. Lancement de l'application..."
echo "-----------------------------------"
./build/genetic