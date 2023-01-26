/*
 * CameraController.h
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#ifndef INC_CAMERACONTROLLER_H_
#define INC_CAMERACONTROLLER_H_

#include "defs.h"
#include "DAC.h"

struct sCameraController
{
    struct sDAC_Channel *pDAC;
};

//Initalizre the Camera Controller
void CameraController_InitStruct(struct sCameraController *pCameraController, struct sDAC_Channel *pDAC);
void CameraController_TriggerCamera(struct sCameraController *pCameraController);

#endif /* INC_COMPENSATOR_H_*/