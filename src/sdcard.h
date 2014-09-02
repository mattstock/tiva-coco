#ifndef __SDCARD_H
#define __SDCARD_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"

#define SPI_BASE SSI2_BASE
#define SPI_SYSCTL SYSCTL_PERIPH_SSI2
#define SPI_SYSCTL_PORT SYSCTL_PERIPH_GPIOD
#define SPI_GPIO_BASE GPIO_PORTD_BASE
#define SPI_TX GPIO_PIN_1
#define SPI_RX GPIO_PIN_0
#define SPI_CLK GPIO_PIN_3
#define SPI_TX_MUX GPIO_PD1_SSI2XDAT0
#define SPI_RX_MUX GPIO_PD0_SSI2XDAT1
#define SPI_CLK_MUX GPIO_PD3_SSI2CLK
#define COCO_CS_SYSCTL SYSCTL_PERIPH_GPIOC
#define COCO_CS_BASE GPIO_PORTC_BASE
#define COCO_CS GPIO_PIN_6
#define SSD_CS_SYSCTL SYSCTL_PERIPH_GPIOC
#define SSD_CS_BASE GPIO_PORTC_BASE
#define SSD_CS GPIO_PIN_7

#define SPI_LED_BASE GPIO_PORTF_BASE
#define SPI_LED_SYSCTL SYSCTL_PERIPH_GPIOF
#define SPI_LED_BUSY GPIO_PIN_0
#define SPI_LED_ERROR GPIO_PIN_4

/* Definitions for MMC/SDC command */
#define CMD0    (0x40+0)    /* GO_IDLE_STATE */
#define CMD1    (0x40+1)    /* SEND_OP_COND */
#define CMD8    (0x40+8)    /* SEND_IF_COND */
#define CMD9    (0x40+9)    /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */

#endif

