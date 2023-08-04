#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "defs.h"
#include "DAC.h"




struct sBipolarOutput{
	uint8_t BipolarOutputNum; // The Identifying Number of the GPIO
	struct sDAC_Channel Channel; // The DAC Channel to Output to
    uint16_t Frequency; // The Frequency of the Output
    uint16_t Pulses; // The Number of Pulses to Output
    bool Enabled;
    //A count down timer that will be used to determine when to change the output
    uint16_t Timer;
    float voltage;
};


void BipolarOutput_InitStruct(struct sBipolarOutput* s, uint8_t BipolarOutputNum, struct sDAC_Channel* sDAC_Channel);
void BipolarOutput_SetFrequency(struct sBipolarOutput* s, uint16_t frequency);
void BipolarOutput_SetPulses(struct sBipolarOutput* s, uint16_t pulses);
void BipolarOutput_Enable(struct sBipolarOutput* s, bool enable);
void BipolarOutput_SetVoltage(struct sBipolarOutput* s, float voltage);
void BipolarOutput_TimerReload(struct sBipolarOutput* s);
