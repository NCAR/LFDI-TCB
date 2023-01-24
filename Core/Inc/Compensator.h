/*
 * Compensator.h
 *
 *  Created on: Jan 24, 2023
 *      Author: mjeffers
 */

#ifndef INC_COMPENSATOR_H_
#define INC_COMPENSATOR_H_



#include "defs.h"
#include "TMP117.h"
#include "DAC.h"

//Structure that Pairs a specific Temperature Sensor with Specific DAC Channel
struct sCompensator
{
  struct sTMP117 Sensor;
  struct sDAC_Channel Channel;
  bool enable = false;
  bool compensate = false;
};

void Compensator_InitStruct(struct sCompensator* s, struct sTMP117 *t,struct sDAC_Channel *c);
void Compensator_SetChannel(struct sCompensator* s);




#endif /* INC_COMPENSATOR_H_ */
