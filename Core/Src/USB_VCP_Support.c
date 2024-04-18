/*
 * USB_VCP_Support.c
 *
 *  Created on: Apr 18, 2024
 */

#include <stdarg.h>
#include "USB_VCP_Support.h"

struct sStringFIFO FIFO_USB_In;
struct sStringFIFO FIFO_USB_Out;
static char buf[200] = {0};

extern USBD_HandleTypeDef hUsbDeviceFS;

void USB_FIFO_Init(uint16_t in_fifo_size, uint16_t out_fifo_size)
{
  StringFIFOInit(&FIFO_USB_In, in_fifo_size);
  StringFIFOInit(&FIFO_USB_Out, out_fifo_size);
}

uint8_t USB_Try_Send()
{
  int res;
  // check if the CDC device is busy
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0)
    return 1;

  if (StringFIFORemove(&FIFO_USB_Out, sizeof(buf), buf, sizeof(buf)) == 0)
  {
    res = CDC_Transmit_FS((uint8_t *)buf, strlen(buf));
    if (res == USBD_OK)
      return 0;
    else
      return 3;
  }
  return 2;
}

uint8_t USB_Aggressive_Send(uint16_t retries)
{
  uint16_t i;
  for (i=0; i < retries; i++)
  {
    if (USB_Try_Send() == 0)
      return 0;
    HAL_Delay(1);
  }
  return 1;
}

int uprintf(const char *format, ...)
{
  char buffer[200] = {0};
  int len = 0;
  va_list arguments;
  va_start(arguments, format);
  len = vsnprintf(buffer, sizeof(buffer), format, arguments);
  va_end(arguments);
  if (len > 0)
    StringFIFOAdd(&FIFO_USB_Out, (uint8_t *)buffer, strlen(buffer));
  return len;
}
