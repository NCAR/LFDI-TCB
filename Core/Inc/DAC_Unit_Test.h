/*
 * DAC_Unit_Test.h
 *
 *  Created on: Dec 6, 2022
 *      Author: mjeffers
 */


#ifndef INC_DAC_UNIT_TEST_H_
#define INC_DAC_UNIT_TEST_H_
#include "DAC.h"

uint8_t unit_test_Default_Values(struct sDAC *s, uint8_t *Register, uint8_t *Default_Value);
uint8_t unit_test_Config_Register_Default(struct sDAC *s);
uint8_t unit_test_ANALOG_MONITOR_SELECT_Register_Default(struct sDAC *s);
uint8_t unit_test_GPIO_Register_Default(struct sDAC *s);
uint8_t unit_test_OFFSET_A_Default(struct sDAC *s);
uint8_t unit_test_OFFSET_B_Default(struct sDAC *s);
uint8_t unit_test_SPI_MODE_Default(struct sDAC *s);
uint8_t unit_test_BROADCAST_Default(struct sDAC *s);
uint8_t unit_test_DAC_0_Default(struct sDAC *s);
uint8_t unit_test_DAC_1_Default(struct sDAC *s);
uint8_t unit_test_DAC_2_Default(struct sDAC *s);
uint8_t unit_test_DAC_3_Default(struct sDAC *s);
uint8_t unit_test_DAC_4_Default(struct sDAC *s);
uint8_t unit_test_DAC_5_Default(struct sDAC *s);
uint8_t unit_test_DAC_6_Default(struct sDAC *s);
uint8_t unit_test_DAC_7_Default(struct sDAC *s);
uint8_t unit_test_ZERO_0_Default(struct sDAC *s);
uint8_t unit_test_ZERO_1_Default(struct sDAC *s);
uint8_t unit_test_ZERO_2_Default(struct sDAC *s);
uint8_t unit_test_ZERO_3_Default(struct sDAC *s);
uint8_t unit_test_ZERO_4_Default(struct sDAC *s);
uint8_t unit_test_ZERO_5_Default(struct sDAC *s);
uint8_t unit_test_ZERO_6_Default(struct sDAC *s);
uint8_t unit_test_ZERO_7_Default(struct sDAC *s);
uint8_t unit_test_GAIN_0_Default(struct sDAC *s);
uint8_t unit_test_GAIN_1_Default(struct sDAC *s);
uint8_t unit_test_GAIN_2_Default(struct sDAC *s);
uint8_t unit_test_GAIN_3_Default(struct sDAC *s);
uint8_t unit_test_GAIN_4_Default(struct sDAC *s);
uint8_t unit_test_GAIN_5_Default(struct sDAC *s);
uint8_t unit_test_GAIN_6_Default(struct sDAC *s);
uint8_t unit_test_GAIN_7_Default(struct sDAC *s);
uint8_t unit_test_Default_Values_All_Registers(struct sDAC *s);




#endif /* INC_DAC_UNIT_TEST_H_ */
