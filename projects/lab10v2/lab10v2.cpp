//*****************************************************************************
//
// lab10v2.cpp - lab 10 for the UT course, using fsm.hpp header
// Evin Lodder 7/19
//
//*****************************************************************************

#include <vin>
#include <port>
#include <array>
#include <utility>
#include <fsm>

//INDECES FOR STATES
constexpr std::size_t go_west    = 0;
constexpr std::size_t slow_west  = 1;
constexpr std::size_t go_south   = 2;
constexpr std::size_t slow_south = 3;
constexpr std::size_t go_walk    = 4;
constexpr std::size_t blink_0    = 5;
constexpr std::size_t blink_1    = 6;
constexpr std::size_t blink_2    = 7;
constexpr std::size_t blink_3    = 8;
constexpr std::size_t blink_4    = 9;
constexpr std::size_t blink_5    = 10;
constexpr std::size_t blink_6    = 11;

//CONSTANTS FOR OUTPUT DATA
constexpr std::uint8_t go_west_output    = 0x4C;
constexpr std::uint8_t slow_west_output  = 0x54;
constexpr std::uint8_t go_south_output   = 0x61;
constexpr std::uint8_t slow_south_output = 0x62;
constexpr std::uint8_t go_walk_output    = 0xA4;
constexpr std::uint8_t blink_0_output    = 0x64;
constexpr std::uint8_t blink_1_output    = 0x24;
constexpr std::uint8_t blink_2_output    = 0x64;
constexpr std::uint8_t blink_3_output    = 0x24;
constexpr std::uint8_t blink_4_output    = 0x64;
constexpr std::uint8_t blink_5_output    = 0x24;
constexpr std::uint8_t blink_6_output    = 0x64;

using fsm_type = vin::moore_fsm<uint8_t, 0, 12, 3>;
using state_type = fsm_type::state_type;

//finite state machine
constexpr state_type states[12] = {
    {go_west_output,    1000, {go_west, go_west, slow_west, slow_west, slow_west, slow_west, slow_west, slow_west}},
    {slow_west_output,  1000, {go_south, go_south, go_south, go_south, go_walk, go_walk, go_south, go_south}},
    {go_south_output,   1000, {go_south, slow_south, go_south, slow_south, slow_south, slow_south, slow_south, slow_south}},
    {slow_south_output, 1000, {go_west, go_west, go_walk, go_west, go_walk, go_walk, go_walk, go_walk}},
    {go_walk_output,    1000, {go_walk, blink_0, blink_0, blink_0, go_walk, blink_0, blink_0, blink_0}},
    {blink_0_output,    500,  {blink_1, blink_1, blink_1, blink_1, blink_1, blink_1, blink_1, blink_1}},
    {blink_1_output,    500,  {blink_2, blink_2, blink_2, blink_2, blink_2, blink_2, blink_2, blink_2}},
    {blink_2_output,    500,  {blink_3, blink_3, blink_3, blink_3, blink_3, blink_3, blink_3, blink_3}},
    {blink_3_output,    500,  {blink_4, blink_4, blink_4, blink_4, blink_4, blink_4, blink_4, blink_4}},
    {blink_4_output,    500,  {blink_5, blink_5, blink_5, blink_5, blink_5, blink_5, blink_5, blink_5}},
    {blink_5_output,    500,  {blink_6, blink_6, blink_6, blink_6, blink_6, blink_6, blink_6, blink_6}},
    {blink_6_output,    500,  {go_west, go_west, go_south, go_west, go_walk, go_west, go_south, go_west}}
};

constinit fsm_type fsm{states};

//*****************************************************************************
//
// traffic light system with a moore fsm
// there are 11 states- 
//  -go_west = east/west light green, everything else red
//  -slow_west = east/west light yellow, everything else red
//  -go_south = same as go_west but lights swap
//  -slow_south = same as slow_west but lights swap
//  -go_walk = walk light green, everything else red
//  -blink_(0,2,4,6) = walk light red, everything else red
//  -other blinks, walk light off, everything else
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

    //note: output will be in the following format:
    // -bits 0-5 are PE0-5
    // -bit 6 is pf1
    // -bit 7 is pf3

    while(true) {
        std::uint8_t output = fsm.output();
        e.data(0, 1, 2, 3, 4, 5) = (output & 0b111111); //set 6 LEDS
        f.data(1, 3) = (((output & 0x40) >> 5) | ((output & 0x80) >> 4)); //move bit 6 to bit 1
                                                                          //and bit 7 to bit3
        fsm.wait();

        if(fsm.next(a.data(2, 3, 4) >> 2)) {
            e.data(0, 1, 2, 3, 4, 5) = 0b111111;
            while(true);
        }
    }
}
