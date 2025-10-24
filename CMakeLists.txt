cmake_minimum_required(VERSION 3.15)
project(GeneticAlgorithm)

# ============================================================
# 🔧 Configuration du compilateur et des options globales
# ============================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Génération du fichier compile_commands.json (pour Clangd) (fonctionne pour Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================
# 🧱 Organisation des fichiers de sortie
# ============================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ============================================================
# 📁 Inclusion des dossiers d'en-têtes
# ============================================================
include_directories(include)

# ============================================================
# 🧩 Liste des fichiers sources
# ============================================================
set(Genetic_SOURCES
    src/genetic.cpp
    src/randomizer.cpp
    src/Vec.cpp
)

# ============================================================
# 🚀 Définition de l'exécutable principal
# ============================================================
add_executable(genetic
    src/main.cpp
    ${Genetic_SOURCES}
)

# ============================================================
# 📦 Gestion des ressources (ex: dossier "data")
# ============================================================
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# ============================================================
# ✅ Fin du CMakeLists.txt
# ============================================================