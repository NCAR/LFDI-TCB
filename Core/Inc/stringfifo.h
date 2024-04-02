/*
 * stringfifo.h
 *
 *  Created on: Nov 15, 2021
 * 
 */

#ifndef FIFO_H_
#define FIFO_H_

#define FIFO_BUFFER_SIZE 600
#include <stdio.h>
#include "defs.h"

struct sStringFIFO
{
    uint8_t buffer[FIFO_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
};
//Initialize the FIFO
void StringFIFOInit(struct sStringFIFO* fifo);
//Add a string to the FIFO
uint8_t StringFIFOAdd(struct sStringFIFO* fifo, uint8_t* item, size_t size);
//Remove a string from the FIFO
uint8_t StringFIFORemove(struct sStringFIFO* fifo, char* string);

uint16_t StringFIFOUsed(struct sStringFIFO* fifo);
uint16_t StringFIFOAvailable(struct sStringFIFO* fifo);


#endif /* FIFO_H_ */
