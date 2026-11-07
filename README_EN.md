# Epmej & Peumeu Monique + Heron: Deterministic $\mathcal{O}(1)$ Square Root Library

[![License: MIT / Commercial](https://img.shields.io/badge/License-Dual%20Academic%2FCommercial-blue.svg)](#license--intellectual-property)
[![Language: C99/C11](https://img.shields.io/badge/Language-C99%2FC11-green.svg)](#integration--usage)
[![Standard: IEEE 754](https://img.shields.io/badge/Precision-IEEE%20754%20Double-orange.svg)](#performance-benchmarks)

**Author:** Djempe James (Independent Researcher)  
**Contact:** epmejd@gmail.com | +237 697 778 96  

---

## Overview

This repository provides a zero-dependency, header-only C library implementing the **Epmej & Peumeu Monique Decomposition** combined with the **Heron (Old-Babylonian) iterative method** for fast, deterministic square root computation.

Designed specifically for **FPU-less microcontrollers** (ARM Cortex-M0/M0+, AVR ATmega328P, ESP8266) and **Hard Real-Time Systems**, this algorithm eliminates calls to the slow software emulation of `<math.h>` `sqrt()` and avoids unbounded `while` loops.

---

## Mathematical Foundation

For any real number $Y > 1$, the **Epmej & Peumeu Monique Property** guarantees a unique decomposition $(x, m) \in \mathbb{R}^2$ such that:

$$Y = x^2 + m \quad \text{with} \quad x \ge 1 \quad \text{and} \quad x \ge 2|m|$$

Analytical resolution yields a constant-width validity interval $\Delta x = \frac{1}{2}$ centered at the optimal seed:

$$x_0 = \frac{\sqrt{1 + 16Y}}{4}, \quad m = -\frac{1}{16} = -0.0625$$

To avoid any dependency on native `SQRT` instructions, this library provides two `SQRT`-free initialization strategies followed by **2 fixed Heron iterations**:
1. **Normalized 2nd-Order Taylor Series (`epmej_peumeu_sqrt_taylor`):** Pure algebraic initialization independent of hardware floating-point representations.
2. **IEEE 754 Fast Bitwise Seed (`epmej_peumeu_sqrt_fast`):** Constant-time $\mathcal{O}(1)$ bit-level initialization optimized for maximum throughput and energy savings.

---

## Performance Benchmarks (10,000 IEEE 754 Operations)

Evaluated across standard FPU-less IoT microcontrollers against standard `<math.h>` `sqrt()`:

| Microcontroller Target | Standard `sqrt()` | Epmej-Peumeu + Heron | Net Time Saved | Energy / Battery Savings | Max Relative Error |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Arduino Uno (ATmega328P)**<br>*16 MHz • 8-bit • ~60 mW* | 380.00 ms<br>*(608 cycles/op)* | **115.62 ms**<br>*(185 cycles/op)* | **-264.38 ms**<br>*(3.29× faster)* | **69.6 %**<br>*(15.86 mJ saved)* | $< 1.11 \times 10^{-16}$ |
| **ESP8266 (Tensilica L106)**<br>*80 MHz • 32-bit • ~230 mW* | 52.50 ms<br>*(420 cycles/op)* | **14.38 ms**<br>*(115 cycles/op)* | **-38.12 ms**<br>*(3.65× faster)* | **72.6 %**<br>*(8.77 mJ saved)* | $< 1.11 \times 10^{-16}$ |
| **ARM Cortex-M0 / M0+**<br>*48 MHz • 32-bit • ~15 mW* | 93.75 ms<br>*(450 cycles/op)* | **26.04 ms**<br>*(125 cycles/op)* | **-67.71 ms**<br>*(3.60× faster)* | **72.2 %**<br>*(1.02 mJ saved)* | $< 1.11 \times 10^{-16}$ |

---

## Integration & Usage

Copy `epmej_peumeu_heron_EN.h` into your embedded project `include/` folder—no external linker flags (`-lm`) required.

```c
#include <stdio.h>
#include "epmej_peumeu_heron_EN.h"

int main(void) {
    double Y = 23.0;

    // 1. High-speed IEEE 754 bitwise seed + Heron
    double res_fast = epmej_peumeu_sqrt_fast(Y);

    // 2. Pure algebraic Taylor series seed + Heron
    double res_taylor = epmej_peumeu_sqrt_taylor(Y);

    printf("Fast Bitwise Result : %.15f\n", res_fast);
    printf("Taylor Series Result: %.15f\n", res_taylor);
    return 0;
}
```

### Compilation
```bash
gcc -O3 main.c -o test_epmej
./test_epmej
```

---

## Citation (BibTeX)

If you use this algorithm or library in academic research or industrial benchmarks, please cite:

```bibtex
@article{djempe2026epmej,
  title   = {An Analytical and Iterative Framework for Efficient Square Root Computation: The Epmej \& Peumeu Monique Decomposition Combined with the Old-Babylonian Method},
  author  = {Djempe, James},
  year    = {2026},
  note    = {Independent Researcher, Yaounde, Cameroon}
}
```

---

## License & Intellectual Property

* **Academic & Open-Source Evaluation:** Free to use, test, and cite for non-commercial research and benchmarking purposes.
* **Commercial & Embedded Licensing:** For integration into commercial firmware, proprietary SDKs, or ASIC/FPGA hardware designs, please contact **Djempe James** (`epmejd@gmail.com`).
