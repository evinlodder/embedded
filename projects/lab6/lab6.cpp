//*****************************************************************************
//
// lab6.cpp - lab 6 for the UT course.
// Evin Lodder 7/4
//
//*****************************************************************************

#include <vin>

#define ACCESS(x) (*((uint32_t*)x))

//registers needed
constexpr uint32_t RCGCGPIO    {0x400FE608}; //control register to enable clock
constexpr uint32_t PORTF_BASE  {0x40025000}; //base for port F
constexpr uint32_t PORTF_LOCK  {PORTF_BASE + 0x520}; //register to unlock PF0
constexpr uint32_t PORTF_CR    {PORTF_BASE + 0x524}; //register to enable/disable writing to certain bits
constexpr uint32_t PORTF_DIR   {PORTF_BASE + 0x400}; //register to set pin direction
constexpr uint32_t PORTF_AFSEL {PORTF_BASE + 0x420}; //register to enable/disable alternate functionality
constexpr uint32_t PORTF_DEN   {PORTF_BASE + 0x51C}; //register to enable/disable pin
constexpr uint32_t PORTF_PUR   {PORTF_BASE + 0x510}; //register to enable/disable pull-up resistor
constexpr uint32_t PORTF_PCTL  {PORTF_BASE + 0x52C}; //register to select alternate function - use with AFSEL
constexpr uint32_t PORTF_DATA  {PORTF_BASE + 0x07C}; //register to only be able to effect first 5 bits
constexpr uint32_t PORTF_AMSEL {PORTF_BASE + 0x528}; //register to enable/disable analog mode

//*****************************************************************************
//
// Program starts with blue light on
// If sw1 is on, LED flashes blue at 100ms speed
// Otherwise, it remains solid blue
// NOTE: I will not be using the <rgb.h> or <buttons.h> headers because
// I want to practice interfacing with the hardware and reading the data sheet on my own.
//
//*****************************************************************************

void portf_init();

int
main()
{
    //initialize portf and sleeping functionality
    portf_init();
    vin::tick_init();

    ACCESS(PORTF_DATA) |= 1 << 2; //turn LED blue (PF2)
    while(true) {
        if(!(ACCESS(PORTF_DATA) & (1 << 4))) { //if PF4 == 0 (switch is pressed)
            ACCESS(PORTF_DATA) &= ~(1 << 2); //turn off blue LED
            vin::sleep(100); //wait 100 ms
            ACCESS(PORTF_DATA) |= 1 << 2; //turn LED back on
            vin::sleep(100); //so we don't get off-timed flickering
        }
    }
}

void portf_init() {
    volatile uint32_t delay{};
    ACCESS(RCGCGPIO) |= 0x20;        //enable the systimer
    delay = ACCESS(RCGCGPIO);        //delay so system has time to react
    ACCESS(PORTF_LOCK) = 0x4C4F434B; //unlock port F (for PF0)
    ACCESS(PORTF_CR) |= 0x1F;        //allow afsel, pur, and den to be written
    ACCESS(PORTF_AMSEL) = 0;         //disable analog
    ACCESS(PORTF_PCTL) = 0;          //set function to gpio
    ACCESS(PORTF_DIR) |= 0xE;        //set direction of LED pins to output and switches to input
    ACCESS(PORTF_AFSEL) = 0;         //disable alternate functions
    ACCESS(PORTF_PUR) |= 0x11;       //enable pull-up resistors for the switches so they don't short
    ACCESS(PORTF_DEN) |= 0x1F;       //enable the digital gpio pins
}
