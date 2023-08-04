#include "GPIO.h"


//Initialize the GPIO Struct
void GPIO_InitStruct(struct sGPIO* s, uint8_t GPIONum){
	
	//Switch on the GPIO Number to set the correct pins
	switch (GPIONum){
		case 1:
			s->GPIONum = 1; //Set the number
			s->EnablePort = GPIO1_PORT; // set the port
			s->EnablePin = GPIO1_PIN; // set the pin
			break;
		case 2:
			s->GPIONum = 2;
			s->EnablePort = GPIO2_PORT;
			s->EnablePin = GPIO2_PIN;
			break;
		case 3:
			s->GPIONum = 3;
			s->EnablePort = GPIO3_PORT;
			s->EnablePin = GPIO3_PIN;
			break;
		case 4:
			s->GPIONum = 4;
			s->EnablePort = GPIO4_PORT;
			s->EnablePin = GPIO4_PIN;
			break;
		case 5:
			s->GPIONum = 5;
			s->EnablePort = GPIO5_PORT;
			s->EnablePin = GPIO5_PIN;
			break;
		default:
			break;

	}

	s->Input = false; //Whether the pin is an input (true) or an output (false)
	s->Enabled = false; //if the Pin is currently on
	GPIO_SetState(s, false);
}


//Set the GPIO as an input Input
void GPIO_SetInput(struct sGPIO* s){
	//TODO: Implement this

}

//Set the GPIO as an Output
void GPIO_SetOutput(struct sGPIO* s){
	//TODO: Implement this
}

//Set the GPIO State (High or Low)
void GPIO_SetState(struct sGPIO* s, bool high){
	if(high){
		HAL_GPIO_WritePin(s->EnablePort, s->EnablePin, GPIO_PIN_SET);
		s->Enabled = true;
	}else{
		HAL_GPIO_WritePin(s->EnablePort, s->EnablePin, GPIO_PIN_RESET);
		s->Enabled = false;
	}

}