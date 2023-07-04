#******************************************************************************
#
# common.mk - Definitions common to all makefiles.
#
# Copyright (c) 2005-2017 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
#   Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the
#   distribution.
#
#   Neither the name of Texas Instruments Incorporated nor the names of
#   its contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# This is part of revision 2.1.4.178 of the Tiva Firmware Development Package.
#
#******************************************************************************

#******************************************************************************
#
# Modified by Zee Lv
# Nov 6, 2018
#
#******************************************************************************

#******************************************************************************
#
# Modified further by Bobobo34
# July 2, 2023
#
#******************************************************************************

#
# STANDARD PROGRAMS
#
PREFIX=~/arm-gnu-12.2/bin/arm-none-eabi
IPATH=$(ROOT)include/

CC=$(PREFIX)-gcc
CXX=$(PREFIX)-g++
GDB=$(PREFIX)-gdb
AR=$(PREFIX)-ar
LD=$(PREFIX)-ld
OBJCOPY=$(PREFIX)-objcopy

#
# SET TO FALSE IF COMPILING FOR RELEASE
#
DEBUG=true
PART=TM4C123GH6PM

LM4FLASH=lm4flash
OPENOCD=openocd

#
# ARCHITECTURE FLAGS
#
CPU=-mcpu=cortex-m4
FPU=-mfpu=fpv4-sp-d16 -mfloat-abi=hard

#
# ASSEMBLER FLAGS
#
AFLAGS=-mthumb \
       $(CPU) \
       $(FPU) \
       -MD

#
# COMPILER FLAGS
#
CFLAGS=-mthumb \
       $(CPU) \
       $(FPU) \
       -ffunction-sections \
       -fdata-sections \
       -MD \
       -std=c99 \
       -Wall \
       -pedantic \
       -DPART_$(PART) \
       -c \
       -specs=nano.specs \
       -specs=nosys.specs

#
# LD FLAGS
#
LDFLAGS=--gc-sections

#
# COMMON LIBS
#
LIBGCC:=$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
LIBC:=$(shell $(CC) $(CFLAGS) -print-file-name=libc.a)
LIBM:=$(shell $(CC) $(CFLAGS) -print-file-name=libm.a)

ifeq ($(DEBUG), true)
CFLAGS+=-g
CFLAGS+=-O0
else
CFLAGS+=-O2
endif

CFLAGS+=$(SPCFLAGS)

AFLAGS+=-I$(IPATH)
CFLAGS+=-I$(IPATH)

AFLAGS+=-I$(IPATH)/drivers/
CFLAGS+=-I$(IPATH)/drivers/

CXXFLAGS=$(CFLAGS)
CXXFLAGS+=-fno-exceptions
CXXFLAGS+=-fno-rtti

LIBS=$(ROOT)lib/libtm4cdrivers.a $(ROOT)lib/libdriver.a

#
# DRIVER OBJ FILES
#

P=$(ROOT)include/drivers
include $(ROOT)/driverobjects.mk

cleandrivers:
	rm -rf $(ROOT)include/drivers/*.[o,d] $(ROOT)lib/libtm4cdrivers.a

drivers: cleandrivers $(DOBJF)
	ar rcs $(ROOT)lib/libtm4cdrivers.a $(filter %.o, $^)

$(PROJNAME).axf: drivers
	$(LD) \
	-T $(LINKSCRIPT) \
	-e $(ENTRYPT) \
	$(LDFLAGS) \
	-o $@ \
	$(filter %.o %.a, $^) \
	$(LIBS) \
	'$(LIBM)' '$(LIBC)' '$(LIBGCC)'

$(PROJNAME).bin: $(PROJNAME).axf
	$(OBJCOPY) -O binary ${@:.bin=.axf} $@

include $(ROOT)/password.mk

flash: clean $(PROJNAME).bin
	# I'm aware this is piss poor practice, but it's annoying me that I can't use nvim commands and idk another workaround
	#
	echo $(PASSWORD) | sudo -S $(LM4FLASH) $(PROJNAME).bin

clean:
	rm -rf $(PROJNAME).[d,o] $(PROJNAME).axf $(PROJNAME).bin
