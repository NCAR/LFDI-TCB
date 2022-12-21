/*
 * TuningControlBoard.h
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#ifndef INC_TUNINGCONTROLBOARD_H_
#define INC_TUNINGCONTROLBOARD_H_

#include "defs.h"
#include "TMP117.h"
#include "PID.h"
#include "DAC.h"
#include "Controller.h"




//The Tuning Control Board Contains 8 DACs and 7 TMP117 Temp Sensors as well as One Heater Controller
//Currently the TCB will be set up to Drive the DAC independant of any other input
//The TCB will be set up such that the User can choose which TMP117 to use as the input for the PID Controller along with the heater
struct sTuningControlBoard
{
  struct sController Controller;
  struct sDAC DAC8718;
  struct sTMP117 Sensor[7];

};

void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c, SPI_HandleTypeDef* hspi);


#endif /* INC_TUNINGCONTROLBOARD_H_ */
