//*****************************************************************************
//
// lab10.cpp - lab 10 for the UT course.
// Evin Lodder 7/12
//
//*****************************************************************************

#include <vin>
#include <port>
#include <array>
#include <utility>

//*****************************************************************************
//
// traffic light system with a moore fsm
// there are 9 states- 
//
//*****************************************************************************
int
main()
{
    using namespace vin;
    //
    // INITIALIZE PORTS
    // -portf will use built-in LED for walk & don't walk light functionality
    // -porte will use LEDs for the 6 traffic lights
    // -porta will use switches for the 3 inputs
    //

    // F init
    port<portname::F> f{};
    f.initialize({clockreg::rcgcgpio}); //init port f

    f.init_digital_pins(pin1<direction::out>{},  //don't walk light
                        pin3<direction::out>{}); //walk light
    // E init
    port<portname::E> e{};
    e.initialize({clockreg::rcgcgpio}); //init port e

    e.init_digital_pins(pin0<direction::out>{},  //green north/south
                        pin1<direction::out>{},  //yellow north/south
                        pin2<direction::out>{},  //red north/south
                        pin3<direction::out>{},  //green east/west
                        pin4<direction::out>{},  //yellow east/west
                        pin5<direction::out>{}); //red east/west
    // A init
    port<portname::A> a{};
    a.initialize({clockreg::rcgcgpio}); //init port a

    a.init_digital_pins(pin2<direction::in>{},  //east/west car sensor
                        pin3<direction::in>{},  //north/south car sensor
                        pin4<direction::in>{}); //walk sensor
    //
    // INITIALIZE PLL AND SYSTICK
    // -PLL will be initialized to 80 mHz frequency
    //
    pll_init(80);
    tick_init();

    while(1) {
        f.data(1) ^= 0x0; //toggle red
        sleep(1000); //test sleep 1 sec
    }
}
