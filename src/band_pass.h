/*
 * band_pass.h
 *
 *  Created on: Dec 7, 2024
 *      Author: Iva Bosnic RA 222/2022
 */

#ifndef BAND_PASS_H_
#define BAND_PASS_H_

#include <stdio.h>
#include "tistdtypes.h"
#include <c55x.h>
#define Ntap 35
#define Ntap77 77
#define Ntap129 129
#define DCgain 262144
#define HEADROOM_BITS 3 // Skaliranje prema dokumentaciji

// Funkcije
Int16 fir_circular(Int16 input);
Int16 fir_circular77(Int16 input);

#endif /* BAND_PASS_H_ */
