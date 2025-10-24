#pragma once

#include <array>
#include <cstdint>
#include <iostream>

#include "Vec.h"
#include "randomizer.h"
#include "settings.h"
#include "utils.h"

/**
 * @class Individu
 * @brief Represents an individual in a genetic algorithm with multiple
 * chromosomes
 *
 * @tparam nbVec Number of vectors (chromosomes) in the individual
 * @tparam dim Dimension of each vector (number of genes per chromosome)
 *
 * This class implements an individual for genetic algorithms with the following
 * features:
 * - Multiple chromosomes stored as binary integers
 * - Self-adaptive mutation probabilities
 * - Binary to real number conversion capabilities
 */
template <size_t nbVec, size_t dim>
class Individu {
public:
  /**
   * @typedef gene
   * @brief Type definition for a single gene (integer)
   */
  using gene = integer;

  /**
   * @typedef chromosome
   * @brief Type definition for a chromosome (array of genes)
   */
  using chromosome = std::array<gene, dim>;

  /**
   * @typedef genome
   * @brief Type definition for the complete genome (array of chromosomes)
   */
  using genome = std::array<chromosome, nbVec>;

private:
  genome m_infos;                          // le vecteur représentant l'individu
  std::array<integer, nbVec + 1> m_proba;  // le vecteur des probabilités de mutation de chaque chromosome :
                                           // celui des vecteurs et lui-même. Cela donne n+1 probas
public:
  // ==================================================================================================================
  // Constructeurs de la classe
  // ==================================================================================================================

  /**
   * @brief Default constructor
   *
   * Initializes the individual with:
   * - Random genes for each chromosome
   * - Default mutation probability of 0.9 for each chromosome
   */
  Individu() {
    // Initialiser chaque gène aléatoirement
    for (size_t i = 0; i < nbVec; i++) {
      for (size_t j = 0; j < dim; j++) {
        m_infos[i][j] = Randomizer::getInt(0, bin_max);
      }
    }

    // Probabilité de mutation par défaut
    for (size_t i = 0; i < nbVec + 1; i++) {
      m_proba[i] = proba_to_bin(initial_propa);
    }
  }

  // ==================================================================================================================
  // Getters / Setters
  // ==================================================================================================================

  // =========================================================
  // Vecteurs
  // =========================================================

  /**
   * @brief Gets the complete genome
   * @return Const reference to the array of chromosomes
   */
  [[nodiscard]] const genome& getGenome() const { return m_infos; }

  /**
   * @brief Gets a specific gene
   * @param chromo Index of the chromosome
   * @param gene_idx Index of the gene within the chromosome
   * @return Value of the gene
   */
  [[nodiscard]] gene getGene(size_t chromo, size_t gene_idx) const { return m_infos[chromo][gene_idx]; }

  /**
   * @brief Sets a specific gene value
   * @param indice_chromosome Index of the chromosome
   * @param indice_gene Index of the gene within the chromosome
   * @param value New value for the gene
   */
  void setGene(size_t indice_chromosome, size_t indice_gene, integer value) {
    m_infos[indice_chromosome][indice_gene] = value;
  }

  /**
   * @brief Gets a specific chromosome
   * @param index Index of the chromosome
   * @return Const reference to the chromosome
   */
  [[nodiscard]] const chromosome& getChromosome(size_t index) const { return m_infos[index]; }

  /**
   * @brief Sets an entire chromosome
   * @param indice Index of the chromosome to replace
   * @param c New chromosome
   */
  void setChromosome(size_t indice, chromosome c) { m_infos[indice] = c; }

  // =========================================================
  // Probabilités
  // =========================================================

  /**
   * @brief Gets the mutation probability for a specific chromosome
   * @param index Index of the chromosome
   * @return Mutation probability as an integer
   */
  [[nodiscard]] integer getMutationProba(size_t index) const { return m_proba[index]; }

  /**
   * @brief Sets the mutation probability for a specific chromosome
   * @param index Index of the chromosome
   * @param value New probability value
   */
  void setMutationProba(size_t index, integer value) { m_proba[index] = value; }

  // ==================================================================================================================
  // Fonctions génétiques
  // ==================================================================================================================

  /**
   * @brief Performs mutation on the individual
   *
   * Applies bit-flip mutations on:
   * - Genes in chromosomes based on their respective mutation probabilities
   * - Mutation probabilities themselves (self-adaptation)
   */
  void Mutate() {
    for (size_t i = 0; i < nbVec; i++) {
      // un while pour avoir possiblement plusieurs mutations
      while (Randomizer::getProb() <= bin_to_proba(m_proba[i])) {
        int64_t l = Randomizer::getInt(0, dim - 1);     // le locus du gène à modifier
        int64_t b = Randomizer::getInt(0, Nb_bin - 1);  // le bit à modifier
        m_infos[i][l] ^= (integer(1) << b);             // on inverse le bit au rang b du gène l du chromosome i
      }
    }

    // effectuons maintenant la mutation du chromosome des probas de mutations :
    while (Randomizer::getProb() <= bin_to_proba(m_proba[nbVec])) {
      int64_t chromosome_i = Randomizer::getInt(0, nbVec - 1);  // on détermine quelle proba doit changer
      int64_t b_position = Randomizer::getInt(0, Nb_bin - 1);   // on détermine quel bit inverser
      m_proba[chromosome_i] ^= (integer(1) << b_position);      // on inverse le bit correspondant
    }
  }

  // ==================================================================================================================
  // Fonctions de gestion des fichiers
  // ==================================================================================================================

  void save_agent(const Individu& agent, const std::string& filename);
  [[nodiscard]] Individu load_agent(const std::string& filename);

  // ==================================================================================================================
  // Operator overloading
  // ==================================================================================================================

  /**
   * @brief Output stream operator overload
   * @param os Output stream
   * @param vec Individual to output
   * @return Modified output stream
   *
   * Converts binary representation to real numbers and outputs vectors
   */
  friend std::ostream& operator<<(std::ostream& os, const Individu& vec) {
    // on doit transformer l'écriture binaire de chaque vecteur en une écriture
    // réelle

    for (size_t i = 0; i < nbVec; i++) {
      Vec<dim> c{};
      for (size_t j = 0; j < dim; j++) {
        c[j] += bin_to_real(vec.m_infos[i][j]);
      }

      // a cette ligne, on vient de finir le chromosome c
      os << c;
      if (i < nbVec - 1) {
        os << '\n';
      }
    }
    return os;
  }
};