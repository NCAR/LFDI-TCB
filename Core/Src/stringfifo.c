/*
 * stringfifo.c
 *
 *  Created on: Nov 15, 2021
 *      Edited: mjeffers
 *      This file contains the functions for the string FIFO
 *      The FIFO is used to store strings that are to be sent over USB
 *      The FIFO is a circular buffer
 *      The FIFO is used in the USB CDC interface
 * 
 */

#include <stringfifo.h>
#include "string.h"
#include "defs.h"
#include <stdint.h>

void StringFIFOInit(struct sStringFIFO* fifo, size_t size)
{
  if (size > (UINT16_MAX-1))
    size = UINT16_MAX-1;
  //uint16_t i;
  fifo->head = 0;
  fifo->tail = 0;
  fifo->buffer_size = size;
  fifo->buffer = calloc(size, sizeof(char));

  // if malloc/calloc failed, just stop
  if (fifo->buffer == NULL)
    while(1) {}

//  for (i=0; i<size; i++)
//    fifo->buffer[i]=0;
}

//Return the number of bytes used in the FIFO
uint16_t StringFIFOUsed(struct sStringFIFO* fifo)
{
  int16_t used = fifo->tail - fifo->head;
  if (used < 0)
    used += fifo->buffer_size;
  return (uint16_t) used;
}

//Return the number of bytes available in the FIFO
uint16_t StringFIFOAvailable(struct sStringFIFO* fifo)
{
  uint16_t used = StringFIFOUsed(fifo);
  int16_t available = fifo->buffer_size - used - 1; // don't use the last byte, otherwise we won't know that we're full
  return available;
}

//Add a string to the FIFO
uint8_t StringFIFOAdd(struct sStringFIFO* fifo, uint8_t* item, size_t size)
{
  //Find out how many bytes are available
  int16_t available = StringFIFOAvailable(fifo);

  //If there are no bytes available, return 1
  if (available == 0)
  {
    printf("FIFO full\n");
    return 1;
  }

  //If the size of the string is larger than the available space, truncate it
  if (size > available)
    size = available;

  if ((fifo->buffer_size - fifo->tail) < size) // if our string is going to wrap
  {
    memcpy(&fifo->buffer[fifo->tail], item, (fifo->buffer_size - fifo->tail));
    memcpy(fifo->buffer, &item[(fifo->buffer_size - fifo->tail)], size - (fifo->buffer_size - fifo->tail));
  }
  else
    memcpy(&fifo->buffer[fifo->tail], item, size);
  fifo->tail = (fifo->tail + size) % fifo->buffer_size;

  return 0;
}

//Go through the FIFO and remove the Characters until the first \n or \r is found
uint8_t StringFIFORemoveLine(struct sStringFIFO* fifo, char* string, size_t buffer_size)
// does not include EOL character
{
  uint16_t i, pos;
  uint16_t used = StringFIFOUsed(fifo);
  int16_t len;

  //if its empty, return 3
  if (fifo->head == fifo->tail)
  {
//    printf("Empty fifo.\n");
    return 3;
  }
  //Go through Character by Character
  for (i=0; i<used; i++)
  {
    pos = (fifo->head + i) % fifo->buffer_size;
    if ((fifo->buffer[pos] == '\n') || (fifo->buffer[pos] == '\r'))
    {
      //If the string is empty, return 1
      if (i == 0)
      {
        fifo->head = (fifo->head + 1) % fifo->buffer_size;
        string[0] = 0;
//        printf("Zero length string in fifo.\n");
        return 1;
      }
      len = (pos - fifo->head) + 1;
      if (len > buffer_size)
        len = buffer_size;
      return StringFIFORemove(fifo, len, string, buffer_size);
    }
  }
  //  printf("Fifo has no terminator\n");
  return 2;

}


uint8_t StringFIFORemove(struct sStringFIFO* fifo, uint16_t len, char* string, size_t size)
{
  uint16_t used = StringFIFOUsed(fifo);
  if (used == 0)
    return 1;
  uint16_t h_length, t_length;

  if (len >= size)
    len = size - 1; // leave room for the terminator
  if (len < 0)
    len += fifo->buffer_size;
  if (len > used)
    len = used;
  if (len > (fifo->buffer_size - fifo->head))
  {
    h_length = fifo->buffer_size - fifo->head;
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
//  fifo->head = (fifo->head + len + 1) % fifo->buffer_size;   // we used to skip the EOL character
  fifo->head = (fifo->head + len) % fifo->buffer_size;
  string[len] = 0;
  return 0;
}
