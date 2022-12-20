/*
 * controller.h
 *
 *  Created on: Nov 11, 2021
 *      Author: damonb
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "defs.h"
#include "TMP117.h"
#include "PID.h"
#include "DAC.h"

extern volatile uint8_t HeaterDwell;

struct sController
{
  uint8_t Heater;
  struct sTMP117 Sensor;
  struct sPID PID;
};

void Controller_InitStruct(struct sController* s, struct sTMP117 *t,uint8_t heater);
void Controller_SetHeater(uint8_t heater, bool state);
void Controller_Step(struct sController* C);

void Controller_LoadConfig(struct sController* Controller);
void Controller_SaveConfig(struct sController* Controller);
void Controller_WipeConfig(struct sController* Controller);





#endif /* INC_CONTROLLER_H_ */
