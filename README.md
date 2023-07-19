<h1 align="center">
  embedded
</h1>

A few of my first embedded projects on the TM4C123G Launchpad  
All of the labs in this repo are from the [Embedded Systems- Shape The World: Microcontroller Input/Output](https://www.edx.org/course/embedded-systems-shape-the-world-microcontroller-i) course on edX from UT  
This is the beginning of my embedded journey, hopefully as time passes this repo will get bigger and bigger and others will take inspiration from my trials.  
# Installing  
## Linux (WSL2 Ubuntu)
1. download and install the the arm-none-eabi toolchain [here](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
> NOTE: the only tested version is 12.2
2. Install lm4flash and openocd
  ```bash
 sudo apt-get install lm4flash
 sudo apt-get install openocd
  ```  
3. Go to `common.mk` and set the PREFIX variable (line 51) to the location of the bin/ directory of where you downloaded the toolchain + arm-none-eabi (e.g. usr/arm-gnu-12.2/bin/arm-none-eabi)
> NOTE: if you installed lm4flash and/or openocd outside of PATH, you may also have to change their locations on lines 67-68
4. If you want to change any compiler flags, they are located on lines 87 (CFLAGS) and 129-132 (CXXFLAGS) in `common.mk`
    <details>
      <summary><i>Other flag notes...</i></summary>
      
    > - there are optional debug flags on lines 114-116  
    > - assembler flags can be changed on lines 79-82
    > - linker flags can be changed on line 104
    > - architecture flags can be changed on lines 73-74
    </details>
5. Create a new project folder in the `projects/` directory (or wherever I'm not the boss of you)  
6. Copy `makefile.ex` into your project directory and rename it to `makefile`  
>NOTE: if your project directory isn't in the `projects/` directory, you may have to change the ROOT variable on line 4
7. In `makefile`, change the PROJNAME variable, change the PART variable if necessary, add an entry point (default is ResetISR), set any project-specific flags, and add rules to build project
- Default build rules are `$(PROJNAME).o` and `$(ROOT)/gcc/startup.o` (reference my projects)
>NOTE: if you are using a TM4C123GH6PM like I am, add -DTARGET_IS_TM4C123_RB1 to SPCFLAGS to compile correctly
8. Create your C/C++ file and code away! For reference on the standard library, visit [VIN.md](VIN.md)
## Windows
`to be created...`
## MacOS
`also to be created...`
# Final Notes
If you have any questions or suggestions, please let me know! 
