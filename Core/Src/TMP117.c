/*
 * TMP117.c
 *
 *  Created on: Nov 11, 2021
 *      Author: damonb
 *  Edited on : Dec 15, 2022: MJ
 *  
 */


#include "TMP117.h"
#include "funcs.h"


//@brief Initialize the TMP117 structure
//@param s Pointer to the structure to initialize
//@param interface Pointer to the I2C interface to use
//@param addpin Value of the address pin
//@return None
void TMP117_InitStruct(struct sTMP117* s, I2C_HandleTypeDef* interface, uint8_t addpin)
{
  /* addpin values define what the address pin is tied to:
   * 0x00 ground
   * 0x01 V+
   * 0x10 SDA
   * 0x11 SCL
   */
  uint8_t i;
  s->Address = 0b1001000 | addpin;
  s->Interface = interface;
  s->Average = -273;
  s->Configured = false;
  s->SamplesInAverage = 16;
  s->Index = 0;
  s->State = TMP117_STATE_UNKNOWN;
  for (i=0;i<64;i++)
    s->Temperature[i] = -273;
  s->LastTemperature = -273;
  s->Ready = false;
  s->Errors = 0;
}


//@brief Configure the TMP117
//@param s Pointer to the structure to configure
//@return None
void TMP117_Configure(struct sTMP117* s)
{
  HAL_StatusTypeDef res;
  printf("Attempting to init... ");
  uint8_t buffer[3] = {0};
  buffer[0] = 1;
  buffer[2] = 1 << 5;
  res = HAL_I2C_Master_Transmit(s->Interface, (s->Address) << 1, buffer, 3, 10); // 8 samples averaged
  if (res == HAL_OK)
  {
    s->Configured = true;
    printf("OK!\n");
  }
  else
  {
    s->Errors++;
    s->State = TMP117_STATE_INITFAILED;
    printf("Failed!\n");
  }
}

//@brief Get the temperature from the TMP117
//@param s Pointer to the structure to get the temperature from
//@return None
void TMP117_GetTemperature(struct sTMP117* s)
{
  uint8_t i;
  uint16_t u;
  double t = 0;
  //setup an empty Buffer to request the temperature
  uint8_t buffer[2] = {0};
  uint16_t data = 0;
  HAL_StatusTypeDef res;
  res = HAL_I2C_Master_Transmit(s->Interface, (s->Address) << 1, buffer, 1, 10); // request temperature
  if (res != HAL_OK)
  {
    s->State = TMP117_STATE_REQUESTNOACK;
    s->Errors++;
    //If this temperature sensor gets more than 100 errors in a row, it's probably not connected
    if (s->Errors > 100)
    {
      s->Configured = false;
      s->Errors = 0;
    }
    return;
  }

  res = HAL_I2C_Master_Receive(s->Interface, (s->Address << 1), buffer, 2, 10); // receive temperature
  if (res != HAL_OK)
  {
    s->State = TMP117_STATE_RECEIVEFAIL;
    s->Errors++;
    if (s->Errors > 100)
    {
      s->Configured = false;
      s->Errors = 0;
    }
    return;
  }

  
  s->State = TMP117_STATE_VALIDTEMP;
  data = (buffer[0] << 8) | buffer[1];
  u = *(&(data));
  //Conver the Temp as per documentation
  s->Temperature[s->Index] = ((double)u) * 0.0078125;
  s->LastTemperature = s->Temperature[s->Index];
  s->Index++;
  //If we finally have enough Temperatures to average, set the Ready flag
  TMP117_GetAverageTemperature(s);

}


//@brief Get the average temperature from the TMP117
//@param s Pointer to the structure to get the average temperature from
//@return The average temperature
void TMP117_GetAverageTemperature(struct sTMP117* s)
{
  //if we have engough samples, set the flags
  if (s->Index >= s->SamplesInAverage)
  {
    s->Ready = true;
    s->Index = 0;
  }
  
  //average the Temperatures
  if (s->Ready)
  {
    for (i=0;i<(s->SamplesInAverage); i++)
    {
      t += s->Temperature[i];
    }
    s->Average = t / s->SamplesInAverage;
  }
}