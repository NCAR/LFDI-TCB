/*
 * UI.c
 *
 *  Created on: Nov 15, 2021
 * 
 */

#include "UI.h"
#include <ctype.h>
#include "funcs.h"
#include <math.h>
#include "DAC.h"
#include "TuningControlBoard.h"


#define CONTROLLER_MENU 1
#define COMPENSATOR_MENU 2
#define GPIO_MENU 3
#define BIPOLAROUTPUT_MENU 4
#define MAIN_MENU 0
uint8_t SelectedHeaterController = 0;
uint8_t UI_Compensator = 9;
uint8_t UI_GPIO = 9;
uint8_t UI_BipolarOutput = 9;
uint8_t SUB_MENU = 0;

//Get the User Input and process it
//@param Controller: pointer to the controller struct
//@param buffer: pointer to the buffer
void ProcessUserInput(struct sTuningControlBoard* TCB, char* buffer){
  //Get the User Input
  switch(SUB_MENU){
    case MAIN_MENU:
      TranslateUserInput_MainMenu(TCB, buffer);
      break;
    case CONTROLLER_MENU:
      TranslateUserInput_ControllerMenu(TCB, buffer);
      break;
    case COMPENSATOR_MENU:
      TranslateUserInput_CompensatorMenu(TCB, buffer);
      break;
    case GPIO_MENU:
      TranslateUserInput_GPIOMenu(TCB, buffer);
      break;
    case BIPOLAROUTPUT_MENU:
      TranslateUserInput_BipolarOutputMenu(TCB, buffer);
      break;
    default:
      SUB_MENU = MAIN_MENU;
      USBSendString("\nAn Error Occurred\n");
      break;
  }

}

void ShowGPIOConfig(struct sGPIO* GPIO, uint8_t index){
  char buffer[250];
  char enabled[10];
  if (GPIO->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  snprintf(buffer, 200, "GPIO%u: %s\n", index+1, enabled);
  USBSendString(buffer);

}

//Show the Bipolar Output Config
void ShowBipolarOutputConfig(struct sBipolarOutput* BipolarOutput, uint8_t index){
  char buffer[250];
  char enabled[10];
  if (BipolarOutput->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  
  snprintf(buffer, 200, "Bipolar%u: frequency= %04u pulses= %04u  Peak2Peak=%6.2f  %s\n", index+1, BipolarOutput->Frequency, BipolarOutput->Pulses, BipolarOutput->Voltage, enabled);
  USBSendString(buffer);

}

//Show the configuration of the Mechanisms
//=================================================================================================
//Show the Configuration of a Controllers
void ShowHeaterControllerConfig(struct sHeaterController* Controller)
{
  char s1[12];
  static char buf[250] = {0};

  snprintf(buf, sizeof(buf), "C%u: kp=%5.2f kd=%5.2f ki=%5.2f il=%4.2f",
           Controller->HeaterNumber, Controller->PID.Config.Kp, Controller->PID.Config.Kd,
       Controller->PID.Config.Ki, Controller->PID.Config.Il);

  FormatTemperature(s1, Controller->PID.Config.Target);
  snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
           " target=%8s PwmPeriod=%5.3fs SlewLimit=%4.1f",
           s1, Controller->PwmPeriod_ms / 1000.0f, Controller->PID.Config.SlewLimit_degpermin);

  switch (Controller->PID.Config.OffsetCorrectionEnabled)
  {
    case true:
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " offsetcor=enabled");
      break;
    case false:
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " offsetcor=disabled");
      break;
  }
  switch (Controller->Sensor.Address & 0x03)
  {
    case 0:
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " address=00");
      break;
    case 1:
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " address=01");
      break;
    case 2:
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " address=10");
      break;
    case 3:
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " address=11");
      break;
  }
  if (Controller->HeaterEnabled)
    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " heater=enabled\r");
  else
    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), " heater=disabled\r");

  USBSendString(buf);
}


//Show the Configuration of a Controllers
void ShowCompensatorConfig(struct sCompensator* Compensator, uint8_t index){
  char s1[12];
  char s2[12];
  char buffer[300];
  FormatTemperature(s1, Compensator->Sensor.Temperature[0]);
  FormatTemperature(s2, Compensator->Sensor.Average);
  snprintf(buffer, 200, "Comp%u: Peak2Peak=%6.2f  Wave=%6.2f  Temp=%8s Avg=%8s address=",index+1, Compensator->voltage,
       Compensator->wavelength, s1, s2);
  USBSendString(buffer);
  switch (Compensator->Sensor.Address & 0x03)
  {
    case 0:
      USBSendString("00");
      break;
    case 1:
      USBSendString("01");
      break;
    case 2:
      USBSendString("10");
      break;
    case 3:
      USBSendString("11");
      break;
    default:
      break;
  }
  if (Compensator->Enable)
    USBSendString("  ENABLED\n");
  else
    USBSendString("  DISABLED\n");
}






//Show All the information for the Bipolar Output
void ShowAllBipolarOutput(struct sBipolarOutput* BipolarOutput, bool readable, uint8_t index){
  char buffer[250];
  char enabled[10];
  if (BipolarOutput->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  
  snprintf(buffer, 200, "Bipolar%u:\t%04u\t%04u\t%6.2f\t%s\n", index+1, BipolarOutput->Frequency, BipolarOutput->Pulses, BipolarOutput->Voltage, enabled);
  USBSendString(buffer);

}


//Show All the information for the GPIO
void ShowAllGPIO(struct sGPIO* GPIO, bool readable){
  char buffer[250];
  char enabled[10];
  if (GPIO->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  snprintf(buffer, 200, "GPIO%u:\t%s\n", GPIO->GPIONum, enabled);
  USBSendString(buffer);
}




//Show all the information for one of the mechanisms
//=============================================================================
//Show the Status of a Controller
//@brief Show the Status of a Controller
//@param Controller The Controller to show
//@param readable If true, show the status in human readable form

void ShowAllHeaterController(struct sHeaterController* Controller, bool readable, bool autoflood)
{
    char address[3];
    switch (Controller->Sensor.Address & 0x03)
    {
      case 0: strcpy(address, "00"); break;
      case 1: strcpy(address, "01"); break;
      case 2: strcpy(address, "10"); break;
      case 3: strcpy(address, "11"); break;
      default: break;
    }
    char heat_on[4];
    if (Controller->HeaterEnabled)
      strcpy(heat_on, "on ");
    else
      strcpy(heat_on, "off");
    char oc_on[7];
    if (Controller->PID.Config.OffsetCorrectionEnabled)
      snprintf(oc_on, 7, "%5.3f", Controller->PID.OffsetCorrection);
    else
      strcpy(oc_on, "off  ");
    char average[12], last[12], target[12], sltarget[12];
    FormatTemperature(average, Controller->Sensor.Average);
    FormatTemperature(last, Controller->Sensor.Temperature[0]);
    FormatTemperature(target, Controller->PID.Config.Target);
    FormatTemperature(sltarget, Controller->PID.SlewLimitedTarget);

    char sensor[15];
    switch (Controller->Sensor.State)
    {
      case TMP117_STATE_UNKNOWN:      strcpy(sensor, "Unknown");       break;
      case TMP117_STATE_INITFAILED:   strcpy(sensor, "Config failed"); break;
      case TMP117_STATE_REQUESTNOACK: strcpy(sensor, "Req failed");    break;
      case TMP117_STATE_RECEIVEFAIL:  strcpy(sensor, "No response");   break;
      case TMP117_STATE_VALIDTEMP:    strcpy(sensor, "OK");            break;
      default: break;
    }

    static char buf[500];

    if (readable)
    {
      snprintf(buf, sizeof(buf),
               "\rC%u: address: %2s   offsetcor:%7s   heater:%3s   sensor:%s\r",
               Controller->HeaterNumber, address, oc_on, heat_on, sensor);
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "C%u: kp=%5.2f   ep=% 6.1f    temp=%8s  PwmPeriod=%5.3fs\r",
               Controller->HeaterNumber, Controller->PID.Config.Kp, 100 * Controller->PID.Ep, last, Controller->PwmPeriod_ms/1000.0f);
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "C%u: kd=%5.2f   ed=% 6.1f     avg=%8s    \r",
               Controller->HeaterNumber, Controller->PID.Config.Kd, 100 * Controller->PID.Ed, average);
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "C%u: ki=%5.2f   ei=% 6.1f slewtar=%8s    SlewLim=%4.2f d/m\r",
               Controller->HeaterNumber, Controller->PID.Config.Ki, 100 * Controller->PID.Ei, sltarget, Controller->PID.Config.SlewLimit_degpermin);
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "C%u: il=%5.2f  eff=% 6.1f  target=%8s\r",
               Controller->HeaterNumber, Controller->PID.Config.Il, 100 * Controller->PID.Effort, target);
      USBSendString(buf);
    }
    else
    {
      if ((Controller->HeaterNumber == 1) && (autoflood == false))
        ShowRawHeaderHeaterController();
      if (autoflood)
        snprintf(buf, sizeof(buf), "!C");
      else
        snprintf(buf, sizeof(buf), "C");
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "%u\t%5.2f\t%5.2f\t%5.2f\t%4.2f\t",
               Controller->HeaterNumber, Controller->PID.Config.Kp, Controller->PID.Config.Kd, Controller->PID.Config.Ki, Controller->PID.Config.Il);
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "%7.1f\t%7.1f\t%7.1f\t%7.1f\t",
               100 * Controller->PID.Ep, 100 * Controller->PID.Ed, 100 * Controller->PID.Ei, 100 * Controller->PID.Effort);
      snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
               "%7.3f\t%8s\t%8s\t%8s\t%8s\t%2s\t%5.3f\t%s\t%s\t%s\r",
               0.0, last, average, sltarget, target, address, Controller->PwmPeriod_ms/1000.0f, oc_on, heat_on, sensor);
      USBSendString(buf);
    }
}


//Show all the Information about the Compensator
void ShowAllCompensator(struct sCompensator* Compensator, bool readable, uint8_t index)
{

    char address[3];
    switch (Compensator->Sensor.Address & 0x03)
    {
      case 0: strcpy(address, "00"); break;
      case 1: strcpy(address, "01"); break;
      case 2: strcpy(address, "10"); break;
      case 3: strcpy(address, "11"); break;
      default: break;
    }

    char average[12], last[12];
    FormatTemperature(average, Compensator->Sensor.Average);
    FormatTemperature(last, Compensator->Sensor.Temperature[0]);

    char sensor[15];
    switch (Compensator->Sensor.State)
    {
      case TMP117_STATE_UNKNOWN:      strcpy(sensor, "Unknown");       break;
      case TMP117_STATE_INITFAILED:   strcpy(sensor, "Config failed"); break;
      case TMP117_STATE_REQUESTNOACK: strcpy(sensor, "Req failed");    break;
      case TMP117_STATE_RECEIVEFAIL:  strcpy(sensor, "No response");   break;
      case TMP117_STATE_VALIDTEMP:    strcpy(sensor, "OK");            break;
      default: break;

    }
    //Is Auto Compensatilng enabled?
    char compensating[10];
    if (Compensator->compensate)
      strcpy(compensating, "YES");
    else
      strcpy(compensating, "NO");
    
    //Are we using the average or the last temperature?
    char useaverage[10];
    if (Compensator->useAverage)
      strcpy(useaverage, "YES");
    else
      strcpy(useaverage, "NO");
    //Is the Compensator Enabled?
    char enabled[10];
    if (Compensator->Enable)
      strcpy(enabled, "ENABLED ");
    else
      strcpy(enabled, "DISABLED");
      
    static char buffer[250];

    
    snprintf(buffer, sizeof(buffer), "Comp%u\t%5.2f\t%5.2f\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
        index+1, Compensator->voltage, Compensator->wavelength, last, average, compensating, useaverage, address, enabled, sensor);
    USBSendString(buffer);
}

//Show Everything for the TCB
void ShowAllTCB(struct sTuningControlBoard* sTCB){
  ShowRawHeaderHeaterController();
  for(uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++){
    ShowAllHeaterController(&sTCB->HeaterControllers[i], false, AutoFlood);
  }
  ShowRawHeaderCompensator();
  for(uint8_t i = 0; i < NUMOFCOMPENSATORS; i++){
    ShowAllCompensator(&sTCB->Compensator[i], false, i);
  }
  ShowRawHeaderGPIO();
  for(uint8_t i = 0; i < NUMOFGPIO; i++){
    ShowAllGPIO(&sTCB->GPIO[i], false);
  }
  ShowRawHeaderBipolarOutput();
  for(uint8_t i = 0; i < NUMOFBipolarOutputs; i++){
    ShowAllBipolarOutput(&sTCB->BipolarOutput[i], false, i);
  }
}
//Show the Headers for the Raw Data
//=================================================================================================
//Prints the header for the raw data
void ShowRawHeaderHeaterController(void)
{
  static char buf[200];
  snprintf(buf, sizeof(buf), "\tkp\tkd\tki\tli\tep\ted\tei\teffort\tcurr\tinstant_temp\taverage_temp\tslew_target\tfinal_target\ti2c\tperiod\toffset\theater\tsensor\r");
  USBSendString(buf);
}

//Show the Raw Header for the Compensator
void ShowRawHeaderCompensator(void)
{
  static char buf[100];
  snprintf(buf, sizeof(buf), "Comp\tPeak2Peak\tWave\tTemp\tAvg\tAuto\tUseAverage\ti2c\tenabled\tsensor\n");
  USBSendString(buf);
}


//Show the Raw GPIO header
void ShowRawHeaderGPIO(void)
{
  static char buf[100];
  snprintf(buf, sizeof(buf),  "GPIO\tEnabled\n");
  USBSendString(buf);
}

//Show the Raw Header for the Bipolar Output
void ShowRawHeaderBipolarOutput(void)
{
  static char buf[100];
  snprintf(buf, sizeof(buf),  "Bipolar\tfrequency\tpulses\tPeak2Peak\tEnabled\n");
  USBSendString(buf);
}

//Format the Temperature for the Display
//=================================================================================================
//Formats the Float to fit into the Temperature Display
void FormatTemperature(char* buffer, double temp)
{
  if (temp > -100)
  snprintf(buffer, 12, "% 7.3fC", temp);
  else
  snprintf(buffer, 12, "  error ");
}

void SendAutoFlood(struct sHeaterController Controllers[4])
{
  char buf[50] = {0};
  snprintf(buf, sizeof(buf), "!T%.4f\t%.4f\t%.4f\t%.4f\r",
     Controllers[0].Sensor.Temperature[0],
     Controllers[1].Sensor.Temperature[0],
     Controllers[2].Sensor.Temperature[0],
     Controllers[3].Sensor.Temperature[0]);
  USBSendString(buf);
  ShowAllHeaterController(&Controllers[0], false, true);
  ShowAllHeaterController(&Controllers[1], false, true);
  ShowAllHeaterController(&Controllers[2], false, true);
  ShowAllHeaterController(&Controllers[3], false, true);
}


//Case Switched to Process the User Input
//=================================================================================================
//Parse the input from the main Menu
//Top Level Commands
// SET_Temp_[Temp]  -- Sets the target temperature for all controllers
// SET_TEMP_ON -- Turns on the temperature controllers
// SET_TEMP_OFF -- Turns off the temperature controllers
// SET_WAVE_[Wavelength] -- Sets the wavelength for all compensators
// SET_CONT_[Controller]_KP_[KP] -- Sets the controller to be configured
// SET_CONT_[Controller]_KD_[KD] -- Sets the controller to be configured
// SET_CONT_[Controller]_KI_[KI] -- Sets the controller to be configured
// SET_TUNE_ON -- Turns on the tuning for all controllers
// SET_TUNE_OFF -- Turns off the tuning for all controllers
// GET_HK -- Gets the housekeeping data
// GET_TEMP -- Gets the temperature data
// GET_WAVE -- Gets the wavelength data
// GET_TUNE -- Gets the tuning data
// GET_CONT_[Controller] -- Gets the controller data

void SET_TEMP(struct sTuningControlBoard * s, char* input){
    if (strcmp(input, "set_temp_on") == 0){
      for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
      {
        s->HeaterControllers[i].HeaterEnabled = true;
      }
      return;
    }
    //SET_TEMP_OFF
    if (strcmp(input, "set_temp_off") == 0)
    {
      for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
      {
        s->HeaterControllers[i].HeaterEnabled = false;
      }
      return;
    }
    //SET_TEMP_[Temp] check if the input is a valid temperature
    float f = 0;
    if (sscanf(input, "set_temp_%f", &f) == 1)
    {
      for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
      {
        s->HeaterControllers[i].PID.Config.Target = f;
      }
      return;
    }
    USBSendString("Invalid Command\n");   
}

void SET_WAVE(struct sTuningControlBoard * s, char* input){
    //SET_WAVE_[Wavelength] check if the input is a valid wavelength
    float f = 0;
    if (sscanf(input, "set_wave_%f", &f) == 1)
    {
      for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++)
      {
        s->Compensator[i].wavelength = f;
      }
      return;
    }
    USBSendString("Invalid Command\n");
}

void SET_TUNE(struct sTuningControlBoard * s, char* input){
    //SET_TUNE_ON
    if (strcmp(input, "set_tune_on") == 0)
    {
      for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++)
      {
    	s->Compensator[i].compensate = true;
        s->Compensator[i].Enable = true;
      }
      return;
    }
    //SET_TUNE_OFF
    if (strcmp(input, "set_tune_off") == 0)
    {
      for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++)
      {
    	s->Compensator[i].compensate = false;
        s->Compensator[i].Enable = false;
      }
      return;
    }
    USBSendString("Invalid Command\n");
}

void SET_HEATER(struct  sTuningControlBoard *s, char* input){
    //SET_HEATER_[Controller]_KP_[KP]
    uint8_t u = 0;
    char output[250];
    float f = 0;
    if (sscanf(input, "set_heater_%u_kp_%f", &u, &f) == 2)
    {
      if (u < NUMOFHEATERCONTROLLERS)
      {
        s->HeaterControllers[u].PID.Config.Kp = f;
        sprintf(output, "Controller %u Kp set to %f\n", u, f);
        USBSendString(output);
      }
      return;
    }
    //SET_HEATER_[Controller]_KD_[KD]
    if (sscanf(input, "set_heater_%u_kd_%f", &u, &f) == 2)
    {
      if (u < NUMOFHEATERCONTROLLERS)
      {
        s->HeaterControllers[u].PID.Config.Kd = f;
        sprintf(output, "Controller %u Kd set to %f\n", u, f);
        USBSendString(output);
      }
      return;
    }
    //SET_HEATER_[Controller]_KI_[KI]
    if (sscanf(input, "set_heater_%u_ki_%f", &u, &f) == 2)
    {
      if (u < NUMOFHEATERCONTROLLERS)
      {
        s->HeaterControllers[u].PID.Config.Ki = f;
        sprintf(output, "Controller %u Ki set to %f\n", u, f);
        USBSendString(output);
      }
      return;
    }
    USBSendString("Invalid Command\n");
}

void ProcessUserInput_MainMenu(struct sTuningControlBoard * s,char* input){
  //SET Commands
  if (strncmp(input, "set_", 4) == 0)
  {
    //SET_TEMP_[Temp]
    if (strncmp(input, "set_temp_", 9) == 0)
    {
      SET_TEMP(s, input);
      return;
    }
    //SET_WAVE_[Wavelength]
    if (strncmp(input, "set_wave_", 9) == 0)
    {
      SET_WAVE(s, input);
      return;
    }
    //SET_TUNE_[ON/OFF]
    if (strncmp(input, "set_tune_", 9) == 0)
    {
      SET_TUNE(s, input);
      return;
    }
    //SET_HEATER_[Controller]_KP_[KP]
    if (strncmp(input, "set_heater_", 9) == 0)
    {
      SET_HEATER(s, input);
      return;
    }
  }

  //Pull up the Controller Sub menu
  if (strcmp(input, "cont") == 0)
  { 
    SUB_MENU = CONTROLLER_MENU;
    return;
  }
  //Pull up the Compensator Sub menu
  else if (strcmp(input, "comp") == 0)
  { 
    SUB_MENU = COMPENSATOR_MENU;
    return;
  //Pull up the GPIO Sub menu
  }else if(strcmp(input, "gpio")==0){
	  SUB_MENU = GPIO_MENU;
	  return;
  }else if(strcmp(input, "bipo")== 0){
	  SUB_MENU = BIPOLAROUTPUT_MENU;
	  return;
  }

  //TODO This Should print all of the interesting Values from the TCB
  else if ((strcmp(input, "update") == 0) || (strcmp(input, "u") == 0))
  {
    //ShowUpdate();
  }
  else if ((strcmp(input, "raw") == 0) || (strcmp(input, "r") == 0))
  {
    ShowAllTCB(s);
  }
  //This Should reset the TCB  to the default values
  else if ((strcmp(input, "wipe") == 0) || (strcmp(input, "w") == 0))
  {
    //WipeConfig();
    USBSendString("Wiped Configuration\n");
  }

  //This resets the TCB
  else if (strcmp(input, "b") == 0)
  {
    USBSendString("Bouncing...\n");
    HAL_Delay(1000);
    NVIC_SystemReset();
  }
  //This loads the TCB from the EEPROM
  else if (strcmp(input, "l") == 0)
  {
    //LoadConfig();
    USBSendString("no EEPROM Cannot Load Configuration\n");
  }
  //This saves the TCB to the EEPROM
  else if (strcmp(input, "s") == 0)
  {
    //SaveConfig(); TODO add implementation for SD Card here
    USBSendString("no EEPROM Cannot Save Config\n");
  }
  //This prints the help menu
  else if (strcmp(input, "h") == 0)
  {
    ShowMainHelp();
  }
  else
  {
    USBSendString("Unknown Command\n");
  }
}

//Parse the input from the Compensator Context Menu
void ProcessUserInput_CompensatorMenu(struct sTuningControlBoard * s,char* input)
{
  uint8_t u = 0;
  char output[250];
  char c;
  float f = 0;

  //Send  to main menu
  if (strcmp(input, "m") == 0)
  {
    SUB_MENU = MAIN_MENU;
    UI_Compensator = 9;
    return;
  }
  //Show Help
  else if (strcmp(input, "h") == 0)
  {
    ShowCompensatorHelp();
    if (UI_Compensator == 9)
    {
      USBSendString("No Compensator Selected\n");
      return;
    }
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  } 
  if ((strcmp((char*) input, "r") == 0)){
    ShowRawHeaderCompensator();
    ShowAllCompensator(&s->Compensator[UI_Compensator],false, UI_Compensator);
    return;
  }
  //Set the Compensator
  if ((strcmp((char*) input, "1") == 0) || (strcmp((char*) input, "c1") == 0))
  {
    UI_Compensator = 0;
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  }
  
  if ((strcmp((char*) input, "2") == 0) || (strcmp((char*) input, "c2") == 0))
  {
    UI_Compensator = 1;
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  }

  if ((strcmp((char*) input, "3") == 0) || (strcmp((char*) input, "c3") == 0))
  {
    UI_Compensator = 2;
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  }
  if ((strcmp((char*) input, "4") == 0) || (strcmp((char*) input, "c4") == 0))
  {
    UI_Compensator = 3;
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  }
  if ((strcmp((char*) input, "5") == 0) || (strcmp((char*) input, "c5") == 0))
  {
    UI_Compensator = 4;
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  }
  if ((strcmp((char*) input, "6") == 0) || (strcmp((char*) input, "c6") == 0))
  {
    UI_Compensator = 5;
    ShowCompensatorConfig(&s->Compensator[UI_Compensator], UI_Compensator);
    return;
  }

  //If we dont have a valid controller selected dont let user proceed
  if (UI_Compensator == 9)
  {
    USBSendString("No controller selected.\n");
    return;
  }

  //Turn on the Temperature Controllers
  if (strcmp((char*) input, "e") == 0)
  {

      sprintf(output, "Compensator %d Enabled.\n", UI_Compensator+1);    
    	USBSendString(output);
      Compensator_enableChannel(&s->Compensator[UI_Compensator], true);
      return;
   }

  //Turn off the Temperature Controllers
  if (strcmp((char*) input, "d") == 0)
  {
    //Check to see if the controller is a Compensator
    if (UI_Compensator < 6){
      sprintf(output, "Compensator %d Disabled.\n", UI_Compensator+1);    
    	USBSendString(output);
      Compensator_enableChannel(&s->Compensator[UI_Compensator], false);
      return;
    }
  }

  if (strcmp((char*) input, "co") == 0){
    if(s->Compensator[UI_Compensator].compensate){
      s->Compensator[UI_Compensator].compensate = false;
      sprintf(output, "Compensator %d Auto Compensating Off.\n", UI_Compensator+1);
      USBSendString(output);
    } else {
      s->Compensator[UI_Compensator].compensate = true;
      sprintf(output, "Compensator %d Auto Compensating On.\n", UI_Compensator+1);
      USBSendString(output);
    }
    return;
  }

  if (sscanf((char*) input, "%c%f", &c, &f) == 2){
    u = (uint16_t) f;
    switch (c){
      case 'c':
        USBSendString("Invalid Compensator Number.\n");
        return;
        break;
      case 'a':
        //Set the sensor address
        SetSensor(&s->Compensator[UI_Compensator].Sensor, u);
        sprintf(output, "Compensator %d Sensor Address Set to %d.\n", UI_Compensator+1, u);
        USBSendString(output);
        return;
        break;

      case 'v':
        //Check the Voltage is greater than 0 and less than max peak to peak
        if (f >= 0 && f < s->Compensator[UI_Compensator].Channel.max_peak2peak){
          s->Compensator[UI_Compensator].voltage = f;
          s->Compensator[UI_Compensator].compensate = false;
          //Print out the string with the Compensator number and voltage
          sprintf(output, "Compensator %d Voltage Set to %f.\n", UI_Compensator+1, f);    
      	  USBSendString(output);
          break;
          return;
        } else {
          USBSendString("Invalid Voltage.\n");
          break;
          return;
        }

      case 'w':
        //Set the wavelength
        //Check that the wavelength is greater than 0 and less than 500
        if (f > 656 && f < 657){
          s->Compensator[UI_Compensator].wavelength = f;
          sprintf(output, "Compensator %d Wavelength Set to %f.\n", UI_Compensator+1, f);
          USBSendString(output);
          break;
          return;
        } else {
          USBSendString("Invalid Wavelength. Must be between 0 and 500\n");
          break;
          return;
        }
        //Stage Setting Selected
      case 's':
        //round the float to one decimal place
        f = roundf(f * 10) / 10;
        int i = (int)(f*1000);
              //Set the wavelength
              //Check that stage is defined
        if (i == (int)(STAGE1*1000)){
        	Compensator_SetStage(&s->Compensator[UI_Compensator], STAGE1);
		}else if(i == (int)(STAGE2*1000)){
			Compensator_SetStage(&s->Compensator[UI_Compensator], STAGE2);
		}else if(i == (int)(STAGE3*1000)){
			Compensator_SetStage(&s->Compensator[UI_Compensator], STAGE3);
		}else if(i == (int)(STAGE4*1000)){
			Compensator_SetStage(&s->Compensator[UI_Compensator], STAGE4);
		}else if(i == (int)(STAGE5*1000)){
			Compensator_SetStage(&s->Compensator[UI_Compensator], STAGE5);
		}else if(i == (int)(STAGE6*1000)){
			Compensator_SetStage(&s->Compensator[UI_Compensator], STAGE6);
		}else{
			USBSendString("Invalid Stage.\n");
			break;
		}
		sprintf(output, "Compensator %d Stage Set to %f.\n", UI_Compensator+1, f);
		break;

      default:
        USBSendString("Unknown Command.\n");
        break;
        return;
    }
  }
}

//Parse the input from the Controller Context Menu
void ProcessUserInput_HeaterControllerMenu(struct sTuningControlBoard * tcb, char* input)
{
  uint16_t i = 0;
  uint16_t u = 0;
  char output[250];
  char c1, c2;
  float f = 0;

  if ((strcmp(input, "u") == 0) || (strcmp(input, "/") == 0))
  {
    for (i=0; i<4; i++)
      ShowAllHeaterController(&tcb->HeaterControllers[i], true, false);
    return;
  }

  if (strcmp(input, "*") == 0)
  {
    if (AutoFlood)
      AutoFlood = false;
    else
      AutoFlood = true;
    return;
  }

  if (strcmp(input, "r") == 0)
  {
    ShowRawHeaderHeaterController();
    for (i=0; i<4; i++)
      ShowAllHeaterController(&tcb->HeaterControllers[i], false, false);
    return;
  }

  if (strcmp(input, "bounce") == 0)
  {
    NVIC_SystemReset();
  }


  if ((strcmp(input, "1") == 0) || (strcmp(input, "c1") == 0))
  {
    SelectedHeaterController = 0;
    ShowHeaterControllerConfig(&tcb->HeaterControllers[SelectedHeaterController]);
    return;
  }

  if ((strcmp(input, "2") == 0) || (strcmp(input, "c2") == 0))
  {
    SelectedHeaterController = 1;
    ShowHeaterControllerConfig(&tcb->HeaterControllers[SelectedHeaterController]);
    return;
  }

  if ((strcmp(input, "3") == 0) || (strcmp(input, "c3") == 0))
  {
    SelectedHeaterController = 2;
    ShowHeaterControllerConfig(&tcb->HeaterControllers[SelectedHeaterController]);
    return;
  }

  if ((strcmp(input, "4") == 0) || (strcmp(input, "c4") == 0))
  {
    SelectedHeaterController = 3;
    ShowHeaterControllerConfig(&tcb->HeaterControllers[SelectedHeaterController]);
    return;
  }
  if (strcmp(input, "ec") == 0)
  {
	snprintf(output, sizeof(output), "Controller %i heater output enabled.\r", SelectedHeaterController + 1);
    USBSendString(output);
    tcb->HeaterControllers[SelectedHeaterController].HeaterEnabled = true;
    tcb->HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
    return;
  }

  if (strcmp(input, "dc") == 0)
  {
	snprintf(output, sizeof(output), "Controller %i heater output disabled.\r", SelectedHeaterController + 1);
    USBSendString(output);
    tcb->HeaterControllers[SelectedHeaterController].HeaterEnabled = false;
    return;
  }

  if (strcmp(input, "eo") == 0)
  {
    USBSendString("Offset correction enabled.\r");
    tcb->HeaterControllers[SelectedHeaterController].PID.Config.OffsetCorrectionEnabled = true;
    return;
  }

  if (strcmp(input, "do") == 0)
  {
    USBSendString("Offset correction disabled.\r");
    tcb->HeaterControllers[SelectedHeaterController].PID.Config.OffsetCorrectionEnabled = false;
    tcb->HeaterControllers[SelectedHeaterController].PID.OffsetCorrection = 0.0f;
    return;
  }

  if (sscanf(input, "%c%c%f", &c1, &c2, &f) == 3)
  {
    u = (uint16_t) f;
    if ((c1 == 'a') && (c2 == 'd'))
    {
      switch (u)
      {
        case 0:
          USBSendString("Address set to 0b 10 01 00 0x.\r");
          tcb->HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001000;
          tcb->HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        case 10:
          USBSendString("Address set to 0b 10 01 01 0x.\r");
          tcb->HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001010;
          tcb->HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        case 1:
          USBSendString("Address set to 0b 10 01 00 1x.\r");
          tcb->HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001001;
          tcb->HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        case 11:
          USBSendString("Address set to 0b 10 01 01 1x.\r");
          tcb->HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001011;
          tcb->HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        default:
          USBSendString("Invalid Address.\r");
          return;
          break;
      }
    }
    if ((c1 == 'k') && (c2 == 'p'))
    {
      if (f < 0)
        USBSendString("Invalid value.\r");
      else
      {
    	snprintf(output, sizeof(output), "kp set to %f.\r", f);
        USBSendString(output);
        tcb->HeaterControllers[SelectedHeaterController].PID.Config.Kp = f;
      }
      return;
    }
    if ((c1 == 'k') && (c2 == 'i'))
    {
      if (f < 0)
        USBSendString("Invalid value.\r");
      else
      {
    	snprintf(output, sizeof(output), "ki set to %f.\r", f);
        USBSendString(output);
        tcb->HeaterControllers[SelectedHeaterController].PID.Config.Ki = f;
      }
      return;
    }
    if ((c1 == 'k') && (c2 == 'd'))
    {
      if (f < 0)
        USBSendString("Invalid value.\r");
      else
      {
    	snprintf(output, sizeof(output), "kd set to %f.\r", f);
        USBSendString(output);
        tcb->HeaterControllers[SelectedHeaterController].PID.Config.Kd = f;
      }
      return;
    }
/*    if ((c1 == 't') && (c2 == 'o'))
    {
      snprintf(output, sizeof(output), "Watchdog timeout set to %u seconds.\r", u);
      USBSendString(output);
      WatchdogTimeout = u;
      return;
    }
    */
    if ((c1 == 'p') && (c2 == 'p'))
    {
      if ((f < 20) || (f > 1000))
        USBSendString("Invalid value.\r");
      else
      {
    	snprintf(output, sizeof(output), "PWM period is %.3fs (~ %.1f Hz).\r", (f/1000), (1000/f));
        USBSendString(output);
        tcb->HeaterControllers[SelectedHeaterController].PwmPeriod_ms = f;
      }
      return;
    }
    if ((c1 == 'i') && (c2 == 'l'))
    {
      if (f < 0)
        USBSendString("Invalid value.\r");
      else
      {
    	snprintf(output, sizeof(output), "Integrator limit set to %f.\r", f);
        USBSendString(output);
        tcb->HeaterControllers[SelectedHeaterController].PID.Config.Il = f;
      }
      return;
    }
    if ((c1 == 't') && (c2 == 'g'))
    {
      snprintf(output, sizeof(output), "Target temperature set to %f.\r", f);
      USBSendString(output);
      tcb->HeaterControllers[SelectedHeaterController].PID.Config.Target = f;
      tcb->HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
      return;
    }
    if ((c1 == 's') && (c2 == 'l'))
    {
      snprintf(output, sizeof(output), "Slew limit set to %.2f deg/min.\r", f);
      USBSendString(output);
      tcb->HeaterControllers[SelectedHeaterController].PID.Config.SlewLimit_degpermin = f;
      return;
    }
  }
  USBSendString("Unknown command: ");
  USBSendString((char*) input);
  USBSendString("\r");
  return;
}


//Parse the input from the GPIO Context Menu
void ProcessUserInput_GPIOMenu(struct sTuningControlBoard * s, char * buffer){
  char c;
  float f = 0;

  //Send  to main menu
  if (strcmp(buffer, "m") == 0)
  {
    SUB_MENU = MAIN_MENU;
    UI_GPIO = 9;

    return;
  }
   if ((strcmp(buffer, "?") == 0) || (strcmp(buffer, "help") == 0))
  {
    ShowGPIOHelp();
    if (UI_GPIO == 9)
      USBSendString("No GPIO selected.\n");
    else
    {
      /*
      ShowSensor(&TCB.Controller);
      ShowControllerConfig(&TCB.Controller);
      ShowEffort(&TCB.Controller);
      USBSendString("\n");
      */
      ShowAllGPIO(&s->GPIO[UI_GPIO], true);
    }
    return;
  }
  //Select the controller
  if ((strcmp(buffer, "1") == 0) || (strcmp(buffer, "g1") == 0))
  {
    UI_GPIO = 0;
    ShowGPIOConfig(&s->GPIO[UI_GPIO], UI_GPIO);
    return;
  }
  if ((strcmp(buffer, "2") == 0) || (strcmp(buffer, "g2") == 0))
  {
    UI_GPIO = 1;
    ShowGPIOConfig(&s->GPIO[UI_GPIO], UI_GPIO);
    //USBSendString("Controller 2 not implemented.\n");
    //ShowControllerConfig(Controllers);
    return;
  }
  if ((strcmp(buffer, "3") == 0) || (strcmp(buffer, "g3") == 0))
  {
    UI_GPIO = 2;
    ShowGPIOConfig(&s->GPIO[UI_GPIO], UI_GPIO);
    //USBSendString("Controller 3 not implemented.\n");
    //ShowControllerConfig(Controllers);
    return;
  }
  if ((strcmp(buffer, "4") == 0) || (strcmp(buffer, "g4") == 0))
  {
    UI_GPIO = 3;
    ShowGPIOConfig(&s->GPIO[UI_GPIO], UI_GPIO);
    //ShowControllerConfig(Controllers);
    return;
  }
  if ((strcmp(buffer, "5") == 0) || (strcmp(buffer, "g5") == 0))
  {
    UI_GPIO = 4;
    ShowGPIOConfig(&s->GPIO[UI_GPIO], UI_GPIO);
    //ShowControllerConfig(Controllers);
    return;
  }
  //If there is no valid Controller Selected
  if (UI_GPIO == 9)
  {
    USBSendString("No GPIO selected.\n");
    return;
  }

  //Print the Status all the Controllers
  if ((strcmp(buffer, "u") == 0) || (strcmp(buffer, "/") == 0))
  {
    ShowAllGPIO(&s->GPIO[UI_GPIO], true);
    return;
  }
  //Print the Status all the Controllers in non readable format
  if (strcmp(buffer, "r") == 0)
  {
    ShowRawHeaderGPIO();
    ShowAllGPIO(&s->GPIO[UI_GPIO], false);//Todo Implement this
    return;
  }

  //Enable the Controllers
  if (strcmp(buffer, "e") == 0)
  {
    USBSendString("GPIO enabled.\n");
    GPIO_SetState(&s->GPIO[UI_GPIO], true);
    return;
  }
  //Disable the Controllers
  if (strcmp(buffer, "d") == 0)
  {
    USBSendString("GPIO disabled.\n");
    GPIO_SetState(&s->GPIO[UI_GPIO], true);
    return;
  }


  if (sscanf(buffer, "%c%f", &c, &f) == 2)
  {
    //Switch on the character
    switch (c)
    {
      //User is trying to set the Channel
      case 'c':
        // we shouldn't get here if a valid number was used
        USBSendString("Invalid controller number.\n");
        return;
        break;
      
      default:
        break;
    }
  }
  USBSendString("Unknown command.\n");
  return;

}

//Process User Input for the Bipolar Output Menu
void ProcessUserInput_BipolarOutputMenu(struct sTuningControlBoard * s, char * buffer){
  
  uint8_t u = 0;
  char output[250];
  char c;
  float f = 0;

  //Send  to main menu
  if (strcmp(buffer, "m") == 0)
  {
    SUB_MENU = MAIN_MENU;
    UI_GPIO = 9;

    return;
  }
   if ((strcmp((char*) buffer, "?") == 0) || (strcmp((char*) buffer, "help") == 0))
  {
    ShowBipolarOutputHelp();
    if (UI_GPIO == 9)
      USBSendString("No Bipolar Output selected.\n");
    else
    {
      /*
      ShowSensor(&TCB.Controller);
      ShowControllerConfig(&TCB.Controller);
      ShowEffort(&TCB.Controller);
      USBSendString("\n");
      */
      ShowAllBipolarOutput(&s->BipolarOutput[UI_BipolarOutput], true, UI_BipolarOutput + 1);
    }
    return;
  }
  //Select the BipolarOutput
  if ((strcmp((char*) buffer, "1") == 0) || (strcmp((char*) buffer, "b1") == 0))
  {
    UI_BipolarOutput = 0;
    ShowBipolarOutputConfig(&s->BipolarOutput[UI_BipolarOutput], UI_BipolarOutput);
    return;
  }
  if ((strcmp((char*) buffer, "2") == 0) || (strcmp((char*) buffer, "b2") == 0))
  {
    UI_BipolarOutput = 1;
    ShowBipolarOutputConfig(&s->BipolarOutput[UI_BipolarOutput], UI_BipolarOutput);
    return;
  }
  //If there is no valid Controller Selected
  if (UI_BipolarOutput == 9)
  {
    USBSendString("No Bipolar Output selected.\n");
    return;
  }
  

  //Print the Status all the Controllers
  if ((strcmp((char*) buffer, "u") == 0) || (strcmp((char*) buffer, "/") == 0))
  {
    ShowAllBipolarOutput(&s->BipolarOutput[UI_BipolarOutput], true, UI_BipolarOutput+1);
    return;
  }
  //Print the Status all the Controllers in non readable format
  if (strcmp((char*) buffer, "r") == 0)
  {
    ShowRawHeaderBipolarOutput();
    ShowAllBipolarOutput(&s->BipolarOutput[UI_BipolarOutput], false, UI_BipolarOutput+1);
    return;
  }

  //Enable the Controllers
  if (strcmp((char*) buffer, "e") == 0)
  {
    USBSendString("BipolarOutput enabled.\n");
    BipolarOutput_Enable(&s->BipolarOutput[UI_BipolarOutput], true);
    return;
  }
  //Disable the Controllers
  if (strcmp((char*) buffer, "d") == 0)
  {
    USBSendString("BipolarOutput disabled.\n");
    BipolarOutput_Enable(&s->BipolarOutput[UI_BipolarOutput], true);
    return;
  }


  if (sscanf((char*) buffer, "%c%f", &c, &f) == 2)
  {
    //Convert the float to an integer
    u = (uint16_t) f;
    //Switch on the character
    switch (c)
    {
      //User is trying to set the Channel
      case 'b':
        // we shouldn't get here if a valid number was used
        USBSendString("Invalid controller number.\n");
        return;
        break;
      case 'f':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Frequency set to %f.\n", f);
          USBSendString(output);
          BipolarOutput_SetFrequency(&s->BipolarOutput[UI_BipolarOutput], f);
        }
        break;
      case 'p':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Pulses set to %f.\n", f);
          USBSendString(output);
          BipolarOutput_SetPulses(&s->BipolarOutput[UI_BipolarOutput], u);
        }
        break;

       case 'v':
        //Check the Voltage is greater than 0 and less than max peak to peak
        if (f >= 0 && f < s->BipolarOutput[UI_BipolarOutput].Channel.max_peak2peak){
          BipolarOutput_SetVoltage(&s->BipolarOutput[UI_BipolarOutput], f);
          //Print out the string with the Compensator number and voltage
          sprintf(output, "BipolarOutput %d Voltage Set to %f.\n", UI_BipolarOutput+1, f);    
      	  USBSendString(output);
          break;
          return;
        } else {
          USBSendString("Invalid Voltage.\n");
          break;
          return;
        }

        break;
      default:
        break;
    }
  }
  USBSendString("Unknown command.\n");
  return;
}



void TranslateUserInput_BipolarOutputMenu(struct sTuningControlBoard * s, char * buffer){
  //Make everything lowercase
  for (int i=0; buffer[i]; i++){
    buffer[i] = tolower(buffer[i]);
  }
  replacestr(buffer, "=", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, "bipolar", "b");
  replacestr(buffer, "enable", "e");
  replacestr(buffer, "disable", "d");
  replacestr(buffer, "voltage", "v");
  replacestr(buffer, "frequency", "f");
  replacestr(buffer, "pulses", "p");
  replacestr(buffer, "help", "h");
  replacestr(buffer, "raw", "r");
  replacestr(buffer, "main", "m");
  ProcessUserInput_BipolarOutputMenu(s, buffer);

}

void TranslateUserInput_GPIOMenu(struct sTuningControlBoard * s, char * buffer){
  //Make everything lowercase
  for (int i=0; buffer[i]; i++){
    buffer[i] = tolower(buffer[i]);
  }
  replacestr(buffer, "=", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, "gpio", "g");
  replacestr(buffer, "enable", "e");
  replacestr(buffer, "disable", "d");
  replacestr(buffer, "input", "i");
  replacestr(buffer, "output", "o");
  replacestr(buffer, "help", "h");
  replacestr(buffer, "raw", "r");
  replacestr(buffer, "main", "m");
  ProcessUserInput_GPIOMenu(s, buffer);
}


//format the User input
//=================================================================================================
//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_MainMenu(struct sTuningControlBoard * s, char* buffer)
{
  //Make everything lowercase
  for (int i=0; buffer[i]; i++){
    buffer[i] = tolower(buffer[i]);
  }
  replacestr(buffer, "=", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, "save", "s");
  replacestr(buffer, "load", "l");
  replacestr(buffer, "update", "u");
  replacestr(buffer, "raw", "r");
  replacestr(buffer, "bounce", "b");
  replacestr(buffer, "wipe", "w");
  replacestr(buffer, "help", "h");
  replacestr(buffer, "controller", "cont");
  replacestr(buffer, "compensator", "comp");\

  ProcessUserInput_MainMenu(s, buffer);

}

//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_CompensatorMenu(struct sTuningControlBoard * s, char * buffer)
{
  //Make everything lowercase
  for (int i=0; buffer[i]; i++){
    buffer[i] = tolower(buffer[i]);
  }

  replacestr(buffer, "=", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, "channel", "c");
  replacestr(buffer, "volt", "v");
  replacestr(buffer, "comp", "co");
  replacestr(buffer, "wave", "w");
  replacestr(buffer, "stage", "s");
  replacestr(buffer, "enable", "e");
  replacestr(buffer, "disable", "d");
  replacestr(buffer, "address", "a");
  replacestr(buffer, "main", "m");
  replacestr(buffer, "help", "h");
  replacestr(buffer, "raw", "r");
  ProcessUserInput_CompensatorMenu(s, buffer);
  
}

//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_ControllerMenu(struct sTuningControlBoard * s, char * buffer){
  //Make everything lowercase
  for (int i=0; buffer[i]; i++){
    buffer[i] = tolower(buffer[i]);
  }
  replacestr(buffer, "=", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  replacestr(buffer, " ", "");
  ProcessUserInput_HeaterControllerMenu(s, buffer);
}



//Headers to Show the User to let them Know where they are
//=================================================================================================
void ShowMainMenuHeader(void)
{
  USBSendString("Main Menu\n");
  USBSendString("=========\n");
}

void ShowCompensatorMenuHeader(void)
{
  USBSendString("Compensator Menu\n");
  USBSendString("================\n");
}

void ShowControllerMenuHeader(void)
{
  USBSendString("Controller Menu\n");
  USBSendString("===============\n");
}

void ShowGPIOMenuHeader(void){
	USBSendString("GPIO Menu\n");
	USBSendString("===============\n");
}

void ShowBipolarOutputMenuHeader(void){
  USBSendString("Bipolar Output Menu\n");
  USBSendString("===============\n");
}

//Help Menus for End User
//=================================================================================================
//Main Menu help Text
void ShowMainHelp(void)
{
    USBSendString("\nLFDI TCB Firmware v1.5\n");
    USBSendString("\nLFDI TCB Hardware Rev v1\n");
    ShowMainMenuHeader();
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\n");
    USBSendString("\n");
    USBSendString("Controller      -- Open The Controller Context Menu\n");
    USBSendString("Compensator     -- Open The Compensator Context Menu\n");
    USBSendString("Update          -- shows the status of all of the controllers and Compensators\n");
    USBSendString("Raw             -- shows an easily parsable version of Update\n");
    USBSendString("Wipe            -- wipes the existing configuration and load new defaults\n");
    USBSendString("Bounce          -- performs a power-cycle / reboot on the system\n");
    USBSendString("Load            -- reloads the previously saved values (automatic at power-on)\n");
    USBSendString("Save            -- saves the currently configured values\n");
    USBSendString("\n");
}

//Show the Controller Context Menu
void ShowHeaterControllerHelp(void){
    ShowControllerMenuHeader();
    USBSendString("\rQHC Firmware v2.0\r");
    USBSendString("Commands can be upper or lower case. Spaces are ignored.\r\r");
    USBSendString("C n     -- selects a controller to configure (and show configuration)\r");
    USBSendString("AD nn    -- sets the address of the temperature sensor (00, 01, 10, or 11)\r");
    USBSendString("KP n.nn  -- sets the proportional gain\r");
    USBSendString("KD n.nn  -- sets the derivative gain\r");
    USBSendString("KI n.nn  -- sets the integral gain\r");
    USBSendString("IL n.nn  -- sets the integral limit (0-1)\r");
    USBSendString("TG n.nn  -- sets the target temperature\r");
    USBSendString("PP n     -- sets the PWM period (in milliseconds, 20ms resolution, 1000ms max)\r");
    USBSendString("TO n     -- sets the watchdog timeout in seconds\r");
    USBSendString("SL n.nn  -- sets the slew limit in degrees per minute\r");
    USBSendString("EC       -- enable controller (heater output)\r");
    USBSendString("DC       -- disable controller (heater output)\r");
    USBSendString("EO       -- enable offset correction\r");
    USBSendString("DO       -- disable offset correction\r");
    USBSendString("U        -- shows the status of all of the controllers\r");
    USBSendString("R        -- shows an easily parsable version of Update\r");
    USBSendString("Bounce   -- performs a power-cycle / reboot on the system\r");
    USBSendString("*        -- toggle automatic data flood\r\r");
}

//Show the Compensator Context Menu
void ShowCompensatorHelp(void){
    ShowCompensatorMenuHeader();
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\n");
    USBSendString("\n");
    USBSendString("volt            -- Peak to Peak Voltage output\n");
    USBSendString("comp            -- Toggle Auto Compensation output\n");
    USBSendString("wave            -- Set the Wavelength to Compensation to\n");
    USBSendString("enable          -- enable or disable the controller\n");
    USBSendString("stage		   -- Set Stage Size 2.6/5.4/10.8");
    USBSendString("disable         -- disable the controller\n");
    USBSendString("address         -- i2c address of the sensor\n");
    USBSendString("raw             -- shows an easily parsable version of the Information\n");
    USBSendString("main            -- return to the main menu\n");
    USBSendString("\n");
}

//Show the GPIO Context Menu
void ShowGPIOHelp(void){
    ShowGPIOMenuHeader();
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"gpio=1\", \"gpio 1\", \"gpio1\", \"g1\" are all treated the same.\n");
    USBSendString("\n");
    USBSendString("gpio            -- GPIO Pin to set\n");
    USBSendString("enable          -- enable or disable the controller\n");
    USBSendString("disable         -- disable the controller\n");
    USBSendString("input           -- set the pin to input\n");
    USBSendString("output          -- set the pin to output\n");
    USBSendString("raw             -- shows an easily parsable version of the Information\n");
    USBSendString("main            -- return to the main menu\n");
    USBSendString("\n");

}


void ShowBipolarOutputHelp(void){
  ShowBipolarOutputMenuHeader();
  USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
  USBSendString("\"bipolar=1\", \"bipolar 1\", \"bipolar1\", \"b1\" are all treated the same.\n");
  USBSendString("\n");
  USBSendString("bipolar         -- Bipolar Output to set\n");
  USBSendString("enable          -- enable or disable the controller\n");
  USBSendString("disable         -- disable the controller\n");
  USBSendString("voltage         -- set the voltage of the Bipolar Output\n");
  USBSendString("frequency       -- set the frequency of the Bipolar Output\n");
  USBSendString("pulses          -- set the number of pulses of the Bipolar Output\n");
  USBSendString("raw             -- shows an easily parsable version of the Information\n");
  USBSendString("main            -- return to the main menu\n");
  USBSendString("\n");
}


//Case Switched to Set the Sensors
//=================================================================================================
//General Set sensor method
void SetSensor(struct sTMP117 * sSensor, uint8_t u){
  switch (u){
    case 0:
      USBSendString("Address set to 0b 10 01 00 0x.\n");
      sSensor->Address = 0b1001000;
      break;
    case 10:
      USBSendString("Address set to 0b 10 01 01 0x.\n");
      sSensor->Address = 0b1001010;
      break;
    case 1:
      USBSendString("Address set to 0b 10 01 00 1x.\n");
      sSensor->Address = 0b1001001;
      break;
    case 11:
      USBSendString("Address set to 0b 10 01 01 1x.\n");
      sSensor->Address = 0b1001011;
      break;
    default:
      USBSendString("Invalid Address.\n");
      return;
      break;
  }
  

  sSensor->Average = -273.0f;
  sSensor->Temperature[0] = -273.0f;
  sSensor->Temperature[1] = -273.0f;
  sSensor->Configured = false;
  sSensor->State = 0;
  return;
}
