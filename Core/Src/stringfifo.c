/*
 * stringfifo.c
 *
 *  Created on: Nov 15, 2021
 *      Author: damonb
 */

#include <stringfifo.h>
#include "string.h"

void StringFIFOInit(struct sStringFIFO* fifo)
{
  uint16_t i;
  fifo->head = 0;
  fifo->tail = 0;
  for (i=0; i<FIFO_BUFFER_SIZE; i++)
    fifo->buffer[i]=0;
}

uint16_t StringFIFOUsed(struct sStringFIFO* fifo)
{
  int16_t used = fifo->tail - fifo->head;
  if (used < 0)
    used += FIFO_BUFFER_SIZE;
  return (uint16_t) used;
}

uint16_t StringFIFOAvailable(struct sStringFIFO* fifo)
{
  uint16_t used = StringFIFOUsed(fifo);
  int16_t available = FIFO_BUFFER_SIZE - used - 1; // don't use the last byte, otherwise we won't know that we're full
  return available;
}

uint8_t StringFIFOAdd(struct sStringFIFO* fifo, uint8_t* item, size_t size)
{
  int16_t available = StringFIFOAvailable(fifo);

  if (available == 0)
  {
    printf("Fifo full\n");
    return 1;
  }

  if (size > available)
    size = available;

  memcpy(&fifo->buffer[fifo->tail], item, size);
  fifo->tail = (fifo->tail + size) % FIFO_BUFFER_SIZE;
  return 0;
}

uint8_t StringFIFORemove(struct sStringFIFO* fifo, uint8_t* string)
{
  uint16_t i, pos;
  uint16_t used = StringFIFOUsed(fifo);
  uint16_t h_length, t_length;
  int16_t len;

  if (fifo->head == fifo->tail)
  {
//    printf("Empty fifo.\n");
    return 3;
  }

  for (i=0; i<used; i++)
  {
    pos = (fifo->head + i) % FIFO_BUFFER_SIZE;
    if ((fifo->buffer[pos] == '\n') || (fifo->buffer[pos] == '\r'))
    {
      if (i == 0)
      {
        fifo->head = (fifo->head + 1) % FIFO_BUFFER_SIZE;
        string[0] = 0;
//        printf("Zero length string in fifo.\n");
        return 1;
      }
      len = (pos - fifo->head);
      if (len < 0)
        len += FIFO_BUFFER_SIZE;
      if (len > (FIFO_BUFFER_SIZE - fifo->head))
      {
        h_length = FIFO_BUFFER_SIZE - fifo->head;
        t_length = len - h_length;
      }
      else
      {
        h_length = len;
        t_length = 0;
      }
      memcpy(string, &fifo->buffer[fifo->head], h_length);
      if (t_length)
        memcpy(&string[h_length], fifo->buffer, t_length);
      fifo->head = (fifo->head + len + 1) % FIFO_BUFFER_SIZE;
      string[len] = 0;
      return 0;
    }
  }
//  printf("Fifo has no terminator\n");
  return 2;
}
