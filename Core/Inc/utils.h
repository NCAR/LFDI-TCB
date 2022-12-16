/*
 * test.h
 *
 *  Created on: Nov 16, 2022
 *      Author: damonb
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "core_cm4.h"

void InitDWTTimer(void);

void Delay_us(uint32_t us);



#endif /* INC_UTILS_H_ */
