//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with <viral> open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED <AS IS> AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
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
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************

int
main(void)
{
    volatile uint32_t ui32Loop;

    volatile uint32_t colors[3];

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    vin::tick_init();

    RGBInit(1);
    RGBIntensitySet(1.0);

    ButtonsInit();

    uint8_t changed = 0;
    uint8_t buttons = 0;

    //
    // Loop forever.
    //
    while(1)
    {
        ButtonsPoll(&changed, &buttons);

        if(buttons & LEFT_BUTTON) {
            colors[0] = 0xFFFE;
            colors[1] = 0xFFFE;
            colors[2] = 0xFFFE;
        }
        else if(buttons & RIGHT_BUTTON) {
            colors[0] = 0;
            colors[1] = 0;
            colors[2] = 0;
        }
        else {
            colors[0] = vin::rand32() % 0xFFFF;
            colors[1] = vin::rand32() % 0xFFFF;
            colors[2] = vin::rand32() % 0xFFFF;
        }

        //
        // Turn on the LED and change the color.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        RGBColorSet(colors);

        vin::sleep(200);
    }
}
