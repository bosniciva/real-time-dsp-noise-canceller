/*
 * iir.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Iva Bosnic RA 222/2022
 */
#include "iir.h"
#define SCALE_FACTOR 16384
void init_notch_filter_300Hz(NotchFilter* filter) {
	float r = 0.99; // Prigušenje
    float cosine = cos(2 * M_PI * 300 / 48000);
    filter->coefficients[0] = SCALE_FACTOR;            // A0
    filter->coefficients[1] = -2*cosine*SCALE_FACTOR; // A1
    filter->coefficients[2] = SCALE_FACTOR;            // A2
    filter->coefficients[3] = -2*SCALE_FACTOR * r * cosine; // B1
    filter->coefficients[4] = SCALE_FACTOR *(r*r);   // B2

    filter->x_history[0] = 0;
    filter->x_history[1] = 0;
    filter->y_history[0] = 0;
    filter->y_history[1] = 0;
}

// Inicijalizacija Notch filtera za 2000 Hz
void init_notch_filter_2000Hz(NotchFilter* filter) {
	float r = 0.99; // Prigušenje
	float cosine = cos(2 * M_PI * 2000 / 48000);
    filter->coefficients[0] = SCALE_FACTOR;            // A0
    filter->coefficients[1] = -2*cosine*SCALE_FACTOR; // A1
    filter->coefficients[2] = SCALE_FACTOR;            // A2
    filter->coefficients[3] = -2*r* SCALE_FACTOR * cosine; // B1
    filter->coefficients[4] = (r*r) * SCALE_FACTOR;   // B2

    filter->x_history[0] = 0;
    filter->x_history[1] = 0;
    filter->y_history[0] = 0;
    filter->y_history[1] = 0;
}

// Primena Notch filtera
Int16 apply_notch_filter(NotchFilter* filter, Int16 input) {
    Int32 accum = 0;

    accum = _smac(accum, filter->coefficients[0], input);
    accum = _smac(accum, filter->coefficients[1], filter->x_history[0]);
    accum = _smac(accum, filter->coefficients[2], filter->x_history[1]);
    accum = _smas(accum, filter->coefficients[3], filter->y_history[0]);
    accum = _smas(accum, filter->coefficients[4], filter->y_history[1]);

    accum >>= 16;

    filter->x_history[1] = filter->x_history[0];
    filter->x_history[0] = input;

    filter->y_history[1] = filter->y_history[0];
    filter->y_history[0] = (Int16)accum;

    return (Int16)accum;
}
Int16 apply_notch_filter_4th_order(Int16 input, NotchFilter* stage1, NotchFilter* stage2) {
    Int16 temp = apply_notch_filter(stage1, input);  // Prvi drugoredni filter
    return apply_notch_filter(stage2, temp);        // Drugi drugoredni filter
}
Int16 apply_notch_filter_6th_order(Int16 input, NotchFilter* stage1, NotchFilter* stage2, NotchFilter* stage3) {
    Int16 temp1 = apply_notch_filter(stage1, input);  // Prvi drugoredni filter
    Int16 temp2 = apply_notch_filter(stage2, temp1);  // Drugi drugoredni filter
    return apply_notch_filter(stage3, temp2);         // Treći drugoredni filter
}





