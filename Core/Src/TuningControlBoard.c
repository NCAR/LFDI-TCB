/*
 * TungControlBoard.c
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#include "TuningControlBoard.h"
#include "Controller.h"
#include "TMP117.h"
#include "DAC.h"
#include "Compensator.h"
#include "CameraController.h"

//This Will Setup the TCB structer by initiallizing the DAC and the TMP117 and the Controller
void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c, SPI_HandleTypeDef* hspi){

    //For each of the Seven Temperature sensors initialize the struct
    for(int i = 0; i < 4; i++){
        TMP117_InitStruct(&s->TMP117[i], hi2c, i);
    }   
   
    //Init
    DAC_InitStruct(&s->DAC8718, hspi);
    
    
    //Use the First Sensor as the Input for the Controller
    Controller_InitStruct(&s->Controller,&s->TMP117[0], 0);

    //Initialize all of the Compensators
    for(int i = 0; i < 4; i++){
        Compensator_InitStruct(&s->Compensator[i], &s->TMP117[i+1], &s->DAC8718.DAC_Channels[i]);
    }
    //Initalize the CameraController
    for(int i = 0; i < 2; i++){
        CameraController_InitStruct(&s->CameraController[i], &s->DAC8718.DAC_Channels[i+6]);
    }

}
