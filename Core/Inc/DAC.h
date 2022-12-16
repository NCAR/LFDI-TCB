#ifndef __DAC_H_
#define __DAC_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "utils.h"

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
extern float MAX_PEAK2PEAK;
struct DAC_Channel{
	uint16_t upper_bound;
	uint16_t lower_bound;
	uint8_t DAC_number;
    bool state_high;
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
//NOP Command
/*
 *
 */
//Setter and Getters for the GPIO Pin State associated with each pin
void Set_nLDAC_high(bool HIGH);
bool Get_nLDAC_high();
void Set_nCLR_high(bool HIGH);
bool Get_nCLR_high();
void Set_nRST_high(bool HIGH);
bool Get_nRST_high();
void Set_nCS_high(bool HIGH);
bool Get_nCS_high();
void Set_nWakeUp_high(bool HIGH);
bool Get_nWakeUp_high();
void Set_DAC_Max(SPI_HandleTypeDef* spi, uint8_t DAC_Num);
void Set_DAC_Min(SPI_HandleTypeDef* spi, uint8_t DAC_Num);
void Send_Command(SPI_HandleTypeDef* spi, uint8_t* Command);
void Recieve_Data(SPI_HandleTypeDef* spi, uint8_t* Data);
void Set_Voltage(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val);
void Set_Voltage_Peak_to_Peak(float* voltage, uint16_t* values);
//Test the Initial Boot of the DAC by reading from the config register at 0x00
void Test_Config_Register(SPI_HandleTypeDef* spi);
void Get_Register(uint8_t address, uint8_t *Value);
void Set_Register(SPI_HandleTypeDef* spi);

//@Brief: This function Will
void Test_Config_Register(SPI_HandleTypeDef* spi);
void Send_Command(SPI_HandleTypeDef* spi, uint8_t* Command);
void Recieve_Data(SPI_HandleTypeDef* spi, uint8_t* Data);

//Set the DAC Values for Zero, gain and value
void Set_DAC_Value(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val);
void Set_DAC_Zero(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val);
void Set_DAC_Gain(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val);
void Get_DAC_Value(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t* val);
void Get_DAC_Zero(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t* val);
void Get_DAC_Gain(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t* val);


//Set all the Pull up/pull down pins
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

//Top level Functions for the DAC
void Hardware_Reset(void);
void Syncronous_Update(void);
void Setup_DAC(void);
void Read_All_Registers(SPI_HandleTypeDef* spi);


//Test_Config_Register(uint8_t *Value);
#endif
