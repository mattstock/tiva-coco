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
#include "cocobus.h"
#include "debug.h"

void setAddr(uint32_t address) {
  uint32_t res;

  ROM_GPIOPinWrite(COCO_CS_BASE, COCO_CS, 0);
  ROM_SSIDataPut(SDC_SSI_BASE, 0x01);
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_SSIDataPut(SDC_SSI_BASE, (address >> 16));
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_SSIDataPut(SDC_SSI_BASE, (address >> 8));
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_SSIDataPut(SDC_SSI_BASE, address);
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_GPIOPinWrite(COCO_CS_BASE, COCO_CS, COCO_CS);
}

void setData(uint16_t data) {
  uint32_t res;

  ROM_GPIOPinWrite(COCO_CS_BASE, COCO_CS, 0);
  ROM_SSIDataPut(SDC_SSI_BASE, 0x03);
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_SSIDataPut(SDC_SSI_BASE, (data >> 8));
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_SSIDataPut(SDC_SSI_BASE, data);
  ROM_SSIDataGet(SDC_SSI_BASE, &res);
  ROM_GPIOPinWrite(COCO_CS_BASE, COCO_CS, COCO_CS);
}

// only 16k for the ROMs in this version
void programRom(FIL *fil_obj) {
  uint32_t addr = 0x200000; // base of coco rom mem
  FRESULT res;
  uint8_t buf[0x20];
  UINT c;
  
  setAddr(addr);
  while ((res = f_read(fil_obj, buf, 0x20, &c)) == FR_OK) {
    if (c == 0)
      break;
    
    addr += c;
    if (addr % 0x100 == 0) {
      UART_printstr("0x100\r\n");
    }      

    for (uint8_t i=0; i < c; i+=2) {
      if (i == c-1) {
        setData(buf[i] << 8);
      } else {
        setData((buf[i] << 8) | buf[i+1]);
      }
    }
  }
}


