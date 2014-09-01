# Tiva Makefile
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
MCU = TM4C1294NCPDT
# SOURCES: list of input source sources
SOURCES = main.c ff.c diskio.c startup_gcc.c
# INCLUDES: list of includes, by default, use Includes directory
INCLUDES = -IInclude
# OUTDIR: directory to use for output
OUTDIR = build
# TIVAWARE_PATH: path to tivaware folder
TIVAWARE_PATH = /home/mstock/projects/tivaware

# LD_SCRIPT: linker script
LD_SCRIPT = $(MCU).ld

LIBS = -ldriver

# define flags
CFLAGS = -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS +=-Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall
CFLAGS += -pedantic -DPART_$(MCU) -c -I$(TIVAWARE_PATH)
CFLAGS += -DTARGET_IS_TM4C129_RA0
LDFLAGS = -T $(LD_SCRIPT) -L$(TIVAWARE_PATH)/driverlib/gcc --entry ResetISR --gc-sections $(LIBS)

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

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:.c=.o)))

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

install: $(OUTDIR)/$(TARGET).bin
	../lm4tools/lm4flash/lm4flash $(OUTDIR)/$(TARGET).bin

.PHONY: all clean
