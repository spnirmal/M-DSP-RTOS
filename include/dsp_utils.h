#ifndef DSP_UTILS_H
#define DSP_UTILS_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    float *coeffs;
    float *history;
    size_t num_taps;
    size_t index;
} fir_t;

void fir_init(fir_t *f, float *coeffs, size_t num_taps);
float fir_process_sample(fir_t *f, float input);
void fir_process_buffer(fir_t *f, const float *input, float *output, size_t len);

void generate_sine_wave(float *buf, size_t len, float freq_hz, float sample_rate);
void dsp_delay_ms(uint32_t ms);

#endif