#ifndef DSP_STREAM_BUFFER_H
#define DSP_STREAM_BUFFER_H

#include <stddef.h>
#include <stdint.h>

/*
 * DSP stream buffer
 * circular buffer for real-time DSP pipelines
*/

typedef struct {
    float *buffer; //buffer
    size_t size; //size of buffer
    size_t read_pos; //read pointer
    size_t write_pos; //write pointer
} dsp_stream_t;

void dsp_stream_init(dsp_stream_t *s, float *buffer, size_t size);

size_t dsp_stream_write(dsp_stream_t *s,const float *data, size_t n);

size_t dsp_stream_read(dsp_stream_t *s, float *data, size_t n);

size_t dsp_stream_available_for_read(dsp_stream_t *s);

size_t dsp_stream_available_for_write(dsp_stream_t *s);

#endif
