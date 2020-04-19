#include "STP08CP05.h"

uint8_t freqToLed (float frequency)
{
    uint8_t leds;

    if (frequency < 23.50)
        leds = 0;
    else if (frequency < 36.875)
        leds = 0x1;
    else if (frequency < 73.75)
        leds = 0x3;
    else if (frequency < 187.5)
        leds = 0x7;
    else if (frequency < 375)
        leds = 0xF;
    else if (frequency < 750)
        leds = 0x1F;
    else if (frequency < 1500)
        leds = 0x3F;
    else if (frequency < 3000)
        leds = 0x7F;
    else if (frequency < 6000)
        leds = 0xFF;

    return leds;
}

void stpSpiTx(uint8_t leds)
{
    HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 0);
    DWT_Delay_us(1);
	// Transfer the bits
	for (uint8_t i = 8; i > 0 ; i--)
	{
		HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, leds & (1 << i - 1));
		DWT_Delay_us(1);
		HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 1);
		DWT_Delay_us(1);
		HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 0);
	}
	HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, 0);
    DWT_Delay_us(1);

    // Latch Enable
    HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 1);
    HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, 1);
    DWT_Delay_us(1);
    HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 0);
    HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, 0);
    DWT_Delay_us(1);

    // Clock Again
    HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 1);
	
}