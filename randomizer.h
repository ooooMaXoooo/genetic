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