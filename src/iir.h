/*
 * iir.h
 *
 *  Created on: Dec 8, 2024
 *      Author: Iva Bosnic RA 222/2022
 */

#ifndef IIR_H_
#define IIR_H_

#include <stdio.h>
#include "tistdtypes.h"
#include <c55x.h>
#include <math.h>
#define M_PI 3.14

// Struktura za IIR Notch filter
#define MAX_HISTORY_SIZE 2  // Dve prethodne vrednosti za x i y

typedef struct {
    float coefficients[5];  // Koeficijenti filtra: A0, A1, A2, B1, B2
    Int16 x_history[MAX_HISTORY_SIZE];  // Istorija ulaznih vrednosti (x(n-1), x(n-2))
    Int16 y_history[MAX_HISTORY_SIZE];  // Istorija izlaznih vrednosti (y(n-1), y(n-2))
} NotchFilter;

// Inicijalizacija IIR Notch filtera za 300Hz
void init_notch_filter_300Hz(NotchFilter* filter);

// Inicijalizacija IIR Notch filtera za 2000Hz
void init_notch_filter_2000Hz(NotchFilter* filter);

// Primena IIR Notch filtera za 300Hz
Int16 apply_notch_filter_300Hz(NotchFilter* filter, Int16 input);

// Primena IIR Notch filtera za 2000Hz
Int16 apply_notch_filter_2000Hz(NotchFilter* filter, Int16 input);

#endif /* IIR_H_ */
