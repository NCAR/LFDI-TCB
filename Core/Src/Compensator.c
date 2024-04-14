#include "Compensator.h"


//Initalize the Structure of the Compensator
void Compensator_InitStruct(struct sCompensator* s, struct sTMP117 *t, struct sDAC_Channel *c, double stage){
  s->Sensor = *t;
  s->Channel = *c;
  //is the Channel enabled
  s->Enable = false;
  //Should we be trying to compensate to a specific wavelength
  s->compensate = false;
  //Use the Average stored in the TMP117 struct or the last temperature
  s->useAverage = false;
  //Wavelength to Compensate to
  s->wavelength =  0;
  //if we are not using the auto compensation what should this be outputting
  s->voltage = 0;
  //set the stage that this channel is used on
  Compensator_SetStage(s, stage);
  

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
	float * temperature;
	if(s->useAverage){
		temperature = &s->Sensor.Average;
	}else{
		temperature = &s->Sensor.Temperature[0];
	}
	s->voltage = Wavelength_to_Voltage(&s->wavelength, temperature, s);
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

void Compensator_SetStage(struct sCompensator* s, double stage){
	//Do it this way because its quicker than choosing at runtime
	int i = (int)(stage*1000);
	s->Stage.slope = DefaultSlope;
	s->Stage.intercept = DefaultIntercept;
	if (i == (int)(STAGE1*1000)){
		s->Stage.lookuptable = lookup27;
		s->Stage.freeSpectralRange = STAGE1FSR;
		s->Stage.stageSize = STAGE1;
		s->Stage.stageUpper = STAGE1UPPER;
		s->Stage.stageLower = STAGE1LOWER;
	}else if (i == (int)(STAGE2*1000)){
		s->Stage.lookuptable = lookup54;
		s->Stage.freeSpectralRange = STAGE2FSR;
		s->Stage.stageSize = STAGE2;
		s->Stage.stageUpper = STAGE2UPPER;
		s->Stage.stageLower = STAGE2LOWER;
	}else if (i == (int)(STAGE3*1000)){
		s->Stage.lookuptable = lookup108;
		s->Stage.freeSpectralRange = STAGE3FSR;
		s->Stage.stageSize = STAGE3;
		s->Stage.stageUpper = STAGE3UPPER;
		s->Stage.stageLower = STAGE3LOWER;
	}else if (i == (int)(STAGE4*1000)){
		s->Stage.lookuptable = lookup27;
		s->Stage.freeSpectralRange = STAGE4FSR;
		s->Stage.stageSize = STAGE4;
		s->Stage.stageUpper = STAGE4UPPER;
		s->Stage.stageLower = STAGE4LOWER;
	}else if (i == (int)(STAGE5*1000)){
		s->Stage.lookuptable = lookup54;
		s->Stage.freeSpectralRange = STAGE5FSR;
		s->Stage.stageSize = STAGE5;
		s->Stage.stageUpper = STAGE5UPPER;
		s->Stage.stageLower = STAGE5LOWER;
	}else if (i == (int)(STAGE6*1000)){
		s->Stage.lookuptable = lookup108;
		s->Stage.freeSpectralRange = STAGE6FSR;
		s->Stage.stageSize = STAGE6;
		s->Stage.stageUpper = STAGE6UPPER;
		s->Stage.stageLower = STAGE6LOWER;
	}
}


//Lookup Table to convert a pixel position to a voltage at the base Temperature. 25C
float BaseT_Position_to_BaseT_Voltage(uint16_t position, struct sCompensator* s){
	if(position > 1000){
		return 0.0f;
	}
	//This look up will convert a pixel position to a voltage at 25C
	//The Index is the pixel position and the value is the voltage in mV
	return s->Stage.lookuptable[position]/1000.0f;

}



//This Function Will Convert the temperature to a positional offset
//This is based on the Found relation ship between Position and Temperature
float temperature_position_offset(float* temp, struct sCompensator* s){

	float offset = (float)(s->Stage.slope*(*temp)+s->Stage.intercept);
	return offset;
}


//This Function Will Convert a Wavelength and a temperature to a Voltage
float Wavelength_to_Voltage(float* base_wavelength, float* temp, struct sCompensator* s){


	float base_temp = 25;
	float absolute_BaseT_Offset = temperature_position_offset(&base_temp, s);
	float absolute_Offset = temperature_position_offset(temp, s);
	float wavelength = *base_wavelength + (absolute_BaseT_Offset - absolute_Offset);



	while(wavelength < s->Stage.stageLower){
		wavelength += s->Stage.freeSpectralRange;
	}
	while(wavelength > s->Stage.stageUpper){
		wavelength -= s->Stage.freeSpectralRange;
	}
	//Convert your float wavelength to an int so that we can use it as an index
	//For instance 656.28 would turn into position 280
	uint16_t position = (wavelength - 656.0) * 1000;
	float Voltage = BaseT_Position_to_BaseT_Voltage(position, s);
	return Voltage;
}
