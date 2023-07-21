#include "Heater.h"


//Heater1 Definitions
#define HEATER_1_ENABLE_PORT GPIOB
#define HEATER_1_ENABLE_PIN GPIO_PIN_1
#define HEATER_1_READ_PIN
#define HEATER_1_READ_PORT

//Heater2 definitions
#define HEATER_2_ENABLE_PORT GPIOE
#define HEATER_2_ENABLE_PIN GPIO_PIN_8
#define HEATER_2_READ_PIN
#define HEATER_2_READ_PORT

//Heater2 definitions
#define HEATER_3_ENABLE_PORT GPIOE
#define HEATER_3_ENABLE_PIN GPIO_PIN_9
#define HEATER_3_READ_PIN
#define HEATER_3_READ_PORT


//Initialize the Heater Struct
void Heater_InitStruct(struct sHeater* s, uint8_t heater){
	s->HeaterNum = heater;
	if(heater == 1){
		s->EnablePort = HEATER_1_ENABLE_PORT;
		s->EnablePin = HEATER_1_ENABLE_PIN;
  	}else if(heater == 2){
		s->EnablePort = HEATER_2_ENABLE_PORT;
		s->EnablePin = HEATER_2_ENABLE_PIN;
	}else if(heater == 3){
		s->EnablePort = HEATER_3_ENABLE_PORT;
		s->EnablePin = HEATER_3_ENABLE_PIN;
	}else{
		printf("Invalid Heater Number");
	}
	s->Enabled = false;
}

//Set the Heater Enable
void set_HeaterEnable(struct sHeater* s, bool Heater_On){
	if(Heater_On){
    	HAL_GPIO_WritePin(s->EnablePort, s->EnablePin, GPIO_PIN_SET);
		s->Enabled = true;
    }else{
    	HAL_GPIO_WritePin(s->EnablePort, s->EnablePin, GPIO_PIN_RESET);
		s->Enabled = false;
    }
}


