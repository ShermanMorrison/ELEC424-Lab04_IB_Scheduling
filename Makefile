# Makefile for Lab03-Blinky, ELEC424 Fall 2014
# Authors: Jie Liao, Abeer Javed, Steven Arroyo. Rice University 
# Derived from the crazyflie-firmware Makefile

# Modified by Chase E Stewart and Jonathan Wilson


# Path Definitions

# The home path 
COMMON =..

# Branches off of the home path
STM_LIB = crazyflie-firmware/lib/STM32F10x_StdPeriph_Driver
CM3_FIRM = crazyflie-firmware/lib/CMSIS/Core/CM3
CM3_STM = STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
BLINKY = ELEC424-Lab04_IB_Scheduling

# Additional directory shortcuts 
STARTUP_FIRM = startup/gcc
STARTUP_STM = startup/gcc_ride7
CONFIG = crazyflie-firmware/config
LINKER_SCRIPT = linker_script

# Compiler 
CC = arm-none-eabi-gcc

# Particular processor
PROCESSOR = -mcpu=cortex-m3 -mthumb

# Directories of used header files
INCLUDE = -I  $(COMMON)/$(STM_LIB)/inc 
INCLUDE += -I $(COMMON)/$(CM3_FIRM) 
INCLUDE += -I $(COMMON)/$(CM3_FIRM)/$(STARTUP_FIRM) 
INCLUDE += -I $(COMMON)/$(BLINKY)/inc

# C Files included for this function
CFILES = $(COMMON)/$(STM_LIB)/src/stm32f10x_gpio.c
CFILES += $(COMMON)/$(STM_LIB)/src/stm32f10x_rcc.c
CFILES += $(COMMON)/$(STM_LIB)/src/stm32f10x_tim.c
CFILES += $(COMMON)/$(STM_LIB)/src/misc.c
CFILES += $(COMMON)/$(BLINKY)/src/blinky.c
CFILES += $(COMMON)/$(BLINKY)/src/sys_clk_init.c

# STM chip specific flags
STFLAGS = -DSTM32F10X_MD -include $(COMMON)/$(BLINKY)/inc/stm32f10x_conf.h

# Define the compiler flags
CFLAGS = -O0 -g3 $(PROCESSOR) $(INCLUDE) $(STFLAGS) -Wl,--gc-sections -T $(COMMON)/$(BLINKY)/$(LINKER_SCRIPT)/stm32_flash.ld

# Build all relevant files and create .elf
all:
	@$(CC) $(CFILES) $(CFLAGS) $(COMMON)/$(CM3_STM)/$(STARTUP_STM)/startup_stm32f10x_md.s -o bin/blinky.elf 

# Program .elf into Crazyflie flash memory via the busblaster
flash:
	@openocd -d0 -f interface/busblaster.cfg -f target/stm32f1x.cfg -c init -c targets -c "reset halt" -c "flash write_image erase bin/blinky.elf" -c "verify_image bin/blinky.elf" -c "reset run"


# Runs OpenOCD, opens GDB terminal, and establishes connection with Crazyflie
debug:
	@openocd -g -d0 -f interface/busblaster.cfg -f target/stm32f1x.cfg -c init -c targets -c "reset halt" & arm-none-eabi-gdb -e "blinky_fast.elf" -x targetkillall openocd


# Remove all files generated by target 'all'
clean:
	rm -f $(COMMON)/$(BLINKY)/bin/*.elf $(COMMON)/$(BLINKY)/bin/*.o
