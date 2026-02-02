/*
 * CurrentSensor.h
 *
 *  Created on: Jan 3, 2026
 *      Author: iguser
 */

#ifndef INC_CURRENTSENSOR_H_
#define INC_CURRENTSENSOR_H_


#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include "defs.h"

extern ADC_HandleTypeDef hadc1;
#define NUMOFCurrentSensors (3)
#define ADC_TIMEOUT_MS  10
#define ADC_MAX_12BIT   4095.0f
#define VREF 3.3f
#define RSHUNT 0.1f

#define CURRENTSENSOR_STATE_UNKNOWN (0)
#define CURRENTSENSOR_STATE_READFAIL (1)
#define CURRENTSENSOR_STATE_VALIDCURRENT (2)

struct sCurrentSensor
{
  ADC_HandleTypeDef* Interface;   // ADC handle (e.g. &hadc1)
  uint8_t Channel;                // ADC channel/rank for this sensor
  float Currents[80];             // Array of current samples (80 samples × 130ms = 10.4 seconds)
  float Current;                  // Averaged current value
  uint8_t SamplesInAverage;
  uint8_t State;
  uint16_t Errors;
};

void CurrentSensor_InitStruct(struct sCurrentSensor* s, ADC_HandleTypeDef* interface, uint8_t channel);
void CurrentSensor_GetCurrent(struct sCurrentSensor* s);

#endif /* INC_CURRENTSENSOR_H_ */
