PART=TM4C123GH6PM
SPCFLAGS=-DTARGET_IS_TM4C123_RB1
include common.mk

library: include/drivers/buttons.o include/drivers/rgb.o
	ar rcs drivers.a include/drivers/buttons.o include/drivers/rgb.o
startup: gcc/startup.o
