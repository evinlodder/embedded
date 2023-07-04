//*****************************************************************************
//
// lab4.c - lab 4 for the UT course.
//
//*****************************************************************************

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <hw_memmap.h>
#include <driverlib/debug.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <hw_types.h>
#include <hw_timer.h>
#include <hw_ints.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
#include <rgb.h>
#include <vin>
#include <buttons.h>

//*****************************************************************************
//
// If both switches are off, so is LED.
// If sw1 is on, LED is red.
// If sw2 is on, LED is green.
// If both switches are on, LED is blue.
//
//*****************************************************************************

int
main(void)
{
    RGBInit(1);
    ButtonsInit();

    uint8_t buttons{};
    uint8_t changed{};
    uint32_t colors[3]{};

    while(1)
    {
        ButtonsPoll(&changed, &buttons);
        if(!changed) {
            continue;
        }

        bool left_pressed = buttons & LEFT_BUTTON;
        bool right_pressed = buttons & RIGHT_BUTTON;

        if(left_pressed && right_pressed) {
            colors[RED] =   0;
            colors[GREEN] = 0;
            colors[BLUE] =  0xFFFE;
        } else if(left_pressed) {
            colors[RED] =   0xFFFE;
            colors[GREEN] = 0;
            colors[BLUE] =  0;
        } else if(right_pressed) {
            colors[RED] =   0;
            colors[GREEN] = 0xFFFE;
            colors[BLUE] =  0;
        } else {
            colors[0] = 0;
            colors[1] = 0;
            colors[2] = 0;
        }
        RGBColorSet(colors);
    }
}
