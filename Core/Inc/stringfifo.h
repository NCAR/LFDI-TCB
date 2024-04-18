/*
 * stringfifo.h
 *
 *  Created on: Nov 15, 2021
 * 
 */

#ifndef FIFO_H_
#define FIFO_H_

//#define FIFO_BUFFER_SIZE 250
#include <stdio.h>
#include "defs.h"
#include <stdlib.h>

struct sStringFIFO
{
    char* buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t buffer_size;
};

void StringFIFOInit(struct sStringFIFO* fifo, size_t size);
uint8_t StringFIFOAdd(struct sStringFIFO* fifo, uint8_t* item, size_t size);
uint8_t StringFIFORemove(struct sStringFIFO* fifo, uint16_t len, char* string, size_t size);
uint8_t StringFIFORemoveLine(struct sStringFIFO* fifo, char* string, size_t buffer_size);

uint16_t StringFIFOUsed(struct sStringFIFO* fifo);
uint16_t StringFIFOAvailable(struct sStringFIFO* fifo);


#endif /* FIFO_H_ */
