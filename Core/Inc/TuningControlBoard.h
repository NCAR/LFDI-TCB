/*
 * TunungControlBoard.h
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "defs.h"
#include "TMP117.h"
#include "PID.h"
#include "DAC.h"
#include "Heater_Controller.h"
#include "Controller.h"



//The Tuning Control Board Contains 8 DACs and 7 TMP117 Temp Sensors as well as One Heater Controller
//Currently the TCB will be set up to Drive the DAC independant of any other input
//The TCB will be set up such that the User can choose which TMP117 to use as the input for the PID Controller along with the heater
struct sTuningControlBoard
{
  struct sController Controller;
  struct sDAC DAC;
  struct sTMP117 Sensor[7];

};

void TCB_InitStruct(struct sController* s, struct sTMP117** Sensor, struct sDAC* DAC);


#endif /* INC_TUNINGCONTROLBOARD_H_ */
