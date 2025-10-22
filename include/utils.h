#pragma once
/**
 * @file utils.h
 * @brief Utility conversion functions between binary genes, real values and probabilities.
 *
 * This header provides inline helper functions to map between:
 *  - a discrete binary gene representation (type: integer, range: [0, bin_max]),
 *  - a continuous real-valued phenotype (type: real, range: [min_real, max_real]),
 *  - a probability in the interval [0.0, 1.0].
 *
 * The functions rely on configuration symbols defined in "settings.h":
 *  - integer : integral type used to encode genes
 *  - real    : floating-point type used for phenotypes/probabilities
 *  - bin_max : maximum (inclusive) value representable by a binary gene
 *  - min_real, max_real, real_size : bounds and span of the real interval
 *
 * All functions are inline, have constant time complexity O(1), do not allocate memory,
 * and do not throw exceptions. Correct behavior assumes the settings constants are
 * properly defined and that integer and real are compatible with the operations below.
 */


#include "settings.h"
#include <random>
#include <stdint.h>
#include <algorithm>



/**
 * @brief Convert a binary gene to its corresponding real value.
 *
 * Maps the integer gene b in the discrete domain [0, bin_max] to the continuous
 * interval [min_real, max_real] using a linear mapping:
 *   result = min_real + (b / bin_max) * real_size
 *
 * @param b Binary gene value (expected range: 0 .. bin_max).
 * @return Mapped real value in the interval [min_real, max_real] when b ∈ [0, bin_max].
 *
 * @note
 *  - The division is performed in floating point to preserve fractional values.
 *  - If b is outside the expected range [0, bin_max], the result may lie outside
 *    [min_real, max_real]; callers should ensure b is valid if strict bounding is required.
 */
inline real bin_to_real (integer b) noexcept {
    // CORRECTION : Forcer la division flottante en castant 'b' en 'real' AVANT la division.
    return min_real + (static_cast<real>(b) / bin_max) * real_size;
}

/**
 * @brief Convert a real value to its corresponding binary gene.
 *
 * Normalizes the real value x into the unit interval [0, 1], clamps it to that range,
 * then scales to the discrete domain and casts to integer:
 *   normalized = (x - min_real) / real_size
 *   clamped    = clamp(normalized, 0.0, 1.0)
 *   result     = static_cast<integer>(clamped * bin_max)
 *
 * @param x Real-valued phenotype to encode.
 * @return Integer gene in the range [0, bin_max].
 *
 * @note
 *  - Values of x outside [min_real, max_real] are clipped to the nearest bound before encoding.
 *  - The final conversion uses a truncating cast (static_cast<integer>), which discards
 *    fractional parts. If symmetric rounding is desired, callers should change the cast to
 *    an appropriate rounding function.
 */
inline integer real_to_bin (real x) noexcept {
    // On normalise x pour le ramener dans [0, 1]
    real normalized = (x - min_real) / real_size;
    
    // On bride (clamp) la valeur pour éviter les débordements
    real clamped = std::clamp(normalized, 0.0, 1.0);
    
    return static_cast<integer>(clamped * bin_max);
}


/**
 * @brief Convert a binary gene to a probability in [0.0, 1.0].
 *
 * Performs a linear mapping from the integer domain to the unit interval:
 *   result = b / bin_max
 *
 * @param b Binary gene value (expected range: 0 .. bin_max).
 * @return Probability as a real in [0.0, 1.0] when b ∈ [0, bin_max].
 *
 * @note
 *  - Division is performed in floating point.
 *  - If b is outside [0, bin_max], the returned value may be outside [0.0, 1.0].
 */
inline real bin_to_proba (integer b) noexcept {
    // CORRECTION : Forcer la division flottante.
    return static_cast<real>(b) / bin_max;
}

/**
 * @brief Convert a probability to its binary gene representation.
 *
 * Clamps the input probability p to [0.0, 1.0], scales by bin_max and casts to integer:
 *   clamped = clamp(p, 0.0, 1.0)
 *   result  = static_cast<integer>(clamped * bin_max)
 *
 * @param p Probability value (may be outside [0.0, 1.0]; it will be clamped).
 * @return Integer gene in the range [0, bin_max].
 *
 * @note
 *  - If p is exactly 1.0 after clamping, the result will be bin_max.
 *  - The final cast truncates toward zero; fractional probabilities are not rounded.
 */
inline integer proba_to_bin(real p) noexcept {
    // On bride (clamp) la valeur pour garantir qu'elle reste dans l'intervalle
    real clamped = std::clamp(p, 0.0, 1.0);
    
    return static_cast<integer>(clamped * bin_max);
}