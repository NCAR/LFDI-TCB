/*
 * UI.h
 *
 *  Created on: 8 4, 2023
 *      Author: Mitchell Jeffers
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include "defs.h"
#include "HeaterController.h"
#include "TuningControlBoard.h"

void ProcessUserInput(struct sTuningControlBoard* TCB, char* buffer);


void ShowHeaterControllerConfig(struct sHeaterController* Controller);
void ShowCompensatorConfig(struct sCompensator* Compensator, uint8_t index);
void ShowGPIOConfig(struct sGPIO* GPIO, uint8_t index);
void ShowBipolarOutputConfig(struct sBipolarOutput* BipolarOutput, uint8_t index);


void FormatTemperature(char* buffer, double temp);
void ShowAll(struct sHeaterController* Controller, bool readable);



void ShowRawHeaderHeaterController(void);
void ShowRawHeaderCompensator();
void ShowRawHeaderGPIO(void);
void ShowRawHeaderBipolarOutput(void);
void ShowMainMenuHeader(void);

void ShowMainMenu(void);
void ShowMainHelp(void);
void ShowHeaterControllerHelp(void);
void ShowCompensatorHelp(void);
void ShowGPIOHelp(void);
void ShowBipolarOutputHelp(void);


void ProcessUserInput_MainMenu(struct sTuningControlBoard* s, char* input);
void ProcessUserInput_CompensatorMenu(struct sTuningControlBoard* s, char* input);
void ProcessUserInput_HeaterControllerMenu(struct sHeaterController HeaterControllers[NUMOFHEATERCONTROLLERS], char* input);
void ProcessUserInput_GPIOMenu(struct sTuningControlBoard* s, char* input);
void ProcessUserInput_BipolarOutputMenu(struct sTuningControlBoard * s, char * buffer);


void TranslateUserInput_MainMenu(struct sTuningControlBoard * s,char* buffer);
void TranslateUserInput_CompensatorMenu(struct sTuningControlBoard * s,char* buffer);
void TranslateUserInput_ControllerMenu(struct sTuningControlBoard * s,char* buffer);
void TranslateUserInput_GPIOMenu(struct sTuningControlBoard * s,char* buffer);
void TranslateUserInput_BipolarOutputMenu(struct sTuningControlBoard * s, char * buffer);


void SetSensor(struct sTMP117 * sSensor, uint8_t u);
extern volatile bool AutoFlood;

#endif /* INC_UI_H_ */
