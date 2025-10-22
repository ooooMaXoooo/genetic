#include <fstream>
#include <array>
#include <utility>
#include <cstdint>
#include <filesystem>
#include <string>


#include "Vec.h"
#include "settings.h"
#include "utils.h"
#include "randomizer.h"
#include "Individu.h"


#include "genetic.h"




namespace fs = std::filesystem; // alias pour avoir à moins écrire












/* 
    Cette fonction fitness retourne la somme des coordonnées des vecteurs, le tout au carré
*   dans notre cas, c'est la fonction x ⟼ x²
!   DO NOT CHANGE THE DEFINITION OF THE FUNCTION, CHANGE THE CODE INSIDE !
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













real eval_agent (const Agent& a) {
    // on doit transformer l'écriture binaire de chaque vecteur en une écriture réelle
    std::array<Vec<Dimension>, NumberOfVectors> vecteurs; // initialisé à 0

    for (size_t i=0; i<NumberOfVectors; i++) {
        auto vec = a.getGenome()[i];
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
static void bit_level_crossover(
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