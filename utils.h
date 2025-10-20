#pragma once

#include "settings.h"
#include <random>
#include <stdint.h>
#include <algorithm>

/**
 * @brief Convertit un gène binaire (integer) en sa valeur réelle (real).
 */
inline real bin_to_real (integer b) {
    // CORRECTION : Forcer la division flottante en castant 'b' en 'real' AVANT la division.
    return min_real + (static_cast<real>(b) / bin_max) * real_size;
}

/**
 * @brief Convertit une valeur réelle (real) en son gène binaire (integer).
 * Les valeurs en dehors de [min_real, max_real] sont bridées.
 */
inline integer real_to_bin (real x) {
    // On normalise x pour le ramener dans [0, 1]
    real normalized = (x - min_real) / real_size;
    
    // On bride (clamp) la valeur pour éviter les débordements
    real clamped = std::clamp(normalized, 0.0, 1.0);
    
    return static_cast<integer>(clamped * bin_max);
}

/**
 * @brief Convertit un gène binaire (integer) en une probabilité [0.0, 1.0].
 */
inline real bin_to_proba (integer b) {
    // CORRECTION : Forcer la division flottante.
    return static_cast<real>(b) / bin_max;
}

/**
 * @brief Convertit une probabilité [0.0, 1.0] en son gène binaire (integer).
 * Les probabilités en dehors de [0.0, 1.0] sont bridées.
 */
inline integer proba_to_bin(real p) {
    // On bride (clamp) la valeur pour garantir qu'elle reste dans l'intervalle
    real clamped = std::clamp(p, 0.0, 1.0);
    
    return static_cast<integer>(clamped * bin_max);
}