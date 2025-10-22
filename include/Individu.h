#pragma once

#include "Vec.h"
#include "randomizer.h"
#include "settings.h"
#include "utils.h"

#include <array>
#include <iostream>

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
        
        // Probabilité de mutation par défaut
        for (size_t i = 0; i < nbVec + 1; i++) {
            m_proba[i] = proba_to_bin(real(0.9));
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