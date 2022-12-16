#include "Heater_Controller.h"

void set_heater(bool Heater_On){
    if(Heater_On){
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    }else{
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

bool get_heater(){
	/* read PC13 */
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5))
	{
	 return true;
	}else{
		return false;
	}
}
