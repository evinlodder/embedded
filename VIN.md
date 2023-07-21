# Standard Library Documentation
All of the standard library is located in the `include/drivers/` directory   
Everything else in `include/` should remain unchanged unless you have a processor-specific file like `include/tm4c123gh6pm.h` to add  
However, realistically you can do whatever you want as long as everything functions smoothly. Those are just my tips  
> NOTE: the standard library is almost completely C++ exclusive and many parts are only compatible with the TM4C123GH6PM chip
## Contributing to the Standard Library
Adding anything to the standard library (which you probably will do since it is extremely small) is pretty straight-forward  
Any header files created in the standard library directory will be automatically included in the build  
Any source files must be declared in the `driverobjects.mk` file
Any standard library improvements would be greatly appreciated and probably added!
## Standard Library Features

<details>
    <summary>Hardware register access with <code>&#60reg&#62</code></summary>
      
  - currently only supports limited registers, but adding any is extremely simple and beneficial to the growth of the library! Just make sure you guard your processor-specific addresses in an #if defined() guard
  - data() function to get access to a register (they are all defined as type std::uint32_t)
    <details>
      <summary><i>Currently Supported Registers</i></summary>
    
    > SYSCTL Registers
    > - sysctl_rcc2_r
    > - sysctl_rcc_r
    > - sysctl_ris

    > SysTick Registers
    > - systick_ctrl_r
    > - systick_reload_r
    > - systick_current_t
    </details>
    
</details>
<details>
  <summary>PLL configuration with <code>&#60pll&#62</code></summary>
  
  - PLL initialization with custom frequency (`vin::pll_init(std::uint8_t)`)
</details>
<details>
  <summary>Port initialization & GPIO Pin configuration with <code>&#60port&#62</code></summary>
  
  - create a port with `vin::port<vin::portname>`
  - initialize the port with `vin::port::initialize(const std::initializer_list<vin::clockreg>&)` (also optionally with any number of registers to set)
  - configure pins with `vin::port::init_digital_pins`
  >NOTE: see examples of <port> in use with `projects/lab10v2/lab10v2.cpp`
</details>
<details>
  <summary>C-compatible button and RGB interfaces for the TM4C on-board peripherals with <code>&#60rgb.h&#62</code> and <code>&#60buttons.h&#62</code></summary>

  - initialize on-board buttons and LED with `RGBInit(uint32_t)` and `ButtonsInit()`
  - poll buttons with `ButtonsPoll(uint32_t*, uint32_t*)`
  - change RGB color and intensity with `RGBSet(uint32_t*, float)`, `RGBColorSet(uint32_t*)`, and `RGBIntensitySet(float)`
    - the uint32_t* passed should be an array of 3 numbers from 0 - 0xFFFF where red is element 0, green 1, and blue 2
  >NOTE: visit the source files in `include/drivers` for more documentation and `projects/lab4/lab4.cpp` for example usage 
</details>
<details>
  <summary>SysTick initialization and usage with <code>&#60tick&#62</code></summary>

  - initialize SysTick with `vin::tick_init()`
  - sleep any number of ticks with `vin::sleep_ticks(std::uint32_t)`
  - sleep any number of ms with `vin::sleep(std::uint32_t)`
  >NOTE: most of the labs have example usage of the `<tick>` header
</details>
<details>
  <summary>Finite State Machine interface with <code>&#60fsm&#62</code></summary>

  - currently only moore machine interface
  - get data output with `vin::moore_fsm::output()`
  - wait current state's delay with `vin::moore_fsm::wait()`
  - set next state with `vin::moore_fsm::next(std::size_t)`
  >NOTE: visit the source files in `include/drivers` for more documentation and `projects/lab10v2/lab10v2.cpp` for example usage 
</details>
<details>
  <summary>Data dump interface with <code>&#60dump&#62</code></summary>

  - currently only able to view data through debugger
    - for most intents and purposes, this header is INCOMPLETE
  - get data output with `vin::dump::data()`
  - save data to dump with `vin::dump::save(const T&)`
  >NOTE: visit the source files in `include/drivers` for more documentation and `projects/lab9/lab9.cpp` for example usage 
</details>
<details>
  <summary>Random number generation with <code>&#60rand&#62</code></summary>

  - this header works, but needs to be re-done to conform more to standards
  - generate random numbers with `vin::rand8()`, `vin::rand16()`, `vin::rand32()`, and `vin::rand64()`
  - generator can be seeded with `vin::srand(unsigned)`
</details>
