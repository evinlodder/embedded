include common.mk

library: include/drivers/buttons.o include/drivers/rgb.o
	ar rcs drivers.a include/drivers/buttons.o include/drivers/rgb.o
startup: gcc/startup.o
