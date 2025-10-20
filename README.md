
# 🧬 Algorithme Génétique en C++

Ce dépôt contient une implémentation simple mais complète d'un algorithme génétique (AG) en C++20. Il est conçu pour être un point de départ facile à compiler et à modifier pour vos propres problèmes d'optimisation.

L'objectif de cet AG est de trouver le **maximum** d'une fonction "fitness" personnalisable.

## 📚 Table des matières

  * [Fonctionnalités](#-fonctionnalités)
  * [Prérequis](#-prérequis)
  * [Compilation](#-compilation)
  * [Comment l'adapter à vos besoins](#-comment-ladapter-à-vos-besoins)
      * [1. Modifier les paramètres (`settings.h`)](#1-modifier-les-paramètres-settingsh)
      * [2. Modifier la fonction Fitness (`genetic.cpp`)](#2-modifier-la-fonction-fitness-geneticcpp)
  * [Structure du projet](#-structure-du-projet)

-----

## ✨ Fonctionnalités

  * **Moderne :** Écrit en C++20.
  * **Facile à compiler :** Utilise **CMake** pour une compilation multiplateforme.
  * **Flexible :** Permet de définir une fonction fitness sur des individus composés de N vecteurs en M dimensions.
  * **Auto-adaptatif :** Les gènes contrôlant les probabilités de mutation évoluent eux-mêmes \!
  * **Complet :** Implémente la sélection par tournoi, le *crossover* au niveau du bit (*bit-level crossover*) et la mutation.

-----

## 🔧 Prérequis

Avant de commencer, assurez-vous d'avoir les outils suivants installés sur votre machine :

  * [**CMake**](https://cmake.org/download/) (version 3.15 ou ultérieure)
  * Un **compilateur C++20** (par exemple, MSVC 2022+, g++ 10+ ou Clang 12+)
  * [**Git**](https://git-scm.com/)

-----

## 🚀 Compilation

1.  Clonez ce dépôt :

    ```bash
    git clone https://github.com/VOTRE_NOM/VOTRE_REPO.git
    cd VOTRE_REPO
    ```

2.  Créez un dossier de `build` et configurez CMake :

    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  Compilez le projet :

    ```bash
    # Sur Windows (avec Visual Studio)
    cmake --build . --config Release

    # Sur Linux / macOS
    cmake --build .
    ```

4.  L'exécutable `genetic` (ou `genetic.exe`) sera créé directement dans le dossier `build/`, à côté d'un dossier `data/` prêt à l'emploi.

5.  Exécutez le programme :

    ```bash
    # Sur Windows
    .\genetic.exe

    # Sur Linux / macOS
    ./genetic
    ```

-----

## 🔧 Comment l'adapter à vos besoins

La personnalisation de l'algorithme se fait principalement à deux endroits :

### 1\. Modifier les paramètres (`settings.h`)

Le fichier `settings.h` est le centre de contrôle de l'algorithme. Vous pouvez y modifier :

  * **La population :** `PopulationSize`, `HalfPopulationSize`, etc.
  * **La structure de l'individu :** `NumberOfVectors` (combien de vecteurs par agent) et `Dimension` (la dimension de chaque vecteur).
  * **Le problème :** `min_real` et `max_real` pour définir l'intervalle de recherche de votre fonction.
  * **La simulation :** `maxGen` (nombre de générations).
  * **La sauvegarde :** `save` (pour activer la sauvegarde) et `save_interval`.

### 2\. Modifier la fonction Fitness (`genetic.cpp`)

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
```

**Important :** Vous pouvez modifier le **corps** de la fonction, mais ne changez pas sa **signature** (son nom ou ses arguments) car le reste du code en dépend.

-----

## 📁 Structure du projet

```
.
├── CMakeLists.txt     # Le "plan" de compilation
├── settings.h         # Paramètres principaux (Population, Dimensions...)
├── genetic.cpp        # Fichier principal (main(), fitness(), genetic_algorithm())
├── Vec.h / .cpp         # Classe utilitaire pour les vecteurs
├── randomizer.h / .cpp  # Classe utilitaire pour la génération aléatoire
├── utils.h              # Fonctions de conversion (bin_to_real...)
└── data/              # Dossier de sortie pour les sauvegardes
```

-----

## 📄 Licence

Ce projet est sous licence MIT. N'hésitez pas à l'utiliser et à l'adapter \!
