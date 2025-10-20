/*
Auteur  : ...
Date    : 18/10/2025
But : 
  Nous essayons d'implémenter un algorithme génétique. Pour ce faire, nous allons nous appuyer sur différents travaux, notamment sur 
  celui de ... : http://magnin.plil.net/spip.php?article44

Bibliographie :
  [1] : http://magnin.plil.net/spip.php?article44
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


#include <random>
#include <iostream>
#include <fstream>
#include <array>
#include <utility>
#include <stdint.h>
#include <filesystem>
#include <string>
#include "Vec.h"
#include "settings.h"
#include "utils.h"
#include "randomizer.h"

namespace fs = std::filesystem;

/* 
    Cette fonction fitness retourne la somme des coordonnées des vecteurs, le tout au carré
*   dans notre cas, c'est la fonction x ⟼ x²
*/
inline float fitness (const std::array<Vec<Dimension>, NumberOfVectors>& vecteurs) {                //? la fonction fitness
    real sum = 0;   
    for(size_t i=0; i<NumberOfVectors; i++) {
        size_t SIZE = vecteurs[i].size();
        for(size_t j=0; j<SIZE; j++) {
            sum += vecteurs[i][j];
        }
    }

    return -sum*sum;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// Structures ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



template <size_t nbVec, size_t dim>
class Individu {
public :
    using gene = integer;
    using chromosome = std::array<gene, dim>;
    using genome = std::array<chromosome, nbVec>;

private :
    genome m_infos; // le vecteur représentant l'individu
    std::array<integer, nbVec + 1> m_proba; // le vecteur des probabilités de mutation de chaque chromosome : celui des vecteurs et lui-même. Cela donne n+1 probas
public :

    Individu() {
        // Initialiser chaque gène aléatoirement
        for (size_t i = 0; i < nbVec; i++) {
            for (size_t j = 0; j < dim; j++) {
                m_infos[i][j] = Randomizer::getInt(0, bin_max);
            }
        }
        
        // Probabilité de mutation par défaut (1 en binaire = valeur faible en réel)
        for (size_t i = 0; i < nbVec + 1; i++) {
            m_proba[i] = proba_to_bin(real(0.9));
        }
    }

    /*
    TODO : finir cette fonction pour pouvoir reprendre l'entraînement à partir d'une sauvegarde
    */
    Individu(std::array<Vec<dim>, nbVec> vectors, std::array<float, nbVec> probas){
        // on doit convertir chaque vecteur en écriture binaire

        for(size_t i=0; i<nbVec; i++) {
            // on gère le chromosome i

            // on gère le gène de locus i dans le chromosome des probas
            
        }

    }

    void Mutate() {
        for (size_t i=0; i<nbVec; i++) {
            while (Randomizer::getProb() <= bin_to_proba(m_proba[i])) { // un while pour avoir possiblement plusieurs mutations
                // on sait qu'il doit y avoir une mutation dans le chromosome i :
                // nous devons maintenant déterminer le gène de locus l qui subit une mutation
                int l = Randomizer::getInt(0, dim-1);

                //nous avons le chromosome et la gène, déterminons maintenant le bit à modifer :
                int b = Randomizer::getInt(0, Nb_bin-1);

                /*// on inverse le bit au rang b du gène l du chromosome i:
                integer mask = 1 << b;
                integer val = m_infos[i][l];

                integer bit_rang_b = (val & mask) >>  b;
                integer reste = (val & (~mask));

                m_infos[i][l] = reste + ((bit_rang_b == 0 ? 1 : 0) << b);*/

                // Version simplifiée avec XOR
                m_infos[i][l] ^= (integer(1) << b);
            }
        }

        // effectuons maintenant la mutation du chromosome des probas de mutations : 
        while (Randomizer::getProb() <= bin_to_proba(m_proba[nbVec])) {
            int chromosome_i = Randomizer::getInt(0, nbVec - 1);  // on détermine quelle proba doit changer
            int b_position   = Randomizer::getInt(0, Nb_bin - 1); // on détermine quel bit inverser
            m_proba[chromosome_i] ^= (integer(1) << b_position);  // on inverse le bit correspondant
        }
    }

    const std::array<std::array<integer, dim>, nbVec>& getVectors () const {
        return m_infos;
    }

    void setGene (size_t indice_chromosome, size_t indice_gene, integer value) {
        m_infos[indice_chromosome][indice_gene] = value;
    }

    void setChromosome (size_t indice, chromosome c) {
        m_infos[indice] = c;
    }

    // Getters/setters pour les probas
    integer getMutationProba(size_t index) const {
        return m_proba[index];
    }

    void setMutationProba(size_t index, integer value) {
        m_proba[index] = value;
    }

    // Getter pour accéder au chromosome complet
    const chromosome& getChromosome(size_t index) const {
        return m_infos[index];
    }

    // Accesseur pour un gène spécifique
    gene getGene(size_t chromo, size_t gene_idx) const {
        return m_infos[chromo][gene_idx];
    }


    // Affichage
    friend std::ostream& operator<<(std::ostream& os, const Individu& vec) {
        // on doit transformer l'écriture binaire de chaque vecteur en une écriture réelle

        for (size_t i=0; i < nbVec ; i++) {
            Vec<dim> c {};
            for (size_t j=0; j < dim; j++) {
                c[j] += bin_to_real(vec.m_infos[i][j]);// * Vec<dim>::unit(j);
            }

            // a cette ligne, on vient de finir le chromosome c
            os << c;
            if (i < nbVec -1) os << '\n';
        }
        return os;
    }


    // ! NE FONCTIONNE ABSOLUMENT PAS
    /*
    // Sauvegarder un agent
    void save_agent(const Agent& agent, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        
        // Sauvegarder les chromosomes
        for (size_t i = 0; i < NumberOfVectors; i++) {
            for (size_t j = 0; j < Dimension; j++) {
                integer gene = agent.getGene(i, j);
                file.write(reinterpret_cast<const char*>(&gene), sizeof(integer));
            }
        }
        
        // Sauvegarder les probas de mutation
        for (size_t i = 0; i <= NumberOfVectors; i++) {
            integer proba = agent.getMutationProba(i);
            file.write(reinterpret_cast<const char*>(&proba), sizeof(integer));
        }
    }

    // Charger un agent
    Agent load_agent(const std::string& filename) {
        Agent agent;
        std::ifstream file(filename, std::ios::binary);
        
        // Charger les chromosomes
        for (size_t i = 0; i < NumberOfVectors; i++) {
            for (size_t j = 0; j < Dimension; j++) {
                integer gene;
                file.read(reinterpret_cast<char*>(&gene), sizeof(integer));
                agent.setGene(i, j, gene);
            }
        }
        
        // Charger les probas de mutation
        for (size_t i = 0; i <= NumberOfVectors; i++) {
            integer proba;
            file.read(reinterpret_cast<char*>(&proba), sizeof(integer));
            agent.setMutationProba(i, proba);
        }
        
        return agent;
    }
    */

};


using Agent = Individu<NumberOfVectors, Dimension>;
using Population = std::array<Agent, PopulationSize>;
using HalfPopulation = std::array<Agent, HalfPopulationSize>;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// Protoypes ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

real eval_agent (const Agent& a);

HalfPopulation selection_tournoi(const Population& p);
std::pair<Agent, Agent> cross_over (const Agent& p1, const Agent& p2);
Population cross_over_half_pop (HalfPopulation& hp);
void mutations (Population* p);
void create_generation (size_t indice, Population* p, bool is_saving_in_file = false);


void print_best_agent(const Population& p) {
    size_t best_indice = 0;
    real best_eval = eval_agent(p[best_indice]);
    for(size_t i = 1; i < PopulationSize; i++) {
        real eval = eval_agent(p[i]);
        if (eval > best_eval) {
            best_eval = eval;
            best_indice = i;
        }
    }

    std::cout << "best :\n" << p[best_indice];
    std::cout << "\nfitness : " << eval_agent(p[best_indice]);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// Fonctions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

real eval_agent (const Agent& a) {
    // on doit transformer l'écriture binaire de chaque vecteur en une écriture réelle
    std::array<Vec<Dimension>, NumberOfVectors> vecteurs; // initialisé à 0

    for (size_t i=0; i<NumberOfVectors; i++) {
        auto vec = a.getVectors()[i];
        for (size_t j=0; j<Dimension; j++) {
            vecteurs[i] += bin_to_real(vec[j]) * Vec<Dimension>::unit(j);
        }
    }

    // normalement à cette ligne, la variable vecteur représente correctement la même information déjà stockée dans l'agent a
    return fitness(vecteurs);
}

HalfPopulation selection_tournoi(const Population& p) {
    HalfPopulation hp {}; // initialisé à 0

    bool best_has_been_selectionned = false; // on laisse comme ça pour le moment, on essai d'éviter la convergence prématurée (c.f [1])
    for (size_t i=0; i<HalfPopulationSize; i++) {
        // on prend 2 indices :
        int iA = Randomizer::getInt(0, PopulationSize - 1);
        int iB = Randomizer::getInt(0, PopulationSize - 1);

        // on prend le meilleur des 2 agents
        float evalA = eval_agent(p[iA]);
        float evalB = eval_agent(p[iB]);

        if(evalA > evalB)   hp[i] = p[iA];
        else                hp[i] = p[iB];
    }

    return hp;

    // ===== Dans le cas où on souhaite garder le meilleur agent =====//

    if(!best_has_been_selectionned) {
        // on veut au moins garder le meilleur.
        // On choisi quelqu'un a remplacer au hasard
        //hp[Randomizer::getInt(0, HalfPopulationSize - 1)] = p[best_indice];
    }
}

// Fonction helper générique pour crossover au niveau des bits
template<typename GetterFunc, typename SetterFunc>
void bit_level_crossover(
    const Agent& p1, const Agent& p2,
    Agent& child1, Agent& child2,
    size_t num_elements, size_t element_size_bits,
    GetterFunc getter, SetterFunc setter)
{
    size_t indice_decoupe = Randomizer::getInt(0, num_elements * element_size_bits - 1);
    size_t k = indice_decoupe / element_size_bits;
    size_t k_prime = indice_decoupe % element_size_bits;
    
    // Recopier les éléments avant le k-ième
    for (size_t i = 0; i < k; i++) {
        setter(child1, i, getter(p1, i));
        setter(child2, i, getter(p2, i));
    }
    
    // Traiter l'élément k (celui qui est coupé)
    if (k < num_elements) {
        integer e1 = getter(p1, k);
        integer e2 = getter(p2, k);
        
        integer mask1 = (integer(1) << k_prime) - 1;  // Bits [0, k_prime-1]
        integer mask2 = ~mask1;                        // Bits [k_prime, Nb_bin-1]
        
        setter(child1, k, (e1 & mask1) | (e2 & mask2));
        setter(child2, k, (e2 & mask1) | (e1 & mask2));
    }
    
    // Recopier les éléments après le k-ième (de l'autre parent)
    for (size_t i = k + 1; i < num_elements; i++) {
        setter(child1, i, getter(p2, i));
        setter(child2, i, getter(p1, i));
    }
}

// Fonction cross_over simplifiée
std::pair<Agent, Agent> cross_over(const Agent& p1, const Agent& p2) {
    Agent child1{};
    Agent child2{};
    
    // ========== CROSSOVER DES CHROMOSOMES DE DONNÉES ==========
    for (size_t chromo = 0; chromo < NumberOfVectors; chromo++) {
        // Lambdas pour accéder aux gènes du chromosome 'chromo'
        auto getter = [chromo](const Agent& agent, size_t gene_idx) -> integer {
            return agent.getGene(chromo, gene_idx);
        };
        
        auto setter = [chromo](Agent& agent, size_t gene_idx, integer value) {
            agent.setGene(chromo, gene_idx, value);
        };
        
        bit_level_crossover(p1, p2, child1, child2, 
                           Dimension, Nb_bin, 
                           getter, setter);
    }
    
    // ========== CROSSOVER DU CHROMOSOME DES PROBAS ==========
    auto getter_proba = [](const Agent& agent, size_t idx) -> integer {
        return agent.getMutationProba(idx);
    };
    
    auto setter_proba = [](Agent& agent, size_t idx, integer value) {
        agent.setMutationProba(idx, value);
    };
    
    bit_level_crossover(p1, p2, child1, child2,
                       NumberOfVectors + 1, Nb_bin,
                       getter_proba, setter_proba);
    
    return std::pair<Agent, Agent>(child1, child2);
}

Population cross_over_half_pop (HalfPopulation& hp) {
    Population res {};

    // on veut générer une liste de couples aléatoirement :
    // on mélange la population, puis on prend tout les i et i+1
    Randomizer::shuffle(hp);

    size_t cur = 0;
    for (size_t i=0; i<HalfPopulationSize; i+=2) {
        // on sélectionne les deux parents i et i+1 :
        res[cur] = hp[i];
        cur++;

        res[cur] = hp[i+1];
        cur++;

        // on ajoute les enfants
        auto [child1, child2] = cross_over(hp[i], hp[i+1]);
        res[cur] = child1;
        cur++;

        res[cur] = child2;
        cur++;
    }

    return res;
}


// on modifie directement la population --> pointeur
void mutations (Population* p) {
    for (auto& agent : *p) {
        agent.Mutate();
    }
}


// on modifie directement la population --> pointeur
void create_generation (size_t indice, Population* p, bool is_saving_in_file) {
    HalfPopulation hp = selection_tournoi(*p);
    *p = cross_over_half_pop(hp);

    //std::cout << "je suis venu ici\n";
    mutations(p);

    if (is_saving_in_file) {
        fs::path gen = ".";
        gen /= directory;
        gen /= std::string("generation_") + std::to_string(indice);
        gen /= std::string("population") + std::string(extension_generations);
        // Crée uniquement les dossiers parents
        if (fs::create_directories(gen.parent_path())) {
            std::cout << "Structure de dossiers '" << gen.parent_path() << "' créée." << std::endl;
        }

        std::ofstream file (gen);
        if (!file.is_open()) {
            throw "cannot open file!\n";
        }

        // on sauvegarde chaque individus sur une ligne
        // on pense à ajouter une ligne de paramètresau début du fichier
    }
}




void print_best_agent(const Population& p);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// Genetic Algoritm ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void genetic_algorithm () {
    Population generation {};
    for (size_t step = 1; step<maxGen + 1; step++) {
        create_generation(step, &generation, false);
        std::cout << "step : " << step  << "/" << maxGen << '\n';
        print_best_agent(generation);

        std::cout << "\n\n<><><><><><><><><><><><><><><><><><><><><><><><><><>\n\n";
    }

    // on veut afficher le meilleur élément de cette génération
    print_best_agent(generation); std::cout << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// Entry Point ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv) {
    Randomizer::Init();
    genetic_algorithm();

    std::cout << "Appuyez sur une touche pour quitter ...";
    std::cin.get();
    return 0;
}