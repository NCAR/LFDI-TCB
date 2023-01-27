/*
 * UI.h
 *
 *  Created on: Nov 15, 2021
 *      Author: damonb
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include "defs.h"
#include "controller.h"
#include "TuningControlBoard.h"

void ProcessUserInput(struct sTuningControlBoard* TCB, char* buffer);


void ShowControllerConfig(struct sController* Controller, uint8_t);
void ShowCompensatorConfig(struct sCompensator* Compensator, uint8_t index);


void ShowSensor(struct sController* Controller);
void FormatTemperature(char* buffer, double temp);
void ShowEffort(struct sController* Controller);
void ShowAll(struct sController* Controller, bool readable);
void ShowRawHeader(void);
void ShowRawHeaderController(void);
void ShowRawHeaderCompensator();
void ShowMainMenuHeader(void);
void ShowMainHelp(void);
void ShowControllerHelp(void);
void ShowCompensatorHelp(void);

void ProcessUserInput_MainMenu(struct sTuningControlBoard* s, char* input);
void ProcessUserInput_CompensatorMenu(struct sTuningControlBoard* s, char* input);
void ProcessUserInput_ControllerMenu(struct sTuningControlBoard* s, char* input);


void TranslateUserInput_MainMenu(struct sTuningControlBoard * s,char* buffer);
void TranslateUserInput_CompensatorMenu(struct sTuningControlBoard * s,char* buffer);
void TranslateUserInput_ControllerMenu(struct sTuningControlBoard * s,char* buffer);

void SetSensor(struct sTMP117 * sSensor, uint8_t u);

extern volatile uint32_t ADCChannelSamples[4][8];

#endif /* INC_UI_H_ */
