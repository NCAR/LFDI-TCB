#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "defs.h"

//Heater1 Definitions
#define HEATER_1_ENABLE_PORT GPIOB
#define HEATER_1_ENABLE_PIN GPIO_PIN_1
#define HEATER_1_READ_PIN
#define HEATER_1_READ_PORT

//Heater2 definitions
#define HEATER_2_ENABLE_PORT GPIOE
#define HEATER_2_ENABLE_PIN GPIO_PIN_8
#define HEATER_2_READ_PIN
#define HEATER_2_READ_PORT

//Heater2 definitions
#define HEATER_3_ENABLE_PORT GPIOE
#define HEATER_3_ENABLE_PIN GPIO_PIN_9
#define HEATER_3_READ_PIN
#define HEATER_3_READ_PORT


struct sHeater
{
  uint8_t HeaterNum;
  uint16_t EnablePin;
  GPIO_TypeDef* EnablePort;
  uint16_t ReadPin;
  GPIO_TypeDef* ReadPort;
  bool Enabled;
};

void Heater_InitStruct(struct sHeater* s, uint8_t heater);

void set_HeaterEnable(struct sHeater* s, bool Heater_On);

