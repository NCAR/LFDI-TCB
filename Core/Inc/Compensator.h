/*
 * Compensator.h

 * This is a structure that pairs a specific Temperature Sensor with Specific DAC Channel
 * And Performs Compensation algorythom to set output voltage
 *  Created on: Jan 24, 2023
 *      Author: mjeffers
 */

#ifndef INC_COMPENSATOR_H_
#define INC_COMPENSATOR_H_



#include "defs.h"
#include "TMP117.h"
#include "DAC.h"
#include <stdbool.h>
#include "TemperatureCorrectionTable.h"

//Structure that Pairs a specific Temperature Sensor with Specific DAC Channel
//And Performs Compensation algorythom to set output voltage
struct sCompensator
{
  struct sTMP117 Sensor;
  struct sDAC_Channel Channel;
  //is the Channel enabled
  bool Enable;
  //Should we be tryying to Compensate to a specific Wavelength
  bool compensate;
  //Use the Average stored in the TMP117 struct or the last temperature
  bool useAverage;

  //Wavelength to Compensate to
  float wavelength;
  //if we are not using the auto compensation what should this be outputting
  float voltage;

};

//Initalize the Structure of the Compensator
void Compensator_InitStruct(struct sCompensator* s, struct sTMP117 *t,struct sDAC_Channel *c);
void Compensator_enableChannel(struct sCompensator* s, bool enable);
void Compensator_SetChannel(struct sCompensator* s);
void Compensator_UseAverage(struct sCompensator* s, bool useAverage);
void Compensator_AutoCompensate(struct sCompensator* s);
void Compensator_SetWavelength(struct sCompensator* s, float wavelength);
void Compensator_SetVoltage(struct sCompensator* s, float voltage);
void Compensator_Update(struct sCompensator* s);


#endif /* INC_COMPENSATOR_H_ */
