//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *                                                                          
// * Description:  Main function.
// *                                                                          
// * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
// * Copyright (C) 2011 Spectrum Digital, Incorporated
// *                                                                          
// * Author: Iva Bosnic RA 222/2022
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "tistdtypes.h"
#include <cstdio>
#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "WAVheader.h"
#include "band_pass.h"
#include "iir.h"

static WAV_HEADER outputWAVhdr;
static WAV_HEADER inputWAVhdr;

#define SAMPLE_RATE 48000L
#define N 100
#define M_PI 3.14

#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];

NotchFilter notch_300Hz, notch_300Hz_stage2, notch_300Hz_stage3;   // Za 300 Hz
NotchFilter notch_2000Hz, notch_2000Hz_stage2, notch_2000Hz_stage3;  // Za 2000 Hz

void main(void) {
    int i, j;
    Int32 number_of_blocks;

    EZDSP5535_init();

    aic3204_hardware_init();

    aic3204_set_input_filename("../22.wav");
    aic3204_set_output_filename("../Output4red.wav");

    aic3204_init();

    aic3204_dma_init();

    aic3204_read_wav_header(&inputWAVhdr);

    outputWAVhdr = inputWAVhdr;

    number_of_blocks = inputWAVhdr.data.SubChunk2Size /
                       (inputWAVhdr.fmt.NumChannels * inputWAVhdr.fmt.BitsPerSample * AUDIO_IO_SIZE / 8);

    aic3204_write_wav_header(&outputWAVhdr);

    init_notch_filter_300Hz(&notch_300Hz);
    init_notch_filter_300Hz(&notch_300Hz_stage2); // Druga faza za 300Hz

    init_notch_filter_2000Hz(&notch_2000Hz);
    init_notch_filter_2000Hz(&notch_2000Hz_stage2); // Druga faza za 2000Hz
    init_notch_filter_2000Hz(&notch_2000Hz_stage3); // Treća faza za 2000Hz

    for(i = 0; i < number_of_blocks; ++i) {
        aic3204_read_block(InputBufferL, InputBufferR);

        for(j = 0; j < AUDIO_IO_SIZE; j++) {
        	//OutputBufferL[j] = fir_circular129(InputBufferL[j]);
        	//OutputBufferR[j] = fir_circular129(InputBufferR[j]);

        	// Primeni FIR filter
        	//Int16 tempL_FIR = fir_circular(InputBufferL[j]);
        	//Int16 tempR_FIR = fir_circular(InputBufferR[j]);

        	/*
        	// Filtriranje za 300Hz
        	Int16 tempL_IIR = apply_notch_filter(&notch_300Hz, InputBufferL[j]);
        	Int16 tempR_IIR = apply_notch_filter(&notch_300Hz, InputBufferR[j]);

        	// Filtriranje za 2000Hz
        	OutputBufferL[j] = apply_notch_filter(&notch_2000Hz, tempL_IIR);
        	OutputBufferR[j] = apply_notch_filter(&notch_2000Hz, tempR_IIR);
			*/

        	OutputBufferL[j] = apply_notch_filter_4th_order(InputBufferL[j], &notch_300Hz, &notch_300Hz_stage2);
        	OutputBufferR[j] = apply_notch_filter_4th_order(InputBufferR[j], &notch_300Hz, &notch_300Hz_stage2);

        	// Primena filtera šestog reda (3 faze kaskadno)
        	//OutputBufferL[j] = apply_notch_filter_6th_order(InputBufferL[j], &notch_2000Hz, &notch_2000Hz_stage2, &notch_2000Hz_stage3);
        	//OutputBufferR[j] = apply_notch_filter_6th_order(InputBufferR[j], &notch_2000Hz, &notch_2000Hz_stage2, &notch_2000Hz_stage3);
        }

        aic3204_write_block(OutputBufferL, OutputBufferR);
    }

    /* Disable I2S and put codec into reset */
    aic3204_disable();

    printf("\n***Program has Terminated***\n");
    SW_BREAKPOINT;
}
