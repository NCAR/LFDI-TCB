/*
 * TungControlBoard.c
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#include "TuningControlBoard.h"



//This Will Setup the TCB structer by initiallizing the DAC and the TMP117 and the Controller
void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c, SPI_HandleTypeDef* hspi){

    //For each of the Seven Temperature sensors initialize the struct
    for(int i = 0; i < 4; i++){
        TMP117_InitStruct(&s->Sensor[i], hi2c, i);
    }
    //Initalize the DAC
    DAC_InitStruct(&s->DAC8718, hspi);   
    
    //Initalize the Controller
    Controller_InitStruct(&s->Controller,&s->Sensor[0], 0);
    //Initalize the Compensator
    for(int i = 0; i < 3; i++){
        Compensator_InitStruct(&s->Compensator[i], &s->Sensor[i], &s->DAC8718.DAC_Channels[i]);
    }

}
