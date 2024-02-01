/*
 * PID.h
 *
 *  Created on: Aug 10, 2021
 *  
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#define SLOW_HISTORY_SIZE (64)
#define FAST_HISTORY_SIZE (16)
#define INTEGRATOR_DECAY (500)

//  a velocity_lookback of 4 was too noisy
// 12 was not quite responsive enough
#define VELOCITY_LOOKBACK (8)

#include "defs.h"

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

struct sPID_Config
{
  float Target;
  float Kp, Ki, Kd, Il;
  float SlewLimit_degpermin;
  bool OffsetCorrectionEnabled;
};

struct sPID
{
  float FastHistory[FAST_HISTORY_SIZE];
  float SlowHistory[SLOW_HISTORY_SIZE];
  float DeltaT, Ep, Ei, Ed, Effort;
  uint16_t IntegratorCount;
  float ExpWeightedAvg;
  float OffsetCorrection;
  struct sPID_Config Config;
  bool NeedRefresh;
  float SlewLimitedTarget;
};

void PID_InitStruct(struct sPID* s);
void PID_Refresh(struct sPID* s);
void PID_SavePoint(struct sPID* s, float p);
float PID_IntegratorValue(struct sPID* s);
float PID_Velocity_degpersec(struct sPID* s);
float PID_CalculateEffort(struct sPID* s, float p);
void PID_PerformOffsetCorrection(struct sPID* s);
bool PID_AtSteadyState(struct sPID* s);
float PID_ApplySlewLimit(struct sPID* s, float eff);
void PID_LimitSlewRate(struct sPID* s);

#endif /* INC_PID_H_ */

