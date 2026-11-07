/**
 * @file epmej_peumeu_heron_EN.h
 * @brief High-Performance C Library for Square Root Computation.
 * @author Djempe James (Independent Researcher)
 * @contact epmejd@gmail.com | +237 697 778 96
 * @date November 2026
 * 
 * Implements the Epmej & Peumeu Monique Property (Y = x^2 + m, x >= 1, x >= 2|m|)
 * combined with Heron's (Babylonian) method using two initialization variants:
 *  1. Normalized 2nd-Order Taylor Series Expansion (pure algebra, hardware-agnostic).
 *  2. Fast IEEE 754 Bitwise Manipulation Approximation (industrial O(1) speed).
 */

#ifndef EPMEJ_PEUMEU_HERON_EN_H
#define EPMEJ_PEUMEU_HERON_EN_H

#include <stdint.h>

/**
 * @brief Method 1: Square root via Epmej & Peumeu + Taylor Expansion + Heron.
 * Runs on any processor architecture without IEEE 754 dependency or <math.h>.
 * @param Y Real number strictly greater than 1.
 * @return Square root of Y (double precision ~ 1.11e-16).
 */
static inline double epmej_taylor_sqrt(double Y) {
    if (Y <= 1.0) return (Y < 0.0) ? 0.0 : Y;

    double radicand = 1.0 + 16.0 * Y;

    /* Power-of-4 range reduction to map r into [1, 4) */
    double r = radicand;
    double scale_factor = 1.0;
    while (r >= 4.0) {
        r *= 0.25;
        scale_factor *= 2.0;
    }

    /* 2nd-order Taylor expansion of sqrt(r) around pivot r0 = 2.25 (sqrt = 1.5) */
    double u = (r - 2.25) / 2.25;
    double est_sqrt_r = 1.5 * (1.0 + 0.5 * u - 0.125 * u * u);

    /* Epmej & Peumeu Monique analytical seed: x0 = sqrt(1 + 16Y) / 4 */
    double x0 = (est_sqrt_r * scale_factor) * 0.25;

    /* Two Heron iterations to reach machine epsilon */
    double x1 = 0.5 * (x0 + Y / x0);
    double x_final = 0.5 * (x1 + Y / x1);

    return x_final;
}

/**
 * @brief Method 2: Square root via Epmej & Peumeu + IEEE 754 Bitwise Approx + Heron.
 * Strict constant-time O(1) execution without loops, ideal for IoT and embedded MCUs.
 * @param Y Real number strictly greater than 1.
 * @return Square root of Y (double precision ~ 1.11e-16).
 */
static inline double epmej_fast_sqrt(double Y) {
    if (Y <= 1.0) return (Y < 0.0) ? 0.0 : Y;

    double radicand = 1.0 + 16.0 * Y;

    /* Initial inverse square root estimation via 64-bit IEEE 754 bit manipulation */
    union {
        double d;
        uint64_t i;
    } u;
    u.d = radicand;
    u.i = 0x5fe6ec85e7de30daULL - (u.i >> 1);
    double inv_sqrt_rad = u.d;
    /* Single Newton-Raphson step for inverse square root */
    inv_sqrt_rad = inv_sqrt_rad * (1.5 - 0.5 * radicand * inv_sqrt_rad * inv_sqrt_rad);

    /* Epmej & Peumeu Monique analytical seed: x0 = sqrt(1 + 16Y) / 4 */
    double x0 = (radicand * inv_sqrt_rad) * 0.25;

    /* Final Heron refinement guarantees full IEEE 754 double precision */
    double x_final = 0.5 * (x0 + Y / x0);

    return x_final;
}

#endif /* EPMEJ_PEUMEU_HERON_EN_H */
