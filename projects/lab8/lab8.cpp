//*****************************************************************************
//
// lab8.cpp - lab 8 for the UT course.
// Evin Lodder 7/7
//
//*****************************************************************************

#include <vin>
#include <port.hpp>

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
    port_t<portname::E> e{};
    e.initialize({
                    clockreg::rcgcgpio,
                 },
                    amsel{0x3, op::clear}, //disable amsel
                    pctl{0xFF, op::clear}, //set to GPIO
                    dir{0x1, op::clear}, //set pe0 to input
                    dir{0x2}, //set pe1 to output
                    afsel{0x3, op::clear}, //disable alt func
                    den{0x3} //enable pe0 and pe1
                 );

    vin::tick_init();

    e.data(1) |= 1 << 1; //turn LED on
    while(true) {
        vin::sleep(100);
        if(e.data(0) & 1) { //if switch is pressed
            e.data(1) ^= 1 << 1; //toggle switch
        } else {
            e.data(1) |= 1 << 1; //turn LED on
        }
    }
}
