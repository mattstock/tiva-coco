#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "ff.h"
#include "diskio.h"
#include "cocofont.h"
#include "debug.h"
#include "cocobus.h"

#define SPI_LED_BASE GPIO_PORTF_BASE
#define SPI_LED_SYSCTL SYSCTL_PERIPH_GPIOF
#define SPI_LED_BUSY GPIO_PIN_0
#define SPI_LED_ERROR GPIO_PIN_4

FATFS fatfs_obj;
DIR dir_obj;
FIL fil_obj;
FILINFO fno;
FRESULT res;

uint32_t clockFreq;

int main() {
  ROM_SysCtlPeripheralEnable(SPI_LED_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY);


  clockFreq = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);


  ROM_SysCtlPeripheralEnable(COCO_CS_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(COCO_CS_BASE, COCO_CS);
  ROM_GPIOPinWrite(COCO_CS_BASE, COCO_CS, COCO_CS);

  uart_init(clockFreq);
  disk_initialize(0);

  ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, SPI_LED_BUSY);

  if ((res = f_mount(0, &fatfs_obj))) {
    ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, SPI_LED_ERROR);
    UART_printstr("Mount failed!\r\n");
    while (1);
  }

  setAddr(0x413000);
  for (int i=0; i < 4*128; i++)
    setData(f88[i]);

  setAddr(0x420000);
  if ((res = f_open(&fil_obj, "/dod.rom", FA_READ)) != FR_OK) {
    ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, SPI_LED_ERROR);
    UART_printstr("Can't open test file.\r\n");
    while (1);    
  }

  programRom(&fil_obj);

  f_close(&fil_obj);

  ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, 0);

  while (1);
}
