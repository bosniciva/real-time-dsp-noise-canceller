/*
 * band_pass.c
 *
 *  Created on: Dec 7, 2024
 *      Author: Iva Bosnic RA 222/2022
 */

#include "band_pass.h"

// Kružni bafer i stanje za kružni FIR filter
static Int16 history[Ntap] = {0}; // Istorijski uzorci, inicijalizovani na nule
static Uint16 state = 0;          // Stanje kružnog bafera

// FIR koeficijenti (originalne vrednosti)
static Int16 FIRCoef[Ntap] = {
        -4454,
        -4163,
        -3539,
        -2574,
        -1277,
          328,
         2203,
         4294,
         6536,
         8853,
        11164,
        13386,
        15434,
        17232,
        18709,
        19807,
        20485,
        20713,
        20485,
        19807,
        18709,
        17232,
        15434,
        13386,
        11164,
         8853,
         6536,
         4294,
         2203,
          328,
        -1277,
        -2574,
        -3539,
        -4163,
        -4454
	};

// Implementacija kružnog FIR filtra sa skaliranjem u hodu
Int16 fir_circular(Int16 input) {
    Int16 i;
    Uint16 temp_state = state; // Kopija trenutnog stanja
    Int32 ret_val = 0;

    // Dodajemo ulazni uzorak u kružni bafer
    history[temp_state] = input;
    if (++temp_state >= Ntap) { // Proveravamo da li treba vratiti na početak
        temp_state = 0;
    }

    // Računanje FIR izlaza sa skaliranjem koeficijenata
    for (i = Ntap - 1; i >= 0; i--) {
        // Skaliramo koeficijent tokom množenja prema HEADROOM_BITS
        ret_val = _smac(ret_val, FIRCoef[i] >> HEADROOM_BITS, history[temp_state]);
        if (++temp_state >= Ntap) { // Kružna inkrementacija
            temp_state = 0;
        }
    }

    state = temp_state; // Ažuriramo globalno stanje

    // Vraćamo normalizovan i ograničen rezultat
    return (Int16)(ret_val >> 16); // Desni shift za fiksnu tačku
}

static Int16 FIRCoef77[Ntap77] = {
		-813,
		         -256,
		          350,
		          969,
		         1559,
		         2080,
		         2492,
		         2762,
		         2861,
		         2771,
		         2484,
		         2005,
		         1351,
		          551,
		         -354,
		        -1316,
		        -2275,
		        -3169,
		        -3933,
		        -4505,
		        -4826,
		        -4847,
		        -4531,
		        -3851,
		        -2802,
		        -1390,
		          357,
		         2397,
		         4673,
		         7113,
		         9635,
		        12150,
		        14567,
		        16796,
		        18753,
		        20360,
		        21556,
		        22293,
		        22542,
		        22293,
		        21556,
		        20360,
		        18753,
		        16796,
		        14567,
		        12150,
		         9635,
		         7113,
		         4673,
		         2397,
		          357,
		        -1390,
		        -2802,
		        -3851,
		        -4531,
		        -4847,
		        -4826,
		        -4505,
		        -3933,
		        -3169,
		        -2275,
		        -1316,
		         -354,
		          551,
		         1351,
		         2005,
		         2484,
		         2771,
		         2861,
		         2762,
		         2492,
		         2080,
		         1559,
		          969,
		          350,
		         -256,
		         -813
};
Int16 fir_circular77(Int16 input) {
    Int16 i;
    Uint16 temp_state = state; // Kopija trenutnog stanja
    Int32 ret_val = 0;

    // Dodajemo ulazni uzorak u kružni bafer
    history[temp_state] = input;
    if (++temp_state >= Ntap77) { // Proveravamo da li treba vratiti na početak
        temp_state = 0;
    }

    // Računanje FIR izlaza sa skaliranjem koeficijenata
    for (i = Ntap77 - 1; i >= 0; i--) {
        // Skaliramo koeficijent tokom množenja prema HEADROOM_BITS
        ret_val = _smac(ret_val, FIRCoef77[i] >> HEADROOM_BITS, history[temp_state]);
        if (++temp_state >= Ntap77) { // Kružna inkrementacija
            temp_state = 0;
        }
    }

    state = temp_state; // Ažuriramo globalno stanje

    // Vraćamo normalizovan i ograničen rezultat
    return (Int16)(ret_val >> 16); // Desni shift za fiksnu tačku
}

static Int16 FIRCoef129[Ntap129] = {
			1053, 1291, 1465, 1565, 1582, 1513, 1357, 1121, 813, 448, 44, -379,
		    -798, -1192, -1536, -1810, -1995, -2079, -2051, -1908, -1652, -1292,
		    -842, -323, 241, 823, 1390, 1912, 2357, 2697, 2908, 2968, 2867, 2598,
		    2165, 1580, 864, 45, -840, -1751, -2640, -3457, -4155, -4683, -4998,
		    -5062, -4843, -4320, -3482, -2330, -877, 851, 2819, 4979, 7277, 9647,
		    12024, 14336, 16513, 18487, 20197, 21588, 22615, 23245, 23457, 23245,
		    22615, 21588, 20197, 18487, 16513, 14336, 12024, 9647, 7277, 4979,
		    2819, 851, -877, -2330, -3482, -4320, -4843, -5062, -4998, -4683, -4155,
		    -3457, -2640, -1751, -840, 45, 864, 1580, 2165, 2598, 2867, 2968, 2908,
		    2697, 2357, 1912, 1390, 823, 241, -323, -842, -1292, -1652, -1908, -2051,
		    -2079, -1995, -1810, -1536, -1192, -798, -379, 44, 448, 813, 1121, 1357,
		    1513, 1582, 1565, 1465, 1291, 1053
};
Int16 fir_circular129(Int16 input) {
    Int16 i;
    Uint16 temp_state = state; // Kopija trenutnog stanja
    Int32 ret_val = 0;

    // Dodajemo ulazni uzorak u kružni bafer
    history[temp_state] = input;
    if (++temp_state >= Ntap129) { // Proveravamo da li treba vratiti na početak
        temp_state = 0;
    }

    // Računanje FIR izlaza sa skaliranjem koeficijenata
    for (i = Ntap129 - 1; i >= 0; i--) {
        // Skaliramo koeficijent tokom množenja prema HEADROOM_BITS
        ret_val = _smac(ret_val, FIRCoef129[i] >> HEADROOM_BITS, history[temp_state]);
        if (++temp_state >= Ntap77) { // Kružna inkrementacija
            temp_state = 0;
        }
    }

    state = temp_state; // Ažuriramo globalno stanje

    // Vraćamo normalizovan i ograničen rezultat
    return (Int16)(ret_val >> 16); // Desni shift za fiksnu tačku
}
