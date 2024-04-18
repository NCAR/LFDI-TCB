/*
 * PID.c
 *
 *  Created on: Aug 10, 2021
 * 
 */

#include <math.h>
#include "USB_VCP_Support.h"
#include "pid.h"
uint8_t HistoryIndex = 0;

#include "funcs.h" // this is for usbsendstring, which is just debugging the offset correction

void PID_InitStruct(struct sPID* s)
{
  s->Config.Kp = 0.0f;
  s->Config.Ki = 0.0f;
  s->Config.Kd = 0.0f;
  s->Config.Il = 1.0f;
  s->Config.SlewLimit_degpermin = 10.0f;
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

  // if we're still slew limited, we're not at steady state
  if (fabs(s->SlewLimitedTarget - s->Config.Target) > 0.1)
	  return false;

  float tmax = s->SlowHistory[0];
  float tmin = tmax;
  for (i=1; i<SLOW_HISTORY_SIZE; i++)
  {
    tmax = MAX(tmax, s->SlowHistory[i]);
    tmin = MIN(tmin, s->SlowHistory[i]);
  }
  if ((tmax-tmin) < 0.05f)
    return true;
  return false;
}

float PID_IntegratorValue(struct sPID* s)
{
  float err = 0;
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
  return (s->SlowHistory[0] - s->SlowHistory[periods]) * (60/periods);
}

void PID_PerformOffsetCorrection(struct sPID* s, uint8_t controller_number)
{
  if (s->Config.OffsetCorrectionEnabled == false)
  {
    s->OffsetCorrection = 0;
    return;
  }

  if (s->IntegratorCount < 500) // not enough history
    return;

  if (PID_AtSteadyState(s) == false)
  {
	  s->OffsetCorrection = 0;  //NOTE this could cause issues, testing now
      return;
  }

  if (s->Config.Il > 0.01f) // if the integrator limit is non-zero
  {
    if ((fabs(s->Ei - s->Config.Il) < 0.01f) || (fabs(s->Ei + s->Config.Il) < 0.01f)) // and the integrator is saturated
      return; // don't calculate a new offset_adjustment
  }

  float offset_adjustment = s->Config.Target - s->ExpWeightedAvg;

  // applying the full offset will cause an immediate jump in the Ei value
  // so we only apply some of the offset_adjustment correction
  offset_adjustment *= 0.7f;

  // limit the offset_adjustment adjustment in case something is really wrong
  offset_adjustment = MAX(offset_adjustment, -0.1f);
  offset_adjustment = MIN(offset_adjustment, 0.1f);

  if ((offset_adjustment < 0) && (s->OffsetCorrection < .001)) // don't allow a negative offset correction
    return;

  if (fabs(offset_adjustment) >= .001) // don't tell us if the correction is tiny
    uprintf("C%u: Applying offset of %.3f.\r", controller_number + 1, offset_adjustment);

  s->OffsetCorrection += offset_adjustment;
  //  s->OffsetCorrection = MAX(s->OffsetCorrection, -1.5f);
  //  s->OffsetCorrection = MIN(s->OffsetCorrection, 1.5f);
  s->OffsetCorrection = MAX(s->OffsetCorrection, 0.0f);
  s->OffsetCorrection = MIN(s->OffsetCorrection, 0.5f);
}

void PID_LimitSlewRate(struct sPID* s)
{
  // we use a temporary target which allows us to control the slew rate
  // if we're close to our target, skip the slew rate limiting
  float CurrentTemperature = s->FastHistory[0];
  float MaximumSafeChange_degpermin = s->Config.SlewLimit_degpermin;


  // deal with the slew target being in the wrong direction. This is frequently triggered on a new enable
  if ((s->Config.Target > CurrentTemperature) && (s->SlewLimitedTarget < CurrentTemperature))
    s->SlewLimitedTarget = CurrentTemperature;

  if ((s->Config.Target < CurrentTemperature) && (s->SlewLimitedTarget > CurrentTemperature))
    s->SlewLimitedTarget = CurrentTemperature;

  // If the SlewLimitedTarget is far away from our current temperature, limit the distance.
  if (fabs(CurrentTemperature - s->SlewLimitedTarget) > (1/s->Config.Kp))
  {
    if (s->Config.Target > CurrentTemperature)
    {
      s->SlewLimitedTarget = CurrentTemperature + (1/s->Config.Kp);
    }
    else
    {
      s->SlewLimitedTarget = CurrentTemperature - (1/s->Config.Kp);
    }
  }

  if (s->SlewLimitedTarget < s->Config.Target)
      s->SlewLimitedTarget += MIN(MaximumSafeChange_degpermin / 60 * s->DeltaT, s->Config.Target - s->SlewLimitedTarget);

  if (s->SlewLimitedTarget > s->Config.Target)
      s->SlewLimitedTarget -= MAX(MaximumSafeChange_degpermin / 60 * s->DeltaT, s->Config.Target - s->SlewLimitedTarget);

  return;
}

// calculate the effort and return a value between 0 and 1
float PID_CalculateEffort(struct sPID* s, float p)
{
  PID_SavePoint(s, p); // we need to save our data point before calculating velocity

  float eff;
  if (s->Config.Target < -200.0f)
  {
    s->Ep = -1.0f;
    s->Ed = -1.0f;
    s->Ei = -1.0f;
    s->Effort = 0.0f;
    return 0.0f;
  }

  PID_LimitSlewRate(s);

  s->Ep = (s->SlewLimitedTarget + s->OffsetCorrection - p) * (s->Config.Kp);
  s->Ed = (0.0f - PID_Velocity_degpersec(s)) * (s->Config.Kd);
  s->Ei = PID_IntegratorValue(s);
  eff = s->Ep + s->Ed + s->Ei;

  eff = MIN(1,eff);
  eff = MAX(0,eff);

  s->Effort = eff;
  return eff;
}

