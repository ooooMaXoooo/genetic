#pragma once

/*
    Pour avoir les couleurs des commentaires, il faut installer l'extension Better Comments (pour VS Code) :
    https://marketplace.visualstudio.com/items?itemName=aaron-bond.better-comments


?   Un commentaire écrit de cette couleur, dans ce fichier uniquement, correspond à une valeur modifiable par l'utilisateur. (--> ? devant le commentaire)
!   Un commentaire écrit de cette couleur, dans ce fichier uniquement, correspond à une valeur non modifiable par l'utilisateur. (--> ! devant le commentaire)
*/



//======= Paramètres de la représentation des nombres =======//

using real = double;                                                                                //? le type qui représente un réel

constexpr real min_real     = -1000;                                                                //? la valeur minimale que peut prendre un réel
constexpr real max_real     =  1000;                                                                //? la valeur maximale que peut prendre un réel
constexpr real real_size    = max_real - min_real;                                                  //! la taille de l'intervalle de représentation des réels

constexpr int Nb_bin    = 32;                                                                       //? le nombre de bits sur lequel est codé un gène
constexpr size_t bin_max  = (1ULL << Nb_bin) - 1;                                                   //! la valeur maximale que peut prendre un gène

using integer = uint32_t;                                                                           //? le type représentant un entier NATUREL



//======= Paramètres de l'algorithme génétique =======//

constexpr size_t HalfPopulationSize     = 3000;                                                     //? la moité de la taille de la population
constexpr size_t PopulationSize         = 2*HalfPopulationSize;                                     //! la taille de la population

constexpr size_t NumberOfVectors    = 2;                                                            //? la taille d'un uplet de vecteurs
constexpr size_t Dimension          = 3;                                                            //? la dimension de l'espace vectoriel E

constexpr size_t maxGen = 1000;                                                                     //? la dernière génération d'enfants


//======= Paramètres de sauvegarde dans des fichiers =======//

#define directory "./data"                                                                          //? le dossier dans lequel sauvegarder les fichiers de sauvegarde
#define extension_generations ".gen"                                                                //? l'extension des fichiers générés
#define extension_individuals ".ind"                                                                //? l'extension des fichiers contenant les meilleurs et pires individus
constexpr bool save             = false;                                                            //? booléen indiquant si oui ou non la sauvegarde se produit
constexpr size_t save_interval  = 10;                                                               //? nombre de générations entre 2 sauvegardes
