/*
 * PID.c
 *
 *  Created on: Aug 10, 2021
 *      Author: damonb
 */

#include "pid.h"

void PID_InitStruct(struct sPID* s)
{
  uint8_t i;
  s->Config.Kp = 0.0f;
  s->Config.Ki = 0.0f;
  s->Config.Kd = 0.0f;
  s->Config.Li = 1.0f;
  s->Config.TargetP = -273.0f;
  s->Config.Enabled = false;
  s->Config.History = 16;
  s->DeltaT = 0.013f;
  s->IntegratorCount = 0;
  for (i=0;i<255;i++)
    s->LastP[i]=0.0f;
  s->Effort = 0.0f;
  s->Ep = 0.0f;
  s->Ei = 0.0f;
  s->Ed = 0.0f;
  s->Config.Frequency = 200;
}

void PID_SavePoint(struct sPID* s, float p)
{
  uint8_t i;
  for (i=(s->Config.History-1); i>0; i--)
    s->LastP[i] = s->LastP[i-1];
  s->LastP[0] = p;
}

float PID_SumError(struct sPID* s)
{
  uint8_t i;
  s->IntegratorCount++;
  if (s->IntegratorCount < s->Config.History)
    return 0;
  else
    s->IntegratorCount = s->Config.History;
  float err = 0;
  for (i=0; i<(s->Config.History); i++)
    err += (s->Config.TargetP - s->LastP[i]);
  err *= (s->Config.Ki);
  if (err < (-s->Config.Li))
    err = -(s->Config.Li);
  if (err > s->Config.Li)
    err = s->Config.Li;
  return err;
}


float PID_Velocity(struct sPID* s)
{
  uint8_t periods = 4;
  if (s->IntegratorCount > periods)
    return (s->LastP[0]-s->LastP[periods])/(s->DeltaT*periods);
  else
    return 0;
}

// calculate the error and return a value between 0 and 1
float PID_CalculateEffort(struct sPID* s, float p)
{
  float eff;
  if ((s->Config.Enabled == false) ||
      (s->Config.TargetP < -200) ||
      (s->DeltaT == 0))
  {
    s->Ep = -1;
    s->Ed = -1;
    s->Ei = -1;
    s->Effort = 0;
    return 0;
  }
  s->Ep = (s->Config.TargetP - p) * (s->Config.Kp);
  s->Ed = (0 - PID_Velocity(s)) * (s->Config.Kd);
  s->Ei = PID_SumError(s);
  eff = s->Ep + s->Ed + s->Ei;

  if (eff > 1)
    eff = 1;
  if (eff < 0)
    eff = 0;
  PID_SavePoint(s, p);
  s->Effort = eff;
  return eff;
}
