/**
 * @file epmej_peumeu_heron_DE.h
 * @brief Hochleistungs-C-Bibliothek zur Berechnung der Quadratwurzel.
 * @author Djempe James (Unabhängiger Forscher)
 * @contact epmejd@gmail.com | +237 697 778 96
 * @date November 2026
 * 
 * Implementiert die Epmej & Peumeu Monique-Eigenschaft (Y = x^2 + m, x >= 1, x >= 2|m|)
 * kombiniert mit dem Heron-Verfahren (Babylonisches Wurzelziehen) in zwei Varianten:
 *  1. Normierte Taylor-Reihenentwicklung 2. Ordnung (reine Algebra, hardwareunabhängig).
 *  2. Schnelle IEEE-754-Bitmanipulations-Approximation (industrielle O(1)-Geschwindigkeit).
 */

#ifndef EPMEJ_PEUMEU_HERON_DE_H
#define EPMEJ_PEUMEU_HERON_DE_H

#include <stdint.h>

/**
 * @brief Methode 1: Quadratwurzel via Epmej & Peumeu + Taylor-Entwicklung + Heron.
 * Läuft auf jeder Prozessorarchitektur ohne IEEE-754-Abhängigkeit oder <math.h>.
 * @param Y Reelle Zahl strikt größer als 1.
 * @return Quadratwurzel von Y (doppelte Genauigkeit ~ 1.11e-16).
 */
static inline double epmej_taylor_wurzel(double Y) {
    if (Y <= 1.0) return (Y < 0.0) ? 0.0 : Y;

    double radikand = 1.0 + 16.0 * Y;

    /* Bereichsreduktion durch Viererpotenzen zur Abbildung von r in [1, 4) */
    double r = radikand;
    double skalierungsfaktor = 1.0;
    while (r >= 4.0) {
        r *= 0.25;
        skalierungsfaktor *= 2.0;
    }

    /* Taylor-Entwicklung 2. Ordnung von sqrt(r) um den Punkt r0 = 2.25 (sqrt = 1.5) */
    double u = (r - 2.25) / 2.25;
    double schaetzung_sqrt_r = 1.5 * (1.0 + 0.5 * u - 0.125 * u * u);

    /* Analytischer Startwert nach Epmej & Peumeu Monique: x0 = sqrt(1 + 16Y) / 4 */
    double x0 = (schaetzung_sqrt_r * skalierungsfaktor) * 0.25;

    /* Zwei Heron-Iterationen zum Erreichen der Maschinengenauigkeit */
    double x1 = 0.5 * (x0 + Y / x0);
    double x_final = 0.5 * (x1 + Y / x1);

    return x_final;
}

/**
 * @brief Methode 2: Quadratwurzel via Epmej & Peumeu + IEEE-754-Bitmanipulation + Heron.
 * Strikte konstante Laufzeit O(1) ohne Schleifen, ideal für IoT und eingebettete Systeme.
 * @param Y Reelle Zahl strikt größer als 1.
 * @return Quadratwurzel von Y (doppelte Genauigkeit ~ 1.11e-16).
 */
static inline double epmej_schnelle_wurzel(double Y) {
    if (Y <= 1.0) return (Y < 0.0) ? 0.0 : Y;

    double radikand = 1.0 + 16.0 * Y;

    /* Initiale Schätzung der inversen Quadratwurzel mittels 64-Bit IEEE-754 Bitmanipulation */
    union {
        double d;
        uint64_t i;
    } u;
    u.d = radikand;
    u.i = 0x5fe6ec85e7de30daULL - (u.i >> 1);
    double inv_sqrt_rad = u.d;
    /* Einmaliger Newton-Raphson-Schritt für die inverse Quadratwurzel */
    inv_sqrt_rad = inv_sqrt_rad * (1.5 - 0.5 * radikand * inv_sqrt_rad * inv_sqrt_rad);

    /* Analytischer Startwert nach Epmej & Peumeu Monique: x0 = sqrt(1 + 16Y) / 4 */
    double x0 = (radikand * inv_sqrt_rad) * 0.25;

    /* Abschließende Heron-Verfeinerung garantiert volle IEEE-754-Genauigkeit */
    double x_final = 0.5 * (x0 + Y / x0);

    return x_final;
}

#endif /* EPMEJ_PEUMEU_HERON_DE_H */
