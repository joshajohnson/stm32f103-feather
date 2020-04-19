#ifndef STP08CP05_H_
#define STP08CP05_H_

#include <stdint.h>
#include "main.h"
#include "dwt_stm32_delay.h"

uint8_t freqToLed (float frequency);
void stpSpiTx (uint8_t leds);

#endif