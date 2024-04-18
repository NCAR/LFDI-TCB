/*
 * USB_VCP_Support.h
 *
 *  Created on: Apr 18, 2024
 */

#ifndef INC_USB_VCP_SUPPORT_H_
#define INC_USB_VCP_SUPPORT_H_

#include "stringfifo.h"
#include "usbd_cdc_if.h"

extern struct sStringFIFO FIFO_USB_In;
extern struct sStringFIFO FIFO_USB_Out;

void USB_FIFO_Init(uint16_t in_fifo_size, uint16_t out_fifo_size);
uint8_t USB_Try_Send();
uint8_t USB_Aggressive_Send(uint16_t retries);
int uprintf(const char *format, ...);


#endif /* INC_USB_VCP_SUPPORT_H_ */
