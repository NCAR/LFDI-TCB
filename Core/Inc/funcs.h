/*
 * funcs.h
 *
 *  Created on: Apr 14, 2021
 *      Author: damonb
 */

#ifndef INC_FUNCS_H_
#define INC_FUNCS_H_

#include "defs.h"


void USBSendString(char* buf);
void InitDWTTimer(void);
void Delay_us(uint32_t us);
int32_t UIntToInt(uint32_t x);
uint32_t IntToUInt(int32_t x);
int parse_comma_delimited_str(char *string, char **fields, int max_fields);
void replacestr(uint8_t *buffer, const char *search, const char *replace);

#endif /* INC_FUNCS_H_ */