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
//  First physical board project
//  Gets button input from PE0 and sends output to a red LED on PE1
//  If button is pressed, LED flashes, otherwise it remains solid color
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
                 });

    e.init_digital_pins(pin0<direction::in>{}, pin1<direction::out>{});

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
