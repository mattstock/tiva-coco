/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sdcard.h"



/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

static void xmit_spi(BYTEC dat) {
	DWORD rcvdat;

  ROM_SSIDataPut(SPI_BASE, dat); /* Write the data to the tx fifo */
  ROM_SSIDataGet(SPIBASE, &rcvdat); /* flush data read during the write */
}


/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/

static BYTEC rcvr_spi (void) {
  DWORD rcvdat;

  ROM_SSIDataPut(SDC_SSI_BASE, 0xFF); /* write dummy data */
  ROM_SSIDataGet(SDC_SSI_BASE, &rcvdat); /* read data frm rx fifo */
  return (BYTEC)rcvdat;
}

/*-----------------------------------------------------------------------*/
/* Send 80 or so clock transitions with CS and DI held high. This is     */
/* required after card power up to get it into SPI mode                  */
/*-----------------------------------------------------------------------*/
static void send_initial_clock_train(void) {
  unsigned int i;
  DWORD dat;

  /* Ensure CS is held high. */
	ROM_GPIOPinWrite(CS_BASE, CS_PIN, CS_PIN);

  /* Switch the SSI TX line to a GPIO and drive it high too. */
  ROM_GPIOPinTypeGPIOOutput(SPI_GPIO_BASE, SPI_TX);
  ROM_GPIOPinWrite(SPI_GPIO_BASE, SPI_TX, SPI_TX);

  /* Send 10 bytes over the SSI. This causes the clock to wiggle the */
  /* required number of times. */
  for(i = 0 ; i < 10 ; i++) {
    /* Write DUMMY data. SSIDataPut() waits until there is room in the */
    /* FIFO. */
    ROM_SSIDataPut(SPI_BASE, 0xFF);
    /* Flush data read during data write. */
    ROM_SSIDataGet(SPI_BASE, &dat);
  }

  /* Revert to hardware control of the SSI TX line. */
  ROM_GPIOPinTypeSSI(SPI_GPIO_BASE, SPI_TX);
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number (0..) */
) {
  DSTATUS stat;
  int result;
  
  if (pdrv) return STA_NOINIT;

  ROM_SysCtlPeripheralEnable(SPI_SYSCTL);
  ROM_SysCtlPeripheralEnable(SPI_SYSCTL_PORT);
  ROM_GPIOPinConfigure(SPI_RX_MUX); // rx
  ROM_GPIOPinConfigure(SPI_TX_MUX); // tx
  ROM_GPIOPinConfigure(SPI_CLK_MUX); // clk
  ROM_GPIOPinTypeSSI(SPI_GPIO_BASE, SPI_RX | SPI_TX | SPI_CLK);
  ROM_SSIConfigSetExpClk(SPI_BASE, 120000000, SSI_FRF_MOTO_MODE_0,
		     SSI_MODE_MASTER, 1000000, 8);
  ROM_SSIEnable(SPI_BASE);

  ROM_SysCtlPeripheralEnable(SSD_CS_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(SSD_CS_BASE, SSD_CS);
	ROM_GPIOPinWrite(SSD_CS_BASE, SSD_CS, SSD_CS);

	// Status LEDs  
  ROM_SysCtlPeripheralEnable(SPI_LED_SYSCTL);
  ROM_GPIOPinTypeGPIOOutput(SPI_LED_BASE, SPI_LED_BUSY|SPI_LED_ERROR);
  ROM_GPIOPinWrite(SPI_LED_BASE, SPI_LED_ERROR|SPI_LED_BUSY, SPI_LED_BUSY);

  /* Set DI and CS high and apply more than 74 pulses to SCLK for the card */
  /* to be able to accept a native command. */
  send_initial_clock_train();
	
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
) {
  DSTATUS stat;
  int result;

  if (pdrv) return STA_NOINIT;

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
) {
  DRESULT res;
  int result;

  if (pdrv) return STA_NOINIT;

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
) {
  DRESULT res;
  int result;

  if (pdrv) return STA_NOINIT;

	return STA_NOINIT;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
) {
  DRESULT res;
  int result;

  if (pdrv) return STA_NOINIT;

	return STA_NOINIT;
}
#endif
