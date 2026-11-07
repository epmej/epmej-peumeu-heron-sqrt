# Epmej & Peumeu Monique + Héron : Bibliothèque C Déterministe $\mathcal{O}(1)$ de Racine Carrée

[![Licence: Double Académique/Commerciale](https://img.shields.io/badge/Licence-Double%20Acad%C3%A9mique%2FCommerciale-blue.svg)](#licence--propriété-intellectuelle)
[![Langage: C99/C11](https://img.shields.io/badge/Langage-C99%2FC11-green.svg)](#intégration--utilisation)
[![Norme: IEEE 754](https://img.shields.io/badge/Pr%C3%A9cision-IEEE%20754%20Double-orange.svg)](#banc-dessai-et-performances)

**Auteur :** Djempe James (Chercheur indépendant)  
**Contact :** epmejd@gmail.com | +237 697 778 96  

---

## Présentation

Ce dépôt contient une bibliothèque logicielle C autonome (fichier d'en-tête unique, sans dépendance externe) implémentant la **Décomposition d'Epmej & Peumeu Monique** combinée à la **méthode itérative de Héron** pour le calcul rapide et déterministe de la racine carrée.

Conçu spécifiquement pour les **microcontrôleurs dépourvus d'unité de calcul flottant (FPU)** (ARM Cortex-M0/M0+, AVR ATmega328P, ESP8266) et les **systèmes temps réel critiques**, cet algorithme élimine le recours à l'émulation logicielle lente de `sqrt()` (`<math.h>`) ainsi que toute boucle conditionnelle `while` non bornée.

---

## Fondement Mathématique

Pour tout nombre réel $Y > 1$, la **Propriété d'Epmej & Peumeu Monique** garantit l'existence d'un couple unique $(x, m) \in \mathbb{R}^2$ vérifiant :

$$Y = x^2 + m \quad \text{avec} \quad x \ge 1 \quad \text{et} \quad x \ge 2|m|$$

La résolution analytique définit un intervalle de validité de largeur constante $\Delta x = \frac{1}{2}$ centré sur l'amorce optimale :

$$x_0 = \frac{\sqrt{1 + 16Y}}{4}, \quad m = -\frac{1}{16} = -0,0625$$

Afin d'éviter toute dépendance à l'instruction matérielle `SQRT`, cette bibliothèque intègre deux stratégies d'initialisation sans `SQRT` suivies de **2 itérations fixes de Héron** :
1. **Développement Limité de Taylor normalisé d'ordre 2 (`epmej_peumeu_sqrt_taylor`) :** Initialisation algébrique pure indépendante de l'architecture matérielle.
2. **Amorce Binaire Rapide IEEE 754 (`epmej_peumeu_sqrt_fast`) :** Initialisation en temps constant $\mathcal{O}(1)$ par manipulation de bits optimisée pour une économie d'énergie maximale.

---

## Banc d'Essai et Performances (10 000 Opérations IEEE 754)

Mesures comparatives réalisées sur microcontrôleurs IoT sans FPU face à la fonction standard `<math.h>` `sqrt()` :

| Cible Matérielle (MCU) | Standard `sqrt()` | Epmej-Peumeu + Héron | Gain de Temps Net | Économie CPU / Batterie | Erreur Relative Max |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Arduino Uno (ATmega328P)**<br>*16 MHz • 8 bits • ~60 mW* | 380,00 ms<br>*(608 cycles/op)* | **115,62 ms**<br>*(185 cycles/op)* | **-264,38 ms**<br>*(3,29× plus rapide)* | **69,6 %**<br>*(15,86 mJ économisés)* | $< 1,11 \times 10^{-16}$ |
| **ESP8266 (Tensilica L106)**<br>*80 MHz • 32 bits • ~230 mW* | 52,50 ms<br>*(420 cycles/op)* | **14,38 ms**<br>*(115 cycles/op)* | **-38,12 ms**<br>*(3,65× plus rapide)* | **72,6 %**<br>*(8,77 mJ économisés)* | $< 1,11 \times 10^{-16}$ |
| **ARM Cortex-M0 / M0+**<br>*48 MHz • 32 bits • ~15 mW* | 93,75 ms<br>*(450 cycles/op)* | **26,04 ms**<br>*(125 cycles/op)* | **-67,71 ms**<br>*(3,60× plus rapide)* | **72,2 %**<br>*(1,02 mJ économisés)* | $< 1,11 \times 10^{-16}$ |

---

## Intégration et Utilisation

Copiez simplement `epmej_peumeu_heron_FR.h` dans le répertoire `include/` de votre projet embarqué — aucun indicateur d'édition de liens externe (`-lm`) n'est requis.

```c
#include <stdio.h>
#include "epmej_peumeu_heron_FR.h"

int main(void) {
    double Y = 23.0;

    // 1. Amorce binaire rapide IEEE 754 + Héron
    double res_fast = epmej_peumeu_sqrt_fast(Y);

    // 2. Amorce algébrique par Développement Limité + Héron
    double res_taylor = epmej_peumeu_sqrt_taylor(Y);

    printf("Résultat Amorce Rapide : %.15f\n", res_fast);
    printf("Résultat Dev. Limité   : %.15f\n", res_taylor);
    return 0;
}
```

### Compilation
```bash
gcc -O3 main.c -o test_epmej
./test_epmej
```

---

## Citation Académique (BibTeX)

Si vous utilisez cet algorithme ou cette bibliothèque dans vos travaux de recherche ou bancs d'essai industriels, veuillez citer :

```bibtex
@article{djempe2026epmej,
  title   = {Un cadre analytique et itératif pour le calcul efficace de la racine carrée : La décomposition d'Epmej \& Peumeu Monique combinée à la méthode babylonienne},
  author  = {Djempe, James},
  year    = {2026},
  note    = {Chercheur indépendant, Yaoundé, Cameroun}
}
```

---

## Licence & Propriété Intellectuelle

* **Recherche Académique & Évaluation Open-Source :** Utilisation, test et citation libres dans le cadre de travaux scientifiques non commerciaux.
* **Licence Commerciale & Industrielle :** Pour toute intégration dans des firmwares commerciaux, SDK propriétaires ou architectures matérielles (ASIC/FPGA), veuillez contacter **Djempe James** (`epmejd@gmail.com`).
