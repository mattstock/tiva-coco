# Stellaris Makefile
# #####################################
#
# Part of the uCtools project
# uctools.github.com
#
#######################################
# user configuration:
#######################################
# TARGET: name of the output file
TARGET = main
# MCU: part number to build for
MCU = msp430g2231
# SOURCES: list of input source sources
SOURCES = main.c startup_gcc.c
# INCLUDES: list of includes, by default, use Includes directory
INCLUDES = -IInclude
# OUTDIR: directory to use for output
OUTDIR = build
# STELLARISWARE_PATH: path to stellarisware folder
STELLARISWARE_PATH = /home/eric/code/stellarisware
# define flags
CFLAGS = 	-g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
			-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall 		\
			-pedantic -DPART_LM4F120H5QR -c -I$(STELLARISWARE_PATH) \
			-DTARGET_IS_BLIZZARD_RA1
LDFLAGS = -T src/lm4f120h5qr.ld --entry ResetISR --gc-sections
#######################################
# end of user configuration
#######################################
#
#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
RM      = rm -f
MKDIR	= mkdir -p
#######################################

# list all object files
OBJECTS = $(addprefix $(OUTDIR)/,$(SOURCES:.c=.o))

# default: build bin
all: $(OUTDIR)/$(TARGET).bin

$(OUTDIR)/%.o: src/%.c | $(OUTDIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OUTDIR)/a.out: $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OUTDIR)/main.bin: $(OUTDIR)/a.out
	$(OBJCOPY) -O binary $< $@

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)

clean:
	-$(RM) $(OUTDIR)/*
