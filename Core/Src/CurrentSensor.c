#include "CurrentSensor.h"
#include "funcs.h"

static inline float adc_to_volts(uint16_t adc)
{
    return ((float)adc / ADC_MAX_12BIT) * VREF;
}

static inline float volts_to_amps(float vsense)
{
    return vsense / RSHUNT;
}

void CurrentSensor_InitStruct(struct sCurrentSensor* s, ADC_HandleTypeDef* interface, uint8_t channel)
{
    uint8_t i;
    s->Interface = interface;
    s->Channel = channel;
    s->Current = 0.0f;
    s->SamplesInAverage = 16;
    s->State = CURRENTSENSOR_STATE_UNKNOWN;
    for (i = 0; i < 64; i++)
        s->Currents[i] = 0.0f;
    s->Errors = 0;
}

void CurrentSensor_GetCurrent(struct sCurrentSensor* s)
{
    uint8_t i;
    float current_sum = 0.0f;
    uint16_t adc_value = 0;
    
    if (HAL_ADC_Start(s->Interface) != HAL_OK) {
        s->State = CURRENTSENSOR_STATE_READFAIL;
        s->Errors++;
        return;
    }

    // Read through all ranks until we get to the one we want
    // Channel field stores the rank (1, 2, or 3)
    for (uint8_t rank = 1; rank <= s->Channel; rank++) {
        if (HAL_ADC_PollForConversion(s->Interface, ADC_TIMEOUT_MS) != HAL_OK) {
            HAL_ADC_Stop(s->Interface);
            s->State = CURRENTSENSOR_STATE_READFAIL;
            s->Errors++;
            return;
        }
        if (rank == s->Channel) {
            adc_value = (uint16_t)HAL_ADC_GetValue(s->Interface);
        }
    }

    HAL_ADC_Stop(s->Interface);

    // Convert ADC value to current (in mA)
    float voltage = adc_to_volts(adc_value);
    float current_A = volts_to_amps(voltage);
    float current_mA = current_A * 1000.0f;  // Convert to milliamps

    s->State = CURRENTSENSOR_STATE_VALIDCURRENT;
    
    // Shuffle all the current values in the array by 1
    memmove(s->Currents + 1, s->Currents, (s->SamplesInAverage - 1) * sizeof(float));
    
    s->Currents[0] = current_mA;

    // Initialize array if this is the first reading
    if (s->Currents[1] == 0.0f && s->Currents[0] != 0.0f) {
        for (i = 1; i < s->SamplesInAverage; i++) {
            s->Currents[i] = s->Currents[0];
        }
    }

    // Calculate average
    for (i = 0; i < s->SamplesInAverage; i++) {
        current_sum += s->Currents[i];
    }
    s->Current = current_sum / s->SamplesInAverage;
}
