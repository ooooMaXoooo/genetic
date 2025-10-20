cmake_minimum_required(VERSION 3.15)
project(GeneticAlgorithm)

# --- Configuration de C++ ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- CORRECTION FINALE POUR VISUAL STUDIO ---
# On force la sortie pour les générateurs "multi-config" (comme VS)
# en spécifiant la configuration (Release, Debug, etc.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
# Et pour les générateurs "single-config" (comme Makefiles)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# --- Définition de l'exécutable ---
add_executable(genetic
    genetic.cpp
    randomizer.cpp
    Vec.cpp
)

# On s'assure que 'data' est au MÊME endroit
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)