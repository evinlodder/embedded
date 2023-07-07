//*****************************************************************************
//
// lab7.cpp - lab 7 for the UT course.
// Evin Lodder 7/5
//
//*****************************************************************************

#include <vin>
#include <port>

//*****************************************************************************
//
// Resembles pacemaker, where PF4 = the atrial sensor (AS), the green LED is the ready LED,
// and the red LED is the ventrical trigger (VT)
// NOTE: I will not be using the <rgb.h> or <buttons.h> headers because
// I want to practice interfacing with the hardware and reading the data sheet on my own.
//
//*****************************************************************************
int
main()
{
    using namespace port;
    //initialize portf and sleeping functionality
    //portf_init();
    port_t<portname::F> f{};
    f.initialize({
                    clockreg::rcgcgpio,
                 },
                    lock{lock_unlock},
                    cr{0x1F},
                    amsel{0},
                    pctl{0},
                    dir{0xE},
                    afsel{0},
                    pur{0x11},
                    den{0x1F}
                 );
    vin::tick_init();

    f.data(3) |= 1 << 3; //turn LED green (PF3)
    while(true) {
        if(!(f.data(4) & (1 << 4))) { //if PF4 == 0 (switch is pressed)
            f.data(3) &= ~(1 << 3); //turn off green LED
            vin::sleep(10); //wait 10 ms
            while(!(f.data(4) & (1 << 4))) {} //wait until switch is released
            vin::sleep(250); //wait 250 ms
            f.data(1) |= 1 << 1; //turn LED red (PF1) (VT on)
            vin::sleep(250); //wait 250 more ms
            f.data(1) &= ~(1 << 1); // turn off red LED (VT off)
            f.data(3) |= 1 << 3; //turn ready back on
        }
    }
}
