#ifndef __DAC_H_
#define __DAC_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "funcs.h"

//Read/Write Should be OR-ed with the Register address
extern uint8_t WRITE;
extern uint8_t READ;
//Addresses of Registers (Last 5 bits)
extern uint8_t CONFIG;
extern uint8_t ANALOG_MONITOR_SELECT;
extern uint8_t GPIO_REGISTER;
extern uint8_t OFFSET_A;
extern uint8_t OFFSET_B;
extern uint8_t SPI_MODE;
extern uint8_t BROADCAST;
extern uint8_t Set_DAC0;
extern uint8_t Set_DAC1;
extern uint8_t Set_DAC2;
extern uint8_t Set_DAC3;
extern uint8_t Set_DAC4;
extern uint8_t Set_DAC5;
extern uint8_t Set_DAC6;
extern uint8_t Set_DAC7;
extern uint8_t ZERO_0;
extern uint8_t GAIN_0;
extern uint8_t ZERO_1;
extern uint8_t GAIN_1;
extern uint8_t ZERO_2;
extern uint8_t GAIN_2;
extern uint8_t ZERO_3;
extern uint8_t GAIN_3;
extern uint8_t ZERO_4;
extern uint8_t GAIN_4;
extern uint8_t ZERO_5;
extern uint8_t GAIN_5;
extern uint8_t ZERO_6;
extern uint8_t GAIN_6;
extern uint8_t ZERO_7;
extern uint8_t GAIN_7;


//Reference Voltage is 3V
extern float REFERENCE_VOLTAGE;



//DAC States
#define DAC_STATE_UNKNOWN (0)
#define DAC_STATE_INITFAILED (1)
#define DAC_STATE_REQUESTNOACK (2)
#define DAC_STATE_RECEIVEFAIL (3)
#define DAC_STATE_OK (4)

//Struct to hold the DAC Channels Information
struct sDAC_Channel{
	uint16_t upper_bound;
	uint16_t lower_bound;
	uint8_t DAC_number;
    bool state_high;
	bool enabled;
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


//Config Register Bits For the MSB REGISTER
extern uint8_t AB;
extern uint8_t LD;
extern uint8_t RST;
extern uint8_t PD_A;
extern uint8_t PD_B;
extern uint8_t SCE;
extern uint8_t No_EFFECT;
extern uint8_t GAIN_A;
//Config Register Bits For the LSB REGISTER
extern uint8_t GAIN_B;
extern uint8_t DSDO;
extern const uint8_t NOP;
extern uint8_t W2;


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
	void Set_Voltage_Peak_to_Peak(struct sDAC* sDAC, uint8_t Channel_Number, float* voltage);



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
