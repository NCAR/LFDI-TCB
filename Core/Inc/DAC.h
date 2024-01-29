#ifndef __DAC_H_
#define __DAC_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "funcs.h"


//Reference Voltage is 3.0V
extern float REFERENCE_VOLTAGE;


//Struct to hold the DAC Channels Information
struct sDAC_Channel{
	uint8_t DAC_number;
	uint16_t upper_bound;
	uint16_t lower_bound;
    bool state_high;
	bool enabled;
	float max_peak2peak;
};

//Struct to hold the DAC Information
struct sDAC{
	SPI_HandleTypeDef* spi;
	uint16_t config;
	uint16_t offset_a;
	uint16_t offset_b;
	bool Configured;
	uint8_t State;
	bool Ready;
	uint16_t Errors;
	float max_peak2peak;
	struct sDAC_Channel DAC_Channels[8];
};


//public:
void DAC_Initialize(struct sDAC* s);
void DAC_InitStruct(struct sDAC* s, SPI_HandleTypeDef* hspi);
void DAC_Reset(struct sDAC* s);
void Set_DAC_Max(struct sDAC* s , uint8_t DAC_Num);
void Set_DAC_Min(struct sDAC* s, uint8_t DAC_Num);
void Set_DAC_Value(struct sDAC* s, uint8_t DAC_Num, uint16_t val);
void Set_DAC_Zero(struct sDAC* s, uint8_t DAC_Num, uint16_t val);
void Set_DAC_Gain(struct sDAC* s, uint8_t DAC_Num, uint16_t val);
void Get_DAC_Value(struct sDAC* s, uint8_t DAC_Num, uint16_t* val);
void Get_DAC_Zero(struct sDAC* s, uint8_t DAC_Num, uint16_t* val);
void Get_DAC_Gain(struct sDAC* s, uint8_t DAC_Num, uint16_t* val);
void Set_Voltage_Peak_to_Peak(struct sDAC_Channel* sDAC_Channel, float* voltage);



//private:
void Set_Config(struct sDAC* s);
void Get_Offset_A(struct sDAC* s);
void Get_Offset_B(struct sDAC* s);
void Get_Register(uint8_t address, uint8_t *Value);
void Set_Register(struct sDAC* s);
void Send_Command(struct sDAC* s, uint8_t* Command);
void Recieve_Data(struct sDAC* s, uint8_t* Data);
void Test_Config_Register(struct sDAC* s);
void Set_nWakeUp_high(bool HIGH);
bool Get_nWakeUp();
void Set_nLDAC_high(bool HIGH);
bool Get_nLDAC();
void Set_nCLR_high(bool HIGH);
bool Get_nCLR();
void Set_nRST_high(bool HIGH);
bool Get_nRST();
void Set_nCS_high(bool HIGH);
bool Get_nCS();
void Hardware_Reset(struct sDAC* s);
void Syncronous_Update();
void Setup_DAC(struct sDAC* s);
void Read_All_Registers(struct sDAC* s);
void Set_Max_Peak_To_Peak_Voltage(struct sDAC* s, bool Gain);

#endif
