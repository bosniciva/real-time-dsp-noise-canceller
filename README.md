# Real-Time Audio Noise Cancellation System (DSP)

A specialized Digital Signal Processing (DSP) framework designed to identify, isolate, and eliminate deterministic sinusoidal noise interference from corrupted audio streams (`.wav` format at 48 kHz). This project features the design, mathematical modeling, and hardware-optimized implementation of both **Finite Impulse Response (FIR)** and **Infinite Impulse Response (IIR)** filtering blocks operating inside a strict real-time execution matrix.

## Architectural & Algorithmic Pipeline

The processing core analyzes the unoptimized signal spectra to calculate cutoff boundaries before passing samples through a multi-stage noise suppression pipeline:

1.  **Spectral Profiling (Audacity):** Conducted frequency analysis mapping highlighting distinct harmonic noise spikes at $f_1 = 300\text{ Hz}$ and $f_2 = 2000\text{ Hz}$.
2.  **Hardware-Optimized IIR Cascade (Notch Filters):** Implemented high-Q 2nd-order IIR Notch Filters to directly nullify the narrow $300\text{ Hz}$ and $2000\text{ Hz}$ interference tones via standard difference equations, utilizing fractional shift division scaling (`SCALE_FACTOR = 16384`) to operate entirely within 16-bit signed integer (`Int16`) fixed-point logic.
3.  **Dynamic FIR Band-Pass Array (Window Design):** Developed a modular FIR Band-Pass compiler running across variable filter orders (**35, 77, and 129 Taps**) using a rectangular window profile to cleanly isolate remaining spectral payloads from baseline low-frequency noise floors ($45\text{ Hz}$).
4.  **Circular Buffer Memory Optimization:** To minimize real-time processing overhead, data loops are processed via optimized circular delay structures (`history` arrays tracking `state` indices) combined with intrinsic multiply-accumulate macro calls (`_smac`).

---

## Comparative Filter Order Benchmarking

The implementation evaluates compiler profiling and passband ripples across alternative FIR Tap depths to balance transition-band sharpness against system group delay:

### 1. 35-Tap FIR Baseline Configuration
*   **Characteristics:** Lowest group delay configuration ($354.16\text{ \mu s}$); wider transition band regions.
*   **Result:** Fast execution profile, but lower out-of-band attenuation.

### 2. 77-Tap FIR Mid-Tier Profile
*   **Characteristics:** Sharpens the roll-off curve cleanly; intermediate response latency ($791.66\text{ \mu s}$).
*   **Result:** Smooth, reliable noise-floor reduction with enhanced attenuation limits.

### 3. 129-Tap FIR High-Selectivity Engine
*   **Characteristics:** Maximum stopband attenuation; narrowest transition band width at the cost of processing latency ($1.333\text{ ms}$).
*   **Result:** High spectral purity and effective elimination of side-lobe pollution leaks.

---

## Code Implementation: Circular FIR with Headroom Tracking

```c
// Implementation of a circular FIR filter with runtime headroom bit-shifting
Int16 fir_circular77(Int16 input) {
    Int16 i;
    Uint16 temp_state = state; // Thread-safe tracking state snapshot
    Int32 ret_val = 0;

    history[temp_state] = input;
    if (++temp_state >= Ntap77) {
        temp_state = 0;
    }

    // Multiply-Accumulate execution block loop
    for (i = Ntap77 - 1; i >= 0; i--) {
        // Scaled multiplication utilizing HEADROOM_BITS protection to prevent overflow
        ret_val = _smac(ret_val, FIRCoef77[i] >> HEADROOM_BITS, history[temp_state]);
        if (++temp_state >= Ntap77) {
            temp_state = 0;
        }
    }
    state = temp_state; 
    return (Int16)(ret_val >> 16); // Fixed-point stabilization conversion shift
}
```

---
## 📁 Repository Structure

*   `/src` — Fixed-point arithmetic DSP implementation blocks.
    *   `band_pass.c` / `band_pass.h` — Circular FIR filter architectures for 35, 77, and 129 taps.
    *   `iir.c` / `iir.h` — Second-order biquad IIR notch filter setup equations and difference arrays.
*   `/simulation_tools` — WinFilter models, filter coefficients parameters matrices, and frequency responses data plots.
*   `/audios` — Sample input waveforms (`22.wav`) and processed audio output profiles.
