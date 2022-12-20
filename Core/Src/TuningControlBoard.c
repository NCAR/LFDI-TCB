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



//This Will Setup the TCB structer by initiallizing the DAC and the TMP117 and the Controller
void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c, SPI_HandleTypeDef* hspi){

    //For each of the Seven Temperature sensors initialize the struct
    for(int i = 0; i < 4; i++){
        TMP117_initStruct(&s->Sensor[i], hi2c, i);
    }   
    Controller_InitStruct(&s->Controller,&s->Sensor[0], 0);
    DAC_InitStruct(s->DAC8718, hspi);

}
