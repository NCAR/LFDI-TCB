/*
 * test.c
 *
 *  Created on: Nov 16, 2022
 *      Author: damonb
 */

#include "utils.h"

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
