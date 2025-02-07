/*
 * TuningControlBoard.h
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#ifndef INC_TUNINGCONTROLBOARD_H_
#define INC_TUNINGCONTROLBOARD_H_
#define NUMOFHEATERCONTROLLERS (3)
#define NUMOFCOMPENSATORS (3)
#define NUMOFSENSORS (8)
#define NUMOFSENSORSBUS1 (3)
#define NUMOFSENSORSBUS2 (0)
#define NUMOFGPIO (5)
#define NUMOFBipolarOutputs (2)



#include "defs.h"
#include "StageDefinitions.h"
#include "TMP117.h"
#include "PID.h"
#include "DAC.h"
#include "HeaterController.h"
#include "DAC.h"
#include "Compensator.h"
#include "GPIO.h"
#include "BipolarOutput.h"
#include "CameraController.h"



//The Tuning Control Board Contains 8 DACs and 7 TMP117 Temp Sensors as well as One Heater Controller
//Currently the TCB will be set up to Drive the DAC independent of any other input
//The TCB will be set up such that the User can choose which TMP117 to use as the input for the PID Controller along with the heater
struct sTuningControlBoard
{
  struct sDAC DAC8718;
  struct sHeaterController HeaterControllers[NUMOFHEATERCONTROLLERS]; //Increase this as we get more controllers
  struct sTMP117 Sensor[NUMOFSENSORS];
  struct sCompensator Compensator[6];
  struct sGPIO GPIO[NUMOFGPIO];
  struct sBipolarOutput BipolarOutput[NUMOFBipolarOutputs];

};

void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c1, I2C_HandleTypeDef* hi2c2, SPI_HandleTypeDef* hspi);


#endif /* INC_TUNINGCONTROLBOARD_H_ */
