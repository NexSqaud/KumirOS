#include <stdint.h>

#ifndef _UTILS_H
#define _UTILS_H

extern uint64_t frequency;

uint32_t rand(void);
void srand(uint32_t seed);

uint64_t getTicksCount(void);
void sleep(uint32_t ticks);

#endif