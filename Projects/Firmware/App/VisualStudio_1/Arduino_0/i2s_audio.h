#ifndef I2S_AUDIO_H
#define I2S_AUDIO_H

#include <stdint.h>

void init_i2s();
void acquire_sample_chunk(uint32_t t_now);
void calculate_vu();

#endif