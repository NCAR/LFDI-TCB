/*
 * PID.h
 *
 *  Created on: Aug 10, 2021
 *      Author: damonb
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#define POSITIONHISTORY (60)
#include "defs.h"

struct sPID_Config
{
  float TargetP;
  float Kp, Ki, Kd, Li;
  bool Enabled;
  uint8_t History;
  uint16_t Frequency;
};

struct sPID
{
  float LastP[256];
  float DeltaT, Ep, Ei, Ed, Effort;
  uint8_t IntegratorCount;
  struct sPID_Config Config;
};


void PID_InitStruct(struct sPID* s);
void PID_SavePoint(struct sPID* s, float p);
float PID_SumError(struct sPID* s);
float PID_Velocity(struct sPID* s);
float PID_CalculateEffort(struct sPID* s, float p);

#endif /* INC_PID_H_ */

