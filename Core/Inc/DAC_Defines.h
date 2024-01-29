/*
 * DAC_Defines.h
 *
 *  Created on: Jan 29, 2024
 *      Author: damonb
 */

#ifndef INC_DAC_DEFINES_H_
#define INC_DAC_DEFINES_H_

//DAC States
#define DAC_STATE_UNKNOWN (0)
#define DAC_STATE_INITFAILED (1)
#define DAC_STATE_REQUESTNOACK (2)
#define DAC_STATE_RECEIVEFAIL (3)
#define DAC_STATE_OK (4)


#define nLDAC_Pin (GPIO_PIN_13)
#define nLDAC_GPIO_Port (GPIOB)
#define nCLR_Pin (GPIO_PIN_11)
#define nCLR_GPIO_Port (GPIOE)
#define nRST_Pin (GPIO_PIN_10)
#define nRST_GPIO_Port (GPIOE)
#define nCS_Pin (GPIO_PIN_14)
#define nCS_GPIO_Port (GPIOB)
#define nWakeUp_Pin (GPIO_PIN_12)
#define nWakeUp_GPIO_Port (GPIOB)

//Read/Write Should be OR-ed with the Register address
#define WRITE (0x00)
#define READ (0x80)
//Addresses of Registers Last 5 bits)
#define CONFIG (0x00)
#define ANALOG_MONITOR_SELECT (0x01)
#define GPIO_REGISTER ( 0x02)
#define OFFSET_A (0x03)
#define OFFSET_B (0x04)
#define SPI_MODE (0x06)
#define BROADCAST (0x07)
#define Set_DAC0 (0x08)
#define Set_DAC1 (0x09)
#define Set_DAC2 (0x0A)
#define Set_DAC3 (0x0B)
#define Set_DAC4 (0x0C)
#define Set_DAC5 (0x0D)
#define Set_DAC6 (0x0E)
#define Set_DAC7 (0x0F)
#define ZERO_0 (0x10)
#define GAIN_0 (0x18)
#define ZERO_1 (0x11)
#define GAIN_1 (0x19)
#define ZERO_2 (0x12)
#define GAIN_2 (0x1A)
#define ZERO_3 (0x13)
#define GAIN_3 (0x1B)
#define ZERO_4 (0x14)
#define GAIN_4 (0x1C)
#define ZERO_5 (0x15)
#define GAIN_5 (0x1D)
#define ZERO_6 (0x16)
#define GAIN_6 (0x1E)
#define ZERO_7 (0x17)
#define GAIN_7 (0x1F)

//Config Register Bits For the MSB REGISTER
#define AB (0x80)
#define LD (0x40)
#define RST (0x20)
#define PD_A (0x10)
#define PD_B (0x08)
#define SCE (0x04)
#define No_EFFECT (0x02)
#define GAIN_A (0x01)
//Config Register Bits For the LSB REGISTER
#define GAIN_B (0x80)
#define DSDO (0x40)
#define NOP (0x20)
#define W2 (0x10)


#endif /* INC_DAC_DEFINES_H_ */
