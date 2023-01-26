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
#include "Compensator.h"
#include "CameraController.h"



//The Tuning Control Board Contains 8 DACs and 7 TMP117 Temp Sensors as well as One Heater Controller
//Currently the TCB will be set up to Drive the DAC independant of any other input
//The TCB will be set up such that the User can choose which TMP117 to use as the input for the PID Controller along with the heater
struct sTuningControlBoard
{
	struct sTMP117 TMP117[4]; //
	struct sDAC DAC8718; //8 DAC Channels

	//Implementations of the Toplevel Functionalities
	struct sController Controller; //a Heater and Temp Sensor Pair
	struct sCompensator Compensator[3]; //a DAC and Temp Sensor pair
	struct sCameraController CameraController[2];//a DAC

};

void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c, SPI_HandleTypeDef* hspi);


#endif /* INC_TUNINGCONTROLBOARD_H_ */
