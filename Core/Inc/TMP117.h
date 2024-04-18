/*
 * TMP117.h
 *
 *  Created on: Nov 11, 2021
 *  
 */

#ifndef INC_TMP117_H_
#define INC_TMP117_H_

#include "defs.h"

#define TMP117_STATE_UNKNOWN (0)
#define TMP117_STATE_INITFAILED (1)
#define TMP117_STATE_REQUESTNOACK (2)
#define TMP117_STATE_RECEIVEFAIL (3)
#define TMP117_STATE_VALIDTEMP (4)

#define TMP117_CONFIG_NEEDED (0)
#define TMP117_CONFIG_OK (1)
#define TMP117_CONFIG_FAILED (2)


struct sTMP117
{
  uint8_t Address;
  I2C_HandleTypeDef* Interface;
  float Temperature[64];
  float Average;
  uint8_t Configured;
  uint8_t SamplesInAverage;
  uint8_t State;
  uint16_t Errors;
};

void TMP117_Configure(struct sTMP117* s);
void TMP117_InitStruct(struct sTMP117* s, I2C_HandleTypeDef* interface, uint8_t addpin);
void TMP117_GetTemperature(struct sTMP117* s);

#endif /* INC_TMP117_H_ */
