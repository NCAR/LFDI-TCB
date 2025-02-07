/*
 * TungControlBoard.c
 *
 *  Created on: Dec 16, 2022
 *      Author: mitchj
 */

#include "TuningControlBoard.h"


//This Will Setup the TCB structer by initiallizing the DAC and the TMP117 and the Controller
void TCB_InitStruct(struct sTuningControlBoard* s, I2C_HandleTypeDef* hi2c1,I2C_HandleTypeDef* hi2c2, SPI_HandleTypeDef* hspi){

	uint8_t tmp_address[4] = {0x02, 0x01, 0x03, 0x02, 0x01, 0x03};
    //For each of the Seven Temperature sensors initialize the struct
    for(int i = 0; i < NUMOFSENSORSBUS1; i++){
        //The Last parameter is the address of the sensor this will need to be changed as we get more sensors
        TMP117_InitStruct(&s->Sensor[i], hi2c1, tmp_address[i]);
    }
//    for(int i = 0; i < NUMOFSENSORSBUS2; i++){
//        //The Last parameter is the address of the sensor this will need to be changed as we get more sensors
//        TMP117_InitStruct(&s->Sensor[i+NUMOFSENSORSBUS1], hi2c2, 0x00+i);
//    }
    //Initialize the DAC
    DAC_InitStruct(&s->DAC8718, hspi);
    
    //Initialize the Controller
    //All of these are set up to use the first sensor this will need to be changed as we get more sensors
    for (int i = 0; i < NUMOFHEATERCONTROLLERS; i++){
        HeaterController_InitStruct(&s->HeaterControllers[i], &s->Sensor[i], i+1);
    }
    
    //Initialize the Compensator
    //Todo this will need to have different sensors
    float StageSizes[NUMOFCOMPENSATORS] = {STAGE1, STAGE2, STAGE3, STAGE4, STAGE5, STAGE6};
    for(int i = 0; i < NUMOFCOMPENSATORS; i++){
        Compensator_InitStruct(&s->Compensator[i], &s->Sensor[i], &s->DAC8718.DAC_Channels[i], StageSizes[i]);
    }

    //Initialize the GPIO
    for(int i = 0; i < NUMOFGPIO; i++){
        GPIO_InitStruct(&s->GPIO[i], i+1);
    }

    //Initialize the Bipolar Output
    for(int i = 0; i < NUMOFBipolarOutputs; i++){
        BipolarOutput_InitStruct(&s->BipolarOutput[i], i+1, &s->DAC8718.DAC_Channels[i+NUMOFCOMPENSATORS]);
    }
}
