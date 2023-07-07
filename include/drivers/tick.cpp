#include <tick>
#include <tm4c123gh6pm.h>

#define COUNT          0x00010000
#define CLOCK_SOURCE   0x00000004
#define INT_ENABLE     0x00000002
#define ENABLE_COUNTER 0x00000001
#define RELOAD_MAX     0x00FFFFFF

void vin::tick_init(void) {
    NVIC_ST_CTRL_R = 0; //disable ticks while setting up
    NVIC_ST_RELOAD_R = RELOAD_MAX; //max reload val
    NVIC_ST_CURRENT_R = 0; //any write clears

    NVIC_ST_CTRL_R = ENABLE_COUNTER + CLOCK_SOURCE; //enable core clock
}

void vin::sleep(uint32_t ms) {
    vin::sleep_ticks(ms * 8000);
}

void vin::sleep_ticks(uint32_t ticks) {
    volatile uint32_t elapsed = 0;
    uint32_t start = NVIC_ST_CURRENT_R;

    while(elapsed <= ticks) {
        elapsed = (start - NVIC_ST_CURRENT_R) & 0x00FFFFFF;
    }
}
