#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "ff.h"
#include "diskio.h"
#include "cocofont.h"

#define SPI_LED_BASE GPIO_PORTF_BASE
#define SPI_LED_SYSCTL SYSCTL_PERIPH_GPIOF
#define SPI_LED_BUSY GPIO_PIN_0
#define SPI_LED_ERROR GPIO_PIN_4
#define COCO_CS_SYSCTL SYSCTL_PERIPH_GPIOC
#define COCO_CS_BASE GPIO_PORTC_BASE
#define COCO_CS GPIO_PIN_6

FATFS fatfs_obj;
DIR dir_obj;
FIL fil_obj;
FILINFO fno;
FRESULT res;

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

uint32_t clockFreq;

void uart_init() {

  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  //
  // Configure the UART for 115,200, 8-N-1 operation.
  //
  ROM_UARTConfigSetExpClk(UART0_BASE, clockFreq, 115200,
                          (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                           UART_CONFIG_PAR_NONE));
}

void UART_printstr(char *str) {
  uint16_t i=0;

  while (str[i] != '\0') {
    ROM_UARTCharPut(UART0_BASE, str[i++]);
  }
}

void UART_printchars(char *str, uint16_t count) {
  for (uint16_t i=0; i < count; i++)
    ROM_UARTCharPut(UART0_BASE, str[i]);
}

void UART_print_dirent() {
  UART_printstr("File name: "); 
  for (uint8_t i=0; i < 13; i++) {
    ROM_UARTCharPut(UART0_BASE, fno.fname[i]);
  }
  UART_printstr("\r\n");

}

int main() {
  UINT c;
  char buf[20];

  ROM_SysCtlPeripheralEnable(SPI_LED_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY);


  clockFreq = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);


  ROM_SysCtlPeripheralEnable(COCO_CS_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(COCO_CS_BASE, COCO_CS);
  ROM_GPIOPinWrite(COCO_CS_BASE, COCO_CS, COCO_CS);

  uart_init();
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
  if ((res = f_open(&fil_obj, "/test.txt", FA_READ)) != FR_OK) {
    ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, SPI_LED_ERROR);
    UART_printstr("Can't open test file.\r\n");
    while (1);    
  }

  while ((res = f_read(&fil_obj, buf, 20, &c)) == FR_OK) {
    if (c == 0)
      break;
    for (uint8_t i=0; i < c; i+=2) {
      if (i == c-1) {
        setData(buf[i] << 8);
      } else {
        setData((buf[i] << 8) | buf[i+1]);
      }
    }
    UART_printchars(buf, c);
  }

  f_close(&fil_obj);

  ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, 0);

  while (1);
}
