#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "mdsp_rtos.h"
#include "task.h"
#include "dsp_stream_buffer.h"

#define STREAM_SIZE 16
static float stream_storage[STREAM_SIZE];
static dsp_stream_t audio_stream;

static void producer_task(void *arg) {
    float sample = 0.0f;
    (void)arg;
    for (;;) {
        sample += 1.0f;
	size_t written = dsp_stream_write(&audio_stream, &sample, 1);
	if (written == 1) {
            printf("[producer] wrote %.1f\n", sample);
	}
	else {
	    printf("[producer] BUFFER FULL - sample %.1f DROPPED\n", sample);
	}

	task_yield();
    }
}

static void consumer_task(void *arg) {
    float value;
    (void)arg;
    for (;;) {
        size_t read = dsp_stream_read(&audio_stream, &value, 1);

	if (read == 1) {
	    printf("[consumer] read %.1f\n", value);
	}

	task_yield();
    }
}

int main(void) {
    printf("M-DSP RTOS Phase 2 demo\n");

    rtos_init();
    dsp_stream_init(&audio_stream, stream_storage, STREAM_SIZE);

    task_create("t1",producer_task,NULL, 0);
    task_create("t2",consumer_task,NULL, 0);

    rtos_start();
    return 0;
}
