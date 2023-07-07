//*****************************************************************************
//
// lab7.cpp - lab 7 for the UT course.
// Evin Lodder 7/5
//
//*****************************************************************************

#include <vin>
#include <port>

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
// Resembles pacemaker, where PF4 = the atrial sensor (AS), the green LED is the ready LED,
// and the red LED is the ventrical trigger (VT)
// NOTE: I will not be using the <rgb.h> or <buttons.h> headers because
// I want to practice interfacing with the hardware and reading the data sheet on my own.
//
//*****************************************************************************

void portf_init();

int
main()
{
    using namespace port;
    //initialize portf and sleeping functionality
    //portf_init();
    port_t<portname::F> f{};
    f.initialize({
                    lock<0x4C4F434B>{},
                    cr<0x1F>{},
                    amsel<0>{},
                    pctl<0>{},
                    dir<0xE>{},
                    afsel<0>{},
                    pur<0x11>{},
                    den<0x1F>{},
                 });
    vin::tick_init();

    ACCESS(PORTF_DATA) |= 1 << 3; //turn LED green (PF3)
    while(true) {
        if(!(ACCESS(PORTF_DATA) & (1 << 4))) { //if PF4 == 0 (switch is pressed)
            ACCESS(PORTF_DATA) &= ~(1 << 3); //turn off green LED
            vin::sleep(10); //wait 10 ms
            while(!(ACCESS(PORTF_DATA) & (1 << 4))) {} //wait until switch is released
            vin::sleep(250); //wait 250 ms
            ACCESS(PORTF_DATA) |= 1 << 1; //turn LED red (PF1) (VT on)
            vin::sleep(250); //wait 250 more ms
            ACCESS(PORTF_DATA) &= ~(1 << 1); // turn off red LED (VT off)
            ACCESS(PORTF_DATA) |= 1 << 3; //turn ready back on
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
