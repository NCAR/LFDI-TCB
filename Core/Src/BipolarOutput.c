#include "BipolarOutput.h"


void BipolarOutput_InitStruct(struct sBipolarOutput* s, uint8_t BipolarOutputNum, struct sDAC_Channel* sDAC_Channel){
    s->BipolarOutputNum = BipolarOutputNum;
    s->Channel = * sDAC_Channel;
    s->Frequency = 0;
    s->Pulses = 0;
    s->Enabled = false;
    s->Timer = 0;
    s->Voltage = 0;
}

//This sets the frequency of the output
//The frequency is in Hz
//The Timer is set to the number of clock cycles between each pulse
 //TIM2 interrupt runs at 4000 Hz, so each interrupt is 0.25ms
//For frequency F Hz, period = 1/F seconds, half-period = 1/(2F) seconds
//Half-period in timer ticks = (1/(2F)) / (1/4000) = 4000/(2F) = 2000/F
//We store Frequency = 2F to make the calculation: Timer = 4000/Frequency = 4000/(2F) = 2000/F
void BipolarOutput_SetFrequency(struct sBipolarOutput* s, uint16_t frequency){
    if (frequency == 0) {
        s->Frequency = 0;
        s->Timer = 0;
        return;
    }
    // Store 2*frequency because each timer countdown represents half a period
    s->Frequency = frequency * 2;
    // This is a count down timer that will be used to determine when to change the output
    BipolarOutput_TimerReload(s);
}
void BipolarOutput_SetPulses(struct sBipolarOutput* s, uint16_t pulses){
    s->Pulses = pulses;
}

void BipolarOutput_Enable(struct sBipolarOutput* s, bool enable){
    s->Enabled = enable;
    s->Channel.enabled = enable; // Also enable/disable the DAC channel
}




void BipolarOutput_TimerReload(struct sBipolarOutput* s){
    if (s->Frequency == 0) {
        s->Timer = 0;
        return;
    }
    // Timer interrupt runs at 4000 Hz
    // Frequency stored is 2*actual_frequency (half-periods per second)
    // Timer ticks per half-period = 4000 / Frequency
    s->Timer = 4000 / (s->Frequency/2);
    // Ensure minimum timer value of 1 to avoid division issues
    if (s->Timer == 0) {
        s->Timer = 1;
    }
}

void BipolarOutput_SetVoltage(struct sBipolarOutput* s, float voltage){
	s->Voltage = voltage;
	Set_Voltage_Peak_to_Peak(&s->Channel, &s->Voltage);
}
