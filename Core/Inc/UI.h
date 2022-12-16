/*
 * UI.h
 *
 *  Created on: Nov 15, 2021
 *      Author: damonb
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include "defs.h"
#include "controller.h"

void ProcessUserInput(struct sController* Controller, uint8_t* buffer);

void ShowControllerConfig(struct sController* Controller);
void ShowSensor(struct sController* Controller);
void FormatTemperature(char* buffer, double temp);
void ShowEffort(struct sController* Controller);
void ShowAll(struct sController* Controller, bool readable);
void ShowRawHeader(void);

extern volatile uint32_t ADCChannelSamples[4][8];

#endif /* INC_UI_H_ */
