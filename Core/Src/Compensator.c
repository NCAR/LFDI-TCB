#include "Compensator.h"




//Initalize the Structure of the Compensator
void Compensator_InitStruct(struct sCompensator* s, struct sTMP117 *t, struct sDAC_Channel *c){
	//is the Channel enabled
	s->Enable = false;
    //Should we be tryying to Compensate to a specific Wavelength
    s->compensate = false;
    //Use the Average stored in the TMP117 struct or the last temperature
    s->useAverage = false;

    //Wavelength to Compensate to
    s->wavelength =  0;
    //if we are not using the auto compensation what should this be outputting
    s->voltage = 0;
	s->Channel = *c;
	s->Sensor = *t;

	return;
}


void Compensator_enableChannel(struct sCompensator* s, bool Enable){
	s->Enable = Enable;
	s->Channel.enabled = Enable;
	if(!Enable){
		s->voltage = 0;
	}
	return;
}
void Compensator_SetChannel(struct sCompensator* s){
	Set_Voltage_Peak_to_Peak(&s->Channel, &s->voltage);
	return;
}



//@brief this will set whether or not the Compensator will use the average temperature or the last temperature
//@param s is the Compensator struct
//@param useAverage is a bool that will set whether or not the Compensator will use the average temperature or the last temperature
//@return void
void Compensator_UseAverage(struct sCompensator* s, bool useAverage){
	s->useAverage = useAverage;
	return;
}


//This will set the DAC to Compensate Based on the Temperature
void Compensator_AutoCompensate(struct sCompensator* s){
	//Get the Temperature from the Sensor
	double * temperature;
	if(s->useAverage){
		temperature = &s->Sensor.Average;
	}else{
		temperature = &s->Sensor.LastTemperature;
	}
	s->voltage = Wavelength_to_Voltage(&s->wavelength, temperature);
	return;

}


//Set the Wavelength to Compensate to
void Compensator_SetWavelength(struct sCompensator* s, float wavelength){
	s->wavelength = wavelength;
	return;
}


//Set DAC to Voltage
void Compensator_SetVoltage(struct sCompensator* s, float voltage){
	s->voltage = voltage;
	return;
}

//This will update the Compensator
//Should be called Regularly
void Compensator_Update(struct sCompensator* s){
	if(s->compensate){
		Compensator_AutoCompensate(s);
	}	
	//Set the DAC to the Voltage
	//printf("Compensator_Update: %f\n", s->voltage);
	Set_Voltage_Peak_to_Peak(&s->Channel, &s->voltage);
	return;

}
