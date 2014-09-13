#ifndef _COCOBUS_H
#define _COCOBUS_H

#include <stdbool.h>
#include <stdint.h>
#include "ff.h"

#define COCO_CS_SYSCTL SYSCTL_PERIPH_GPIOC
#define COCO_CS_BASE GPIO_PORTC_BASE
#define COCO_CS GPIO_PIN_6

void setAddr(uint32_t address);
void setData(uint16_t data);
void programRom(FIL *fil_obj);

#endif

