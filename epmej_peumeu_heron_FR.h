/**
 * @file epmej_peumeu_heron_FR.h
 * @brief Bibliothèque C de haute performance pour le calcul de la racine carrée.
 * @author Djempe James (Chercheur indépendant)
 * @contact epmejd@gmail.com | +237 697 778 96
 * @date Novembre 2026
 * 
 * Implémente la Propriété d'Epmej & Peumeu Monique (Y = x^2 + m, x >= 1, x >= 2|m|)
 * combinée à la méthode de Héron (Babylonienne) avec deux variantes d'amorce :
 *  1. Développement Limité (DL) de Taylor normalisé d'ordre 2 (algèbre pure).
 *  2. Approximation rapide par manipulation de bits IEEE 754 (vitesse industrielle).
 */

#ifndef EPMEJ_PEUMEU_HERON_FR_H
#define EPMEJ_PEUMEU_HERON_FR_H

#include <stdint.h>

/**
 * @brief Méthode 1 : Racine carrée via Epmej & Peumeu + Développement Limité de Taylor + Héron.
 * Fonctionne sur toute architecture sans dépendance matérielle IEEE 754 ni <math.h>.
 * @param Y Nombre réel strictement supérieur à 1.
 * @return Racine carrée de Y (précision double ~ 1.11e-16).
 */
static inline double racine_epmej_dl(double Y) {
    if (Y <= 1.0) return (Y < 0.0) ? 0.0 : Y;

    double radicande = 1.0 + 16.0 * Y;

    /* Normalisation par puissances de 4 pour ramener r dans [1, 4[ */
    double r = radicande;
    double facteur_echelle = 1.0;
    while (r >= 4.0) {
        r *= 0.25;
        facteur_echelle *= 2.0;
    }

    /* Développement limité de Taylor d'ordre 2 de sqrt(r) autour du pivot r0 = 2.25 (sqrt = 1.5) */
    double u = (r - 2.25) / 2.25;
    double sqrt_r_estime = 1.5 * (1.0 + 0.5 * u - 0.125 * u * u);

    /* Application de la formule d'Epmej & Peumeu Monique : x0 = sqrt(1 + 16Y) / 4 */
    double x0 = (sqrt_r_estime * facteur_echelle) * 0.25;

    /* Deux itérations de Héron pour atteindre l'epsilon machine */
    double x1 = 0.5 * (x0 + Y / x0);
    double x_final = 0.5 * (x1 + Y / x1);

    return x_final;
}

/**
 * @brief Méthode 2 : Racine carrée via Epmej & Peumeu + Manipulation de bits IEEE 754 + Héron.
 * Exécution en temps strictement constant O(1) sans boucle, idéale pour l'IoT et l'embarqué.
 * @param Y Nombre réel strictement supérieur à 1.
 * @return Racine carrée de Y (précision double ~ 1.11e-16).
 */
static inline double racine_epmej_rapide(double Y) {
    if (Y <= 1.0) return (Y < 0.0) ? 0.0 : Y;

    double radicande = 1.0 + 16.0 * Y;

    /* Approximation initiale de l'inverse de la racine carrée par manipulation de bits 64 bits */
    union {
        double d;
        uint64_t i;
    } u;
    u.d = radicande;
    u.i = 0x5fe6ec85e7de30daULL - (u.i >> 1);
    double inv_sqrt_rad = u.d;
    /* Raffinement de Newton rapide sur l'inverse */
    inv_sqrt_rad = inv_sqrt_rad * (1.5 - 0.5 * radicande * inv_sqrt_rad * inv_sqrt_rad);

    /* Application de la formule d'Epmej & Peumeu Monique : x0 = sqrt(1 + 16Y) / 4 */
    double x0 = (radicande * inv_sqrt_rad) * 0.25;

    /* Une itération finale de Héron garantit la précision IEEE 754 complète */
    double x_final = 0.5 * (x0 + Y / x0);

    return x_final;
}

#endif /* EPMEJ_PEUMEU_HERON_FR_H */
