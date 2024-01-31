/*
 * stringfifo.c
 *
 *  Created on: Nov 15, 2021
 *      Author: damonb
 *      Edited: mjeffers
 *      This file contains the functions for the string FIFO
 *      The FIFO is used to store strings that are to be sent over USB
 *      The FIFO is a circular buffer
 *      The FIFO is used in the USB CDC interface
 * 
 */

#include <stringfifo.h>
#include "string.h"
#include <stdint.h>


//Initalize the Fifo Structure
void StringFIFOInit(struct sStringFIFO* fifo)
{
  uint16_t i;
  fifo->head = 0;
  fifo->tail = 0;
  //Set all the values to 0
  for (i=0; i<FIFO_BUFFER_SIZE; i++)
    fifo->buffer[i]=0;
}

//Return the number of bytes used in the FIFO
uint16_t StringFIFOUsed(struct sStringFIFO* fifo)
{
  int16_t used = fifo->tail - fifo->head;
  if (used < 0)
    used += FIFO_BUFFER_SIZE;
  return (uint16_t) used;
}

//Return the number of bytes available in the FIFO
uint16_t StringFIFOAvailable(struct sStringFIFO* fifo)
{
  uint16_t used = StringFIFOUsed(fifo);
  int16_t available = FIFO_BUFFER_SIZE - used - 1; // don't use the last byte, otherwise we won't know that we're full
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
    printf("Fifo full\n");
    return 1;
  }

  //If the size of the string is larger than the available space, truncate it
  if (size > available)
    size = available;
  //Copy Over the string to the FIFO buffer
  memcpy(&fifo->buffer[fifo->tail], item, size);
  //Update the tail
  fifo->tail = (fifo->tail + size) % FIFO_BUFFER_SIZE;
  return 0;
}

//Go through the FIFO and remove the Characters until the first \n or \r is found
uint8_t StringFIFORemove(struct sStringFIFO* fifo, char* string)
{

  uint16_t i, pos;
  uint16_t used = StringFIFOUsed(fifo);
  uint16_t h_length, t_length;
  int16_t len;

  //if its empty, return 3
  if (fifo->head == fifo->tail)
  {
	  //printf("Empty fifo.\n");
    return 3;
  }
  //Go through Character by Character
  for (i=0; i<used; i++)
  {
    //Get the position of the character
    pos = (fifo->head + i) % FIFO_BUFFER_SIZE;
    //If the character is a \n or \r, then we have found the end of the string
    if ((fifo->buffer[pos] == '\n') || (fifo->buffer[pos] == '\r'))
    {
      //If the string is empty, return 1
      if (i == 0)
      {
        fifo->head = (fifo->head + 1) % FIFO_BUFFER_SIZE;
        string[0] = 0;
//        printf("Zero length string in fifo.\n");
        return 1;
      }
      //find the length of the string
      len = (pos - fifo->head);
      //If the length is negative, add the buffer size
      if (len < 0)
        len += FIFO_BUFFER_SIZE;
      //If the length is larger than the buffer size to the head, then split the string
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
      //Copy the string to the output buffer
      memcpy(string, &fifo->buffer[fifo->head], h_length);
      //if the string is split, copy the second part
      if (t_length)
        memcpy(&string[h_length], fifo->buffer, t_length);
      //Update the head of the FIFO buffer to the next character
      fifo->head = (fifo->head + len + 1) % FIFO_BUFFER_SIZE;
      //Add a null terminator to the end of the string
      string[len] = 0;
      return 0;
    }
  }
//  printf("Fifo has no terminator\n");
  return 2;
}
