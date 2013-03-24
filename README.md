Stellaris Template
==================

## Toolchain

### Dependencies:

* flex
* bison
* libgmp3-dev
* libmpfr-dev
* libncurses5-dev
* libmpc-dev
* autoconf
* texinfo
* libftdi-dev
* python-yaml
* zlib1g-dev

To get all dependencies on Ubuntu:

    apt-get install flex bison libgmp3-dev libmpfr-dev libncurses5-dev \
    libmpc-dev autoconf texinfo build-essential libftdi-dev python-yaml \
    zlib1g-dev

You will need an ARM bare-metal toolchain to build code for Stellaris targets.
You can get a toolchain from the
[gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded) project that is
pre-built for your platform. Extract the package and add the `bin` folder to
your PATH.

Alternatively, you can use
[summon-arm-toolchain](https://github.com/esden/summon-arm-toolchain).

Once you've installed the ARM toolchain, add the binaries to your path.

The StellarisWare package contains all of the header files and drivers for
Stellaris parts. Grab the file *SW-EK-LM4F12XL-9353.exe* from
[here](http://www.ti.com/tool/sw-ek-lm4f120xl) and unzip it into a directory.

    unzip SW-EK-LM4F120XL-9453.exe

If you are using gcc-arm-embedded, you must edit the makedefs file in the
extracted StellarisWare directory to point at the correct compiler. For version 
4.7.3, change the line:

    CC=${PREFIX}-gcc

to:

    CC=${PREFIX}-gcc-4.7.3

then run `make` to build StellarisWare.

## Writing and Building Firmware
**NOTE:** This is only tested with the LM4F120H5QR, which is the target on
the Stellaris Launchpad.

1. Clone the
   [stellaris-template](https://github.com/uctools/stellaris-templates)
   repository (or fork it and clone your own repository).

        git clone git@github.com:uctools/stellaris-template

2. Modify the Makefile:
    * Set TARGET to the desired name of the output file (eg: TARGET = main)
    * Set SOURCES to a list of your sources (eg: SOURCES = main.c
      startup\_gcc.c)
    * Set STELLARISWARE\_PATH to the full path to where you extracted and built
      StellarisWare (eg: STELLARISWARE_PATH = /home/eric/code/stellarisware)

3. Run `make`

4. The output files will be created in the 'build' folder

## Flashing

The easiest way to flash your device is using lm4flash. First, grab lm4tools
from Git.

    git clone git://github.com/utzig/lm4tools.git

Then build lm4flash and run it:

    cd lm4tools/lm4flash
    make
    lm4flash /path/to/executable.bin

## Credits

Thanks to Recursive Labs for their
[guide](http://recursive-labs.com/blog/2012/10/28/stellaris-launchpad-gnu-linux-getting-started/)
which this template is based on.
