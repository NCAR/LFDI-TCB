/*
 * controller.c
 *
 *  Created on: Nov 11, 2021
 *      Author: damonb
 */

#include "Controller.h"




void Controller_InitStruct(struct sController* s, struct sTMP117 *t,uint8_t heater){
  struct sPID PID;
  PID_InitStruct(&PID);

  s->Heater = heater;
  s->Sensor = *t;
  s->PID = PID;
}

// this doesn't sit in the struct because we want something global that the interrupt can call
//This Has been Edited so that it can be used in the TCB
void Controller_SetHeater(uint8_t heater, bool state)
{
  //For the TCB
  uint16_t Pin;
  GPIO_TypeDef* Port;
  Port = GPIOC;
  Pin = GPIO_PIN_5;
  
  if (state)
    HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(Port, Pin, GPIO_PIN_RESET);

}

void Controller_WipeConfig(struct sController* Controller)
{

  Controller_InitStruct(Controller, &Controller->Sensor, 0);
  printf("Cannot Save No EEPROM");
}



void Controller_Step(struct sController* Controller)
{
  uint8_t i = Controller->Heater - 1;
  float temp, eff;
  if (Controller->PID.Config.Enabled == false)
  {
    HeaterDwell = 100; // duty cycle of 0, disabled
    return;
  }
  if (Controller->Sensor.State != TMP117_STATE_VALIDTEMP)
  {
    HeaterDwell = 100; // duty cycle of 0, disabled
    return;
  }

  temp = Controller->Sensor.Average;
  eff = PID_CalculateEffort(&Controller->PID, temp);

  // dwell is a value between 0 and 100 which tells the heater controller how many
  // heater ticks the heater spends off. A complete heater cycle is 200 ticks.
  // For a 40% duty cycle (effort), dwell becomes 60, making the heater off for 60,
  // on for 80, and off for another 60. This weird design causes heaters with
  // different effort values to turn on and off at different times.
  HeaterDwell = 100 * (1 - eff);
}
