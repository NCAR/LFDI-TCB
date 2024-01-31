/*
 * funcs.c
 *
 *  Created on: Apr 14, 2021
 *      Author: damonb
 */

#include "funcs.h"
#include "usbd_def.h"
#include "usbd_cdc_if.h"
#include "defs.h"
#include <math.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "core_cm4.h"

// found this online
int parse_comma_delimited_str(char *string, char **fields, int max_fields)
{
   int i = 0;
   fields[i++] = string;
   while ((i < max_fields) && NULL != (string = strchr(string, ','))) {
      *string = '\0';
      fields[i++] = ++string;
   }
   return --i;
}

void USBSendString(char* buf)
{
  int i, res;
  for (i=0;i<50; i++)
  {
    res = CDC_Transmit_FS((uint8_t *)buf, strlen(buf));
    if (res == USBD_OK)
      return;

    HAL_Delay(1);
  }
}

void InitDWTTimer(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0; // reset the counter
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk ; // enable the counter
}

void Delay_us(uint32_t us)
{
  volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
  volatile uint32_t start = DWT->CYCCNT;
  do  {
  } while((DWT->CYCCNT - start) < cycles);
}

int32_t UIntToInt(uint32_t x) {
    union { uint32_t u; int32_t i; } un;
    un.u = x;
    return un.i;
}

uint32_t IntToUInt(int32_t x) {
    union { uint32_t u; int32_t i; } un;
    un.i = x;
    return un.u;
}

// in-place string replacement which only works if the new string is smaller than the old string
void replacestr(char *buffer, const char *search, const char *replace)
{
     char *sp;
     char* line = (char*) buffer;
     if ((sp = strstr(line, search)) == NULL) {
         return;
     }
     int search_len = strlen(search);
     int replace_len = strlen(replace);
     int tail_len = strlen(sp+search_len);

     memmove(sp+replace_len,sp+search_len,tail_len+1);
     memcpy(sp, replace, replace_len);
}
