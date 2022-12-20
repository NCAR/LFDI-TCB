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


void TCB_InitStruct(struct sTuningControlBoard* s, struct sDAC* DAC, ){
{
    //For each of the Seven Temperature sensors initialize the struct
    for(int i = 0; i < 7; i++){
        TMP117_initStruct(&s->Sensor[i], &hi2c1, i);
    }
    Controller_InitStruct(struct sController* s, uint8_t heater);
    s->DAC = *DAC;
}
