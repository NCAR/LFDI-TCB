/*
 * TungControlBoard.c
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#include "TuningControlBoard.h"


//This Will Setup the TCB structer by initiallizing the DAC and the TMP117 and the Controller
void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c, SPI_HandleTypeDef* hspi){

	s->NumOfControllers = 1;
	s->NumOfCompensators = 6;
    //For each of the Seven Temperature sensors initialize the struct
    for(int i = 0; i < 4; i++){
        TMP117_InitStruct(&s->Sensor[i], hi2c, i);
    }
    //Initalize the DAC
    DAC_InitStruct(&s->DAC8718, hspi);   
    
    //Initalize the Controller
    for (int i = 0; i < 1; i++){
        Controller_InitStruct(&s->Controller[i], &s->Sensor[0], 0);
    }
    //Initalize the Compensator
    //Todo this will need to have different sensors
    for(int i = 0; i < 3; i++){
        Compensator_InitStruct(&s->Compensator[i], &s->Sensor[0], &s->DAC8718.DAC_Channels[i]);
    }
    for(int i = 3; i < 6; i++){
        Compensator_InitStruct(&s->Compensator[i], &s->Sensor[0], &s->DAC8718.DAC_Channels[i]);
    }

}
