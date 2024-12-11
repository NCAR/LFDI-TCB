/*
 * PID.c
 *
 *  Created on: Aug 10, 2021
 * 
 */

#include "pid.h"
#include <math.h>
uint8_t HistoryIndex = 0;

#include "funcs.h" // this is for usbsendstring, which is just debugging the offset correction

void PID_InitStruct(struct sPID* s)
{
  s->Config.Kp = 0.0f;
  s->Config.Ki = 0.0f;
  s->Config.Kd = 0.0f;
  s->Config.Il = 1.0f;
  s->Config.SlewLimit_degpermin = 10.0f;
  s->Config.SlewLimitDown_degpermin = 20.0f;
  s->Config.Target = -273.0f;
  s->Config.OffsetCorrectionEnabled = false;
  s->DeltaT = 0.13f;
  s->NeedRefresh = false;
  PID_Refresh(s);
}

void PID_Refresh(struct sPID* s)
{
  uint16_t i;
  s->IntegratorCount = 0;
  s->ExpWeightedAvg = -273.0f;
  for (i=0;i<FAST_HISTORY_SIZE;i++)
    s->FastHistory[i]=-273.0f;
  for (i=0;i<SLOW_HISTORY_SIZE;i++)
    s->SlowHistory[i]=-273.0f;
  s->Effort = 0.0f;
  s->Ep = 0.0f;
  s->Ei = 0.0f;
  s->Ed = 0.0f;
  s->SlewLimitedTarget = -273.0f;
  s->OffsetCorrection = 0.0f;
  s->NeedRefresh = false;
}

void PID_SavePoint(struct sPID* s, float p)
{
  uint16_t i;
  // initialize the history if this is our first data point
  if (s->IntegratorCount == 0)
  {
    for (i=0; i < FAST_HISTORY_SIZE; i++)
      s->FastHistory[i] = p;
    for (i=0; i < SLOW_HISTORY_SIZE; i++)
      s->SlowHistory[i] = p;
    s->ExpWeightedAvg = p;
  }
  else
  {
    memmove(s->FastHistory + 1, s->FastHistory, (FAST_HISTORY_SIZE-1)*sizeof(float)); // shift everything down 1
    s->FastHistory[0] = p;
    s->ExpWeightedAvg += (p - s->ExpWeightedAvg) / MIN(s->IntegratorCount+1,INTEGRATOR_DECAY); // 1500 seems way too long
  }
  s->IntegratorCount++;
  if (s->IntegratorCount > 60000)
    s->IntegratorCount = 60000;

  // save every 8th data point (approx every 1.04 seconds) into our slow history
  if (++HistoryIndex >=8)
  {
    HistoryIndex = 0;
    memmove(s->SlowHistory + 1, s->SlowHistory, (SLOW_HISTORY_SIZE-1)*sizeof(float)); // shift everything down 1
    s->SlowHistory[0] = p;
  }
}

bool PID_AtSteadyState(struct sPID* s)
{
  uint16_t i;
  float tmax = s->SlowHistory[0];
  float tmin = tmax;
  for (i=1; i<SLOW_HISTORY_SIZE; i++)
  {
    tmax = MAX(tmax, s->SlowHistory[i]);
    tmin = MIN(tmin, s->SlowHistory[i]);
  }
  if ((tmax-tmin) < 0.1f)
    return true;
  return false;
}

float PID_IntegratorValue(struct sPID* s)
{
  float err = 0;
//  err = (s->Config.Target + s->OffsetCorrection - s->ExpWeightedAvg) * s->Config.Ki;
  err = (s->SlewLimitedTarget + s->OffsetCorrection - s->ExpWeightedAvg) * s->Config.Ki;
  err = MAX(-(s->Config.Il),err);
  err = MIN(s->Config.Il,err);
  return err;
}

float PID_Velocity_degpersec(struct sPID* s) // returns degrees / sec
{
  return (s->FastHistory[0] - s->FastHistory[VELOCITY_LOOKBACK])/(s->DeltaT*VELOCITY_LOOKBACK);
}

float PID_Velocity_degpermin(struct sPID* s)
{
  return PID_Velocity_degpersec(s) * 60;
}

float PID_SlewRate_degpermin(struct sPID* s) // returns degrees / min
{
  uint8_t periods = 15;
//  return (s->SlowHistory[0]-s->SlowHistory[periods])/(s->DeltaT*periods)*60;
  return (s->SlowHistory[0] - s->SlowHistory[periods]) * (60/periods);
}

void PID_PerformOffsetCorrection(struct sPID* s)
{
  char buffer[50];
  if (s->Config.OffsetCorrectionEnabled == false)
    return;
  if (s->IntegratorCount < 500) // not enough history
    return;
  if (PID_AtSteadyState(s) == false)
    return;
  if ((fabs(s->Ei - s->Config.Il) < 0.01f) || (fabs(s->Ei + s->Config.Il) < 0.01f)) // integrator saturated
    return;

  float offset = s->Config.Target - s->ExpWeightedAvg;

  // applying the full offset will cause an immediate jump in the Ei value
  // so we only apply some of the offset correction
  offset *= 0.7f;

  // limit the offset in case something is really wrong

  offset = MAX(offset,-0.3f);
  offset = MIN(offset,0.3f);

  snprintf(buffer, 50, "Applying offset of %.3f.\r", offset);
  USBSendString(buffer);
  s->OffsetCorrection += offset;
  s->OffsetCorrection = MAX(s->OffsetCorrection, -1.5f);
  s->OffsetCorrection = MIN(s->OffsetCorrection, 1.5f);
}


void PID_LimitSlewRate(struct sPID* s)
{
  // we use a temporary target which allows us to control the slew rate
  // if we're close to our target, skip the slew rate limiting
  float CurrentTemperature = s->FastHistory[0];
  float MaximumSafeChange_degpermin = s->Config.SlewLimit_degpermin;
  float MaximumSafeChangeDown_degpermin = s->Config.SlewLimitDown_degpermin;

  if (fabs(s->Config.Target - CurrentTemperature) < 0.3)
  {
    s->SlewLimitedTarget = s->Config.Target;
    return;
  }

  // If the SlewLimitedTarget is far away from our current temperature, start over
  if (fabs(CurrentTemperature - s->SlewLimitedTarget) > (0.5 * MaximumSafeChange_degpermin))
    s->SlewLimitedTarget = CurrentTemperature;

  if (s->SlewLimitedTarget < s->Config.Target)
      s->SlewLimitedTarget += MIN(MaximumSafeChange_degpermin / 60 * s->DeltaT, s->Config.Target - s->SlewLimitedTarget);

  if (s->SlewLimitedTarget > s->Config.Target)
	  s->SlewLimitedTarget -= MAX(MaximumSafeChangeDown_degpermin / 60 * s->DeltaT, s->Config.Target - s->SlewLimitedTarget);

  return;
}

// calculate the effort and return a value between 0 and 1
float PID_CalculateEffort(struct sPID* s, float p)
{
  PID_SavePoint(s, p); // we need to save our data point before calculating velocity

  float eff;
  if (s->Config.Target < -200.0f)
  {
    s->Ep = -1;
    s->Ed = -1;
    s->Ei = -1;
    s->Effort = 0;
    return 0;
  }

  PID_LimitSlewRate(s);

  s->Ep = (s->SlewLimitedTarget + s->OffsetCorrection - p) * (s->Config.Kp);
  s->Ed = (0 - PID_Velocity_degpersec(s)) * (s->Config.Kd);
  s->Ei = PID_IntegratorValue(s);
  eff = s->Ep + s->Ed + s->Ei;

  eff = MIN(1,eff);
  eff = MAX(0,eff);

  s->Effort = eff;
  if(eff > 0.0){
  	  char placeholder  = 0;
  }
  return eff;
}

