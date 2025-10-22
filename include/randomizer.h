/**
 * @file randomizer.h
 * @brief Simple static random utilities wrapper around <random>.
 *
 * This header provides the Randomizer class: a collection of static helper
 * functions for generating random numbers and shuffling arrays. The class
 * is non-instantiable (deleted constructor) and manages a single
 * std::mt19937 PRNG and a uniform real distribution used for probability
 * sampling.
 *
 * Key characteristics:
 * - Lazy initialization: the generator and the real-number distribution are
 *   initialized on first use by calling Randomizer::Init() automatically.
 *   Initialization seeds the PRNG with std::random_device.
 * - Single global generator: a single static std::mt19937 is shared by all
 *   calls. This class is not synchronized for concurrent access; callers
 *   must ensure external synchronization when using Randomizer from
 *   multiple threads.
 * - Convenience functions:
 *     - getProb(): returns a real in [0.0, 1.0) (uses the configured real type
 *       alias from settings.h).
 *     - getInt(int min, int max): returns an integer in the inclusive range
 *       [min, max].
 *     - getDistinctIntCouple(int min, int max): returns a pair of two
 *       distinct integers sampled uniformly from [min, max]. Requires
 *       min < max (checked via assert).
 *     - shuffle(std::array<T, size>&): shuffles the elements of a std::array
 *       in-place using the internal generator.
 *
 * Notes and usage considerations:
 * - The header depends on settings.h which must define the floating-point
 *   alias `real`. The uniform real distribution for getProb() uses the
 *   range [0.0, 1.0].
 * - getInt and getDistinctIntCouple construct temporary uniform_int_distribution
 *   objects per call, so their cost is dominated by distribution construction
 *   and the underlying PRNG.
 * - getDistinctIntCouple asserts that min < max; use this function only when
 *   at least two distinct values are available in the supplied range.
 * - The class deliberately deletes the default constructor to prevent
 *   instantiation; all functionality is accessed via static methods.
 *
 * Example:
 * @code
 * // Ensure settings.h defines `real` (e.g., using real = double;)
 * double p = Randomizer::getProb();              // probability in [0,1)
 * int x = Randomizer::getInt(0, 10);             // integer in [0,10]
 * auto pair = Randomizer::getDistinctIntCouple(0, 5); // two distinct ints 0..5
 * std::array<int,4> a = {1,2,3,4};
 * Randomizer::shuffle(a);                        // shuffles a in-place
 * @endcode
 *
 * @warning This utility is not thread-safe. If you require concurrent access
 * to the PRNG, protect calls with a mutex or use thread-local engines.
 */
#pragma once

#include <algorithm>
#include <random>
#include <utility>
#include "settings.h"
#include <cassert>


class Randomizer {
private:
    static std::mt19937 generator;
    static std::uniform_real_distribution<real> distrib_proba;
    static bool initialized;

public:
    Randomizer() = delete;
   
    static void Init() {
        std::random_device rd;
        generator.seed(rd());
        distrib_proba = std::uniform_real_distribution<real>(0.0, 1.0);
        initialized = true;
    }
    
    static real getProb() {
        if (!initialized) Init();

        return distrib_proba(generator);
    }
    
    static int getInt(int min, int max) {
        if (!initialized) Init();
        
        std::uniform_int_distribution<int> d(min, max);
        return d(generator);
    }

    static std::pair<int, int> getDistinctIntCouple(int min, int max) {
        assert(min < max && "min must be less than max for getDistinctIntCouple");
        if (!initialized) Init();

        std::uniform_int_distribution<int> d(min, max);
        int x1 = d(generator);
        int x2 = 0;

        do {
            x2 = d(generator);
        } while (x2 == x1);

        return {x1, x2};
    }

    template <typename T, size_t size>
    static void shuffle(std::array<T, size>& arr) {
        if (!initialized) Init();
        std::shuffle(arr.begin(), arr.end(), generator);
    }
};