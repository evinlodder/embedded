#include <rand>

static uint64_t next {1};

uint8_t vin::rand8() {
    next = next * 1103515245 + 12345;
    return next >> 56;
}

uint16_t vin::rand16() {
    next = next * 1203543285 + 12895;
    return next >> 48;
}

uint32_t vin::rand32() {
    next = next * 1103999945 + 23445;
    return next >> 32;
}

uint64_t vin::rand64() {
    next = next * 1103143785 + 11115;
    return next;
}

void srand(unsigned seed) { next = seed; }
