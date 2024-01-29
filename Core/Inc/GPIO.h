#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "defs.h"




struct sGPIO{
	uint8_t GPIONum; // The Identifying Number of the GPIO
	uint16_t EnablePin; // The Enable Pin of the GPIO
	GPIO_TypeDef* EnablePort; // Th  Enable Port of the GPIO
	bool Input; //Whether the pin is an input (true) or an output (false)
	bool Enabled; //if the Pin is currently on
};


void GPIO_InitStruct(struct sGPIO* s, uint8_t GPIONum);
void GPIO_SetInput(struct sGPIO* s);
void GPIO_SetOutput(struct sGPIO* s);
void GPIO_SetState(struct sGPIO* s, bool high);


