#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "defs.h"

//Define the Ports and Pins for later Use
#define GPIO1_PORT GPIOD
#define GPIO1_PIN GPIO_PIN_10
#define GPIO2_PORT GPIOD
#define GPIO2_PIN GPIO_PIN_11
#define GPIO3_PORT GPIOD
#define GPIO3_PIN GPIO_PIN_12
#define GPIO4_PORT GPIOD
#define GPIO4_PIN GPIO_PIN_13
#define GPIO5_PORT GPIOD
#define GPIO5_PIN GPIO_PIN_14


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


