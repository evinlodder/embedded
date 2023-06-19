#ifndef TICK_H
#define TICK_H

#include <stdint.h>

void tick_init(void);
void sleep(uint32_t);
void sleep_ticks(uint32_t);

#endif
