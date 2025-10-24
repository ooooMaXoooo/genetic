/*
Auteur  : Lemoine Maxence
Date    : 18/10/2025
But : 
  Nous essayons d'implémenter un algorithme génétique. Pour ce faire, nous allons nous appuyer sur différents travaux, notamment sur 
  celui de  Vincent MAGNIN : http://magnin.plil.net/spip.php?article44

Bibliographie :
  [1] : http://magnin.plil.net/spip.php?article44
  [2] : http://souqueta.free.fr/Project/files/TE_AG.pdf
  [3] : https://fr.wikipedia.org/wiki/Algorithme_g%C3%A9n%C3%A9tique
*/



/*
• Point SVT :
  • Vocabulaire :
    • Une population est un ensemble d'individus/d'agents.
    • Un individu est caractérisé par son génome, i.e. un ensemble de chromosomes.
    • un chromosome est un ensemble de gènes.
    • un gène est une et une seule caractéristique d'un individu. 
    • Le locus est la position d'un gène dans son chromosome. (On peut parler d'un gène sans faire référence explicitement au
      chromosome dans lequel il est car un gène est toujours présent dans un unique chromosome)
    • Un allèle est une version d'un même gène. Il faut donc toujours parler de l'allèle d'un gène.
    • le génome d'un individu est l'ensemble des allèles qui le composent

  
  • Processus d'évolution d'une population :
        Les individus les plus adaptés à leur environnement sont les plus à même de se reproduire. Ainsi, au fil du temps, les individus ayant le génome le plus
      adapté à leur environnement se reproduiront mieux que les autres, ce qui augmentera la proportion des meilleurs allèles au sein de la population.
      Cependant, il faut que le génome puisse être modifié au fil des générations sans quoi il n'y aurait pas d'évolution. C'est (en partie) pour cette raison que 
      nous observons parfois des mutations aléatoires dans les allèles des enfants.

  • Reproduction entre les individus
      Un enfant issu de deux ou plusieurs parents "copie" une partie du génome de chacun de ses parents. Des mutations peuvent intervenir durant le processus.


• Algorithmique :
      Le but est de maximiser une fonction qui associe à un individu un score. Cette fonction est appelée fitness.
    Il existe différents algorithmes permettant de répondre à ce problème : la descente de gradient, les réseaux de neurones, etc.
    Parmi ces algorithmes, nous allons implémenter un algorithme dit évolutionnaire.
    Le but est simple, répliquer l'approche du vivant : seuls les individus les plus adaptés survivent dans leur environnement.
    Il conviendra alors de définir proprement ce que sont les individus et ce qu'est un environnement.
    Nous allons utiliser les définitions données plus haut dans le point SVT pour marquer le lien entre l'algorithme et le vivant. Le but étant de convertir ces
    notions non-adaptées à l'approche informatique, en quelque chose de représentable.

    Subséquemment, afin de réaliser cet algorithme génétique (AG), il nous suffit de coder ce qu'est un gène et ce que sont les chromosomes en machine. cela nous
    permet directement de savoir ce qu'est un individu et une population. Remarquons d'ailleurs que nous n'avons nul besoin de coder ce qu'est un environnement, la
    fonction fitness fera office d'évaluateur dans l'environnement, et puisque les individus sont uniquement influencés par leur environnement lorsqu'il est
    question de qui est le plus adapté, nous n'avons pas besoin de savoir ce qu'est l'environnement afin de sélectionner les 'meilleurs' individus.


    Dans notre cas, nous cherchons à maximiser une fonction ƒ qui part d'un ℝ-espace euclidien E élevé à la puissance n dans ℝ. Elle associe à chaque n-uplet de
    vecteurs un score réel. C'est la fonction fitness. Plus le fitness est élevé, plus l'individu est adapté à son environnement.

    Un individu sera codé par n+1 chromosomes :
    • Les n premiers serviront à coder chaque vecteur représentant l'individu. Ils seront composés d'autant de gènes que la dimension de E. Chaque gène codera une
    composante du vecteur dans une base de E.
    • Le dernier chromosome codera la probabilité de mutation de chaque chromosome, y compris lui-même. Il y aura donc autant de gènes que de chromosomes dans ce
    dernier chromosome

    Afin de simplifier l'algorithmique, nous préfèrerons utiliser une représentation des gènes sous forme d'une liste de chiffres en binaire (c.f bibliographie).
    Cependant, nos vecteurs vivent dans un ℝ-espace euclidien E, il faut donc un moyen de représenter nos vecteurs sous forme de nombres binaires.
    Il vient alors qu'il faut discrétiser notre espace E et lui donner des bornes afin de le rendre représentable en machine.
    Nous allons utiliser deux fonctions qui permettent de passer de la représentation binaire à la représentation réelle. Nous pourrons ainsi coder nos vecteurs 
    sous forme binaire tout en pouvant les utiliser dans la fonction fitness, au moyen d'une transformation de chaque composante binaire sous forme réelle.

    Dans l'exemple ci-dessous, nous cherchons à minimiser la fonction x², i.e. maximiser la fonction -x².
    Nous allons coder un nombre réel sur 32 bits, ce qui donne 4 294 967 296 réels possibles.
    le plus petit réel sera -1000, et le plus grand sera 1000.
    Pour plus de précision sur les paramètres de l'exemple, allez voir le fichier settings.h
*/

/*
    Pour avoir les couleurs des commentaires, il faut installer l'extension Better Comments (pour VS Code) :
    https://marketplace.visualstudio.com/items?itemName=aaron-bond.better-comments
*/


#include "genetic.h"
#include "randomizer.h"
#include <iostream>

int main(int argc, char** argv) {
    Randomizer::Init();
    genetic_algorithm();

    std::cout << "Appuyez sur une touche pour quitter ...";
    std::cin.get();
    return 0;
}