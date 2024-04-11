/*
 * controller.h
 *
 *  Created on: Nov 11, 2021
 * 
 */

#ifndef INC_HEATERCONTROLLER_H_
#define INC_HEATERCONTROLLER_H_

#include "defs.h"
#include "TMP117.h"
#include "PID.h"
#include "Heater.h"

#define NUMOFHEATERCONTROLLERS (3)

extern volatile uint8_t HeaterDwell[NUMOFHEATERCONTROLLERS];

struct sHeaterController
{
  uint8_t HeaterNumber;
  bool HeaterEnabled;
  struct sTMP117 Sensor;
  struct sPID PID;
  uint16_t PwmPeriod_ms;
};

void HeaterController_InitStruct(struct sHeaterController* s, struct sTMP117* tmp117, uint8_t heaternumber);
void HeaterController_SetHeater(uint8_t heater, bool state);
void HeaterController_Step(struct sHeaterController* Controller);

#endif /* INC_HEATERCONTROLLER_H_ */
