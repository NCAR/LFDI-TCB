#include "BipolarOutput.h"


void BipolarOutput_InitStruct(struct sBipolarOutput* s, uint8_t BipolarOutputNum, struct sDAC_Channel* sDAC_Channel){
    s->BipolarOutputNum = BipolarOutputNum;
    s->Channel = * sDAC_Channel;
    s->Frequency = 0;
    s->Pulses = 0;
    s->Enabled = false;
}

//This sets the frequency of the output
//The frequency is in Hz
//The Timer is set to the number of clock cycles between each pulse
//Currently this uses the htim9 timer which is a 4Hz timer
void BipolarOutput_SetFrequency(struct sBipolarOutput* s, uint16_t frequency){
    s->Frequency = frequency*2;// TODO Do a proper Fix
    //THis is a Count down timer that will be used to determine when to change the output
    BipolarOutput_TimerReload(s);
}
void BipolarOutput_SetPulses(struct sBipolarOutput* s, uint16_t pulses){
    s->Pulses = pulses;
}

void BipolarOutput_Enable(struct sBipolarOutput* s, bool enable){
    s->Enabled = enable;
}

void BipolarOutput_TimerReload(struct sBipolarOutput* s){
    s->Timer = 4000/s->Frequency;
}

void BipolarOutput_SetVoltage(struct sBipolarOutput* s, float voltage){
	s->voltage = voltage;
	Set_Voltage_Peak_to_Peak(&s->Channel, &s->voltage);
}
