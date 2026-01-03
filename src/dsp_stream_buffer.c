#include "dsp_stream_buffer.h"
#include <string.h>

void dsp_stream_init(dsp_stream_t *s, float *buffer, size_t size) {
    s->buffer = buffer;
    s->size = size;
    s->read_pos = 0;
    s->write_pos = 0;
}

size_t dsp_stream_available_for_read(dsp_stream_t *s) {
    if (s->write_pos >= s->read_pos)
	   return s->write_pos - s->read_pos;
    else
	   return s->size - (s->read_pos - s->write_pos); 
}

size_t dsp_stream_available_for_write(dsp_stream_t *s) {
    return s->size - dsp_stream_available_for_read(s) - 1;
}

size_t dsp_stream_write(dsp_stream_t *s, const float *data, size_t n) {
    size_t free_space = dsp_stream_available_for_write(s);
    if (n > free_space) n = free_space;
    
    for (size_t i = 0; i < n; i++) {
        s->buffer[s->write_pos] = data[i];
	s->write_pos = (s->write_pos + 1) % s->size;
    }
    return n;
}

size_t dsp_stream_read(dsp_stream_t *s, float *data, size_t n) {
    size_t available = dsp_stream_available_for_read(s);
    if (n > available) n = available;

    for (size_t i =0; i < n; i++) {
        data[i] = s->buffer[s->read_pos];
	s->read_pos = (s->read_pos + 1) % s->size;
    }
    return n;
}
