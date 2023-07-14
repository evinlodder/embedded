//*****************************************************************************
//
// lab9.cpp - lab 9 for the UT course.
// Evin Lodder 7/10
//
//*****************************************************************************

#include <vin>
#include <port>
#include <dump.hpp>
#include <array>
#include <utility>

//dmp declared outside of main so it can be viewed by gdb
auto dmp = vin::make_dump<uint32_t, 50>();


//*****************************************************************************
//
// any time either button is on, the light flashes at 10 Hz
// the important part is the data collection- portf bits should be recorded
// any time either an input or an output changes
//
//*****************************************************************************
int
main()
{
    using namespace vin;
    //initialize portf and sleeping functionality
    port<portname::F> f{};
    f.initialize({
                    clockreg::rcgcgpio,
                 },
                    lock{lock_unlock},
                    cr{0x1F},
                    amsel{0x1F, op::clear},
                    pctl{0xFFFFF, op::clear},
                    dir{0xE},
                    dir{0x11, op::clear},
                    afsel{0x1F, op::clear},
                    pur{0x11},
                    den{0x1F}
                 );

    tick_init();

    while(1) {
        //get current data state for used pins
        uint8_t prev_data = f.data(0, 1, 4);

        if(!(f.data(0) & 1) || !(f.data(4) & (1 << 4))) { //if either switch is pressed
            f.data(1) ^= 0x2; //toggle the red LED
            sleep(50);
        } else {
            f.data(1) = 0;
        }
        //if data changed, save to dump
        if(prev_data != f.data(0, 1, 4)) {
            dmp.save(f.data(0, 1, 4));
        }
    }
}
