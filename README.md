# 🧬 Algorithme Génétique en C++

![Langage](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)
![Licence](https://img.shields.io/badge/license-MIT-lightgrey.svg)

Ce dépôt contient une implémentation simple mais complète d'un algorithme génétique (AG) en C++20. Il est conçu pour être un point de départ facile à compiler et à modifier pour vos propres problèmes d'optimisation.

L'objectif de cet AG est de trouver le **maximum** d'une fonction "fitness" personnalisable.

## 📚 Table des matières

* [Fonctionnalités](#-fonctionnalités)
* [Prérequis](#-prérequis)
* [Installation et Compilation](#-installation-et-compilation)
    * [Option 1 : Utiliser les Scripts (Recommandé)](#option-1--utiliser-les-scripts-recommandé)
    * [Option 2 : Compilation Manuelle](#option-2--compilation-manuelle)
* [Comment l'adapter à vos besoins](#-comment-ladapter-à-vos-besoins)
    * [1. Modifier les paramètres (`settings.h`)](#1-modifier-les-paramètres-settingsh)
    * [2. Modifier la fonction Fitness (`genetic.cpp`)](#2-modifier-la-fonction-fitness-geneticcpp)
* [Structure du projet](#-structure-du-projet)

---

## ✨ Fonctionnalités

* **Moderne :** Écrit en C++20.
* **Facile à compiler :** Utilise **CMake** pour une compilation multiplateforme.
* **Scripts de build :** Fournit des scripts `.bat` (Windows) et `.sh` (Linux/macOS) pour une compilation en une seule commande.
* **Flexible :** Permet de définir une fonction fitness sur des individus composés de N vecteurs en M dimensions.
* **Auto-adaptatif :** Les gènes contrôlant les probabilités de mutation évoluent eux-mêmes !
* **Complet :** Implémente la sélection par tournoi, le *crossover* au niveau du bit (*bit-level crossover*) et la mutation.

---

## 🔧 Prérequis

Avant de commencer, assurez-vous d'avoir les outils suivants installés sur votre machine :

* [**CMake**](https://cmake.org/download/) (version 3.15 ou ultérieure)
* Un **compilateur C++20** (par exemple, MSVC 2022+, g++ 10+ ou Clang 12+)
* [**Git**](https://git-scm.com/)

---

## 🚀 Installation et Compilation

### Option 1 : Utiliser les Scripts (Recommandé)

C'est la méthode la plus simple. Après avoir cloné le dépôt, exécutez simplement le script correspondant à votre système.

1.  Clonez ce dépôt:
    ```bash
    git clone https://github.com/ooooMaXoooo/genetic.git
    cd genetic
    ```

2.  Exécutez le script de votre choix :

    **Sur Windows (double-cliquez) :**
    * `build_and_run.bat` : Compile tout et lance le programme.
    * `build_only.bat` : Compile seulement.
    * `run.bat` : Lance le programme (s'il est déjà compilé).

    **Sur Linux / macOS :**
    ```bash
    # Rendez les scripts exécutables (une seule fois)
    chmod +x *.sh

    # Exécutez le script de votre choix
    ./build_and_run.sh  # Compile tout et lance le programme
    ./build_only.sh     # Compile seulement
    ./run.sh            # Lance le programme (s'il est déjà compilé)
    ```

### Option 2 : Compilation Manuelle

Si vous préférez compiler manuellement, suivez ces étapes.

1.  Clonez le dépôt ( **N'OUBLIEZ PAS de remplacer l'URL** ) :
    ```bash
    git clone [https://github.com/](https://github.com/)[VOTRE_NOM_UTILISATEUR_GIT]/[NOM_DU_REPO.git]
    cd [NOM_DU_REPO]
    ```

2.  Créez un dossier de `build` et configurez CMake :
    ```bash
    mkdir build
    cd build
    ```

3.  **Lancez CMake.** Cette étape diffère selon votre système :

    * **Windows (avec Visual Studio) :**
        ```bash
        # Génère les fichiers de projet pour Visual Studio
        cmake ..
        ```
    * **Windows (avec MinGW g++) :**
        ```bash
        # Génère des Makefiles pour MinGW
        cmake .. -G "MinGW Makefiles"
        ```
    * **Linux / macOS :**
        ```bash
        # Génère des Makefiles standards (en mode Release)
        cmake .. -DCMAKE_BUILD_TYPE=Release
        ```

4.  Compilez le projet :
    * **Si vous avez utilisé Visual Studio :**
        ```bash
        # Compile la configuration Release
        cmake --build . --config Release
        ```
    * **Si vous avez utilisé MinGW ou Linux/macOS :**
        ```bash
        # Utilise la commande de build native (make)
        cmake --build .
        ```

5.  Exécutez le programme **en restant dans le dossier `build/`** :
    ```bash
    # Sur Windows
    .\Release\genetic.exe

    # Sur Linux / macOS
    ./genetic
    ```
    Le programme créera un dossier `data` (ex: `build/Release/data/`) pour y stocker ses résultats.

---

## 🔧 Comment l'adapter à vos besoins

La personnalisation de l'algorithme se fait principalement à deux endroits :

### 1. Modifier les paramètres (`settings.h`)

Le fichier `settings.h` est le centre de contrôle de l'algorithme. Vous pouvez y modifier :

* **La population :** `PopulationSize`, `HalfPopulationSize`, etc.
* **La structure de l'individu :** `NumberOfVectors` (combien de vecteurs par agent) et `Dimension` (la dimension de chaque vecteur).
* **Le problème :** `min_real` et `max_real` pour définir l'intervalle de recherche de votre fonction.
* **La simulation :** `maxGen` (nombre de générations).
* **La sauvegarde :** `save` (pour activer la sauvegarde) et `save_interval`.

### 2. Modifier la fonction Fitness (`genetic.cpp`)

Ce code peut être utilisé pour **maximiser** une fonction. (Remarquez qu'il suffit de maximiser l'opposé d'une fonction pour la minimiser).

La fonction à maximiser (`fitness`) se trouve au début du fichier **`genetic.cpp`**. Elle prend un nombre quelconque de vecteurs, en dimension quelconque, et doit renvoyer un `real` (votre score).

```cpp
// Fichier : genetic.cpp

inline float fitness (const std::array<Vec<Dimension>, NumberOfVectors>& vecteurs) { 
    // 'vecteurs' est l'individu à évaluer.
    // Il contient 'NumberOfVectors' vecteurs, chacun de dimension 'Dimension'.
    
    // --- VOTRE LOGIQUE ICI ---
    // L'exemple actuel (pour Dimension=1, NumberOfVectors=1) est f(x) = x²
    // que l'algorithme va chercher à maximiser.
    
    real sum = 0;   
    for(size_t i=0; i<NumberOfVectors; i++) {
        size_t SIZE = vecteurs[i].size();
        for(size_t j=0; j<SIZE; j++) {
            sum += vecteurs[i][j];
        }
    }
    return sum*sum;
    
    // --- FIN DE VOTRE LOGIQUE ---
};
````

**Important :** Vous pouvez modifier le **corps** de la fonction, mais ne changez pas sa **signature** (son nom ou ses arguments) car le reste du code en dépend.

-----

## 📁 Structure du projet

```
.
├── .gitignore             # Ignore les fichiers de build
├── CMakeLists.txt         # Le "plan" de compilation
├── LICENSE                # Votre licence (MIT)
├── README.md              # Ce fichier
├── build_and_run.bat      # (Script Windows)
├── build_only.bat         # (Script Windows)
├── run.bat                # (Script Windows)
├── build_and_run.sh       # (Script Linux/macOS)
├── build_only.sh          # (Script Linux/macOS)
├── run.sh                 # (Script Linux/macOS)
├── settings.h             # Paramètres principaux (Population, Dimensions...)
├── genetic.cpp            # Fichier principal (main(), fitness(), genetic_algorithm())
├── Vec.h / .cpp           # Classe utilitaire pour les vecteurs
├── randomizer.h / .cpp    # Classe utilitaire pour la génération aléatoire
├── utils.h                # Fonctions de conversion (bin_to_real...)
└── data/                  # (N'est pas dans le dépôt, généré par le programme)
```

-----

## 📄 Licence

Ce projet est sous licence MIT. N'hésitez pas à l'utiliser et à l'adapter \!
