/*
 * defs.h
 *
 *  Created on: Apr 14, 2021
 *      Author: damonb
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include <string.h>
#include "TMP117.h"

#define UART_BUFFER_SIZE (50)
#define UART_SENTENCE_QUEUE_SIZE (4)

extern I2C_HandleTypeDef hi2c2;

struct sUARTBuffer
{
  uint8_t Index;
  uint8_t ByteBuffer;
  uint8_t ReceiveBuffer[UART_BUFFER_SIZE];
  uint8_t ReadySentence[UART_SENTENCE_QUEUE_SIZE][UART_BUFFER_SIZE];
};

//struct sUARTBuffer USBBuffer = {0, 0, {0}, {0}};

#endif /* INC_DEFS_H_ */
