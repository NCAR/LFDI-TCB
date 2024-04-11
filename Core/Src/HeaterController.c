/*
 * controller.c
 *
 *  Created on: Nov 11, 2021
 * 
 */

#include "HeaterController.h"

void HeaterController_InitStruct(struct sHeaterController* s, struct sTMP117* tmp117, uint8_t heaternumber)
{
//  struct sTMP117 TMP117;
//  TMP117_InitStruct(&TMP117, &hi2c2, 0);
  struct sPID PID;
  PID_InitStruct(&PID);

  s->HeaterNumber = heaternumber;
  s->Sensor = *tmp117;
  s->PID = PID;
  s->PwmPeriod_ms = 200;
  s->HeaterEnabled = false;
}

// this doesn't sit in the struct because we want something global that the interrupt can call
void HeaterController_SetHeater(uint8_t heater, bool state)
{
  uint16_t Pin;
  GPIO_TypeDef* Port;
  switch (heater)
  {
    case 0:
      Port = GPIOB;
      Pin = GPIO_PIN_1;
      break;
    case 1:
      Port = GPIOE;
      Pin = GPIO_PIN_8;
      break;
    case 2:
      Port = GPIOE;
      Pin = GPIO_PIN_9;
      break;
    default:
      return;
      break;
  }

  if (state)
    HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_RESET);
}


void HeaterController_Step(struct sHeaterController* Controller)
{
  uint8_t i = Controller->HeaterNumber - 1;
  float eff;

  if (Controller->HeaterEnabled)
    eff = PID_CalculateEffort(&Controller->PID, Controller->Sensor.Average);
  else
  {
    Controller->PID.Ep = -1;
    Controller->PID.Ed = -1;
    Controller->PID.Ei = -1;
    Controller->PID.Effort = 0;
  }

  if (Controller->HeaterEnabled == false)
  {
    HeaterDwell[i] = 100; // duty cycle of 0, disabled
    return;
  }
  if (Controller->Sensor.State != TMP117_STATE_VALIDTEMP)
  {
    HeaterDwell[i] = 100; // duty cycle of 0, disabled
    return;
  }

  // dwell is a value between 0 and 100 which tells the heater controller how many
  // heater ticks the heater spends off. A complete heater cycle is 200 ticks.
  // For a 40% duty cycle (effort), dwell becomes 60, making the heater off for 60,
  // on for 80, and off for another 60. This weird design causes heaters with
  // different effort values to turn on and off at different times.
  HeaterDwell[i] = 100 * (1 - eff);
}
