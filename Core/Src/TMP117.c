/*
 * TMP117.c
 *
 *  Created on: Nov 11, 2021
 *      Author: damonb
 */


#include "TMP117.h"
#include "funcs.h"

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
  s->Average = -273.0f;
  s->Configured = false;
  s->SamplesInAverage = 16;
  s->State = TMP117_STATE_UNKNOWN;
  for (i=0;i<64;i++)
    s->Temperature[i] = -273.0f;
  s->Errors = 0;
}

void TMP117_Configure(struct sTMP117* s)
{
  HAL_StatusTypeDef res;
  printf("Attempting to init... ");
  uint8_t buffer[3] = {0};
  buffer[0] = 1;
  buffer[2] = 1 << 5; // average 8 temperatures

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

void TMP117_GetTemperature(struct sTMP117* s)
{
  uint8_t i;
  int16_t cast_int;
  float t = 0;
  uint8_t buffer[2] = {0};
  uint16_t data = 0;
  HAL_StatusTypeDef res;
  res = HAL_I2C_Master_Transmit(s->Interface, (s->Address) << 1, buffer, 1, 10); // request temperature
  if (res != HAL_OK)
  {
    s->State = TMP117_STATE_REQUESTNOACK;
    s->Errors++;
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

  {
    s->State = TMP117_STATE_VALIDTEMP;
    // shuffle all the temperature values in the array by 1
    memmove(s->Temperature + 1, s->Temperature, (s->SamplesInAverage-1)*sizeof(float));

    data = (buffer[0] << 8) | buffer[1];
    cast_int = *(&(data));
    s->Temperature[0] = ((double)cast_int) * 0.0078125;

    if (s->Temperature[1] < -200.0f) // this is our first temperature? init full array
      for (i=1; i<(s->SamplesInAverage); i++)
        s->Temperature[i] = s->Temperature[0];

    for (i=0;i<(s->SamplesInAverage); i++)
    {
      t += s->Temperature[i];
    }
    s->Average = t / s->SamplesInAverage;
  }
}

