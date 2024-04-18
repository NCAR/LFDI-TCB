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
#include "USB_VCP_Support.h"

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
void ProcessUserInput(struct sTuningControlBoard* TCB, char* input){
  //Get the User Input
  switch(SUB_MENU){
    case MAIN_MENU:
      TranslateUserInput_MainMenu(TCB, input);
      break;
    case CONTROLLER_MENU:
      TranslateUserInput_ControllerMenu(TCB, input);
      break;
    case COMPENSATOR_MENU:
      TranslateUserInput_CompensatorMenu(TCB, input);
      break;
    case GPIO_MENU:
      TranslateUserInput_GPIOMenu(TCB, input);
      break;
    case BIPOLAROUTPUT_MENU:
      TranslateUserInput_BipolarOutputMenu(TCB, input);
      break;
    default:
      SUB_MENU = MAIN_MENU;
      uprintf("\rAn Error Occurred\r");
      break;
  }

}

void ShowGPIOConfig(struct sGPIO* GPIO, uint8_t index){
  char enabled[10];
  if (GPIO->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  uprintf("GPIO%u: %s\r", index+1, enabled);
}

//Show the Bipolar Output Config
void ShowBipolarOutputConfig(struct sBipolarOutput* BipolarOutput, uint8_t index){
  char enabled[10];
  if (BipolarOutput->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  uprintf("Bipolar%u: frequency= %04u pulses= %04u  Peak2Peak=%6.2f  %s\r", index+1, BipolarOutput->Frequency, BipolarOutput->Pulses, BipolarOutput->Voltage, enabled);
}

//Show the configuration of the Mechanisms
//=================================================================================================
//Show the Configuration of a Controllers
void ShowHeaterControllerConfig(struct sHeaterController* Controller)
{
  char s1[12];

  uprintf("C%u: kp=%5.2f kd=%5.2f ki=%5.2f il=%4.2f",
           Controller->HeaterNumber, Controller->PID.Config.Kp, Controller->PID.Config.Kd,
       Controller->PID.Config.Ki, Controller->PID.Config.Il);

  FormatTemperature(s1, Controller->PID.Config.Target);
  uprintf(" target=%8s PwmPeriod=%5.3fs SlewLimit=%4.1f",
           s1, Controller->PwmPeriod_ms / 1000.0f, Controller->PID.Config.SlewLimit_degpermin);

  switch (Controller->PID.Config.OffsetCorrectionEnabled)
  {
    case true:
      uprintf( " offsetcor=enabled");
      break;
    case false:
      uprintf( " offsetcor=disabled");
      break;
  }
  switch (Controller->Sensor.Address & 0x03)
  {
    case 0:
      uprintf( " address=00");
      break;
    case 1:
      uprintf( " address=01");
      break;
    case 2:
      uprintf( " address=10");
      break;
    case 3:
      uprintf( " address=11");
      break;
  }
  if (Controller->HeaterEnabled)
    uprintf( " heater=enabled\r");
  else
    uprintf( " heater=disabled\r");
}


//Show the Configuration of a Controllers
void ShowCompensatorConfig(struct sCompensator* Compensator, uint8_t index){
  char s1[12];
  char s2[12];

  FormatTemperature(s1, Compensator->Sensor.Temperature[0]);
  FormatTemperature(s2, Compensator->Sensor.Average);
  uprintf("Comp%u: Peak2Peak=%6.2f  Wave=%6.2f  Temp=%8s Avg=%8s address=",index+1, Compensator->voltage,
       Compensator->wavelength, s1, s2);
  switch (Compensator->Sensor.Address & 0x03)
  {
    case 0:
      uprintf("00");
      break;
    case 1:
      uprintf("01");
      break;
    case 2:
      uprintf("10");
      break;
    case 3:
      uprintf("11");
      break;
    default:
      break;
  }
  if (Compensator->Enable)
    uprintf("  ENABLED\r");
  else
    uprintf("  DISABLED\r");
}




//Show All the information for the Bipolar Output
void ShowAllBipolarOutput(struct sBipolarOutput* BipolarOutput, bool readable, uint8_t index){
  char enabled[10];
  if (BipolarOutput->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  
  uprintf("Bipolar%u:\t%04u\t%04u\t%6.2f\t%s\r", index+1, BipolarOutput->Frequency, BipolarOutput->Pulses, BipolarOutput->Voltage, enabled);
}


//Show All the information for the GPIO
void ShowAllGPIO(struct sGPIO* GPIO, bool readable){
  char enabled[10];
  if (GPIO->Enabled)
    strcpy(enabled, "ENABLED ");
  else
    strcpy(enabled, "DISABLED");
  uprintf("GPIO%u:\t%s\r", GPIO->GPIONum, enabled);
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
    char heat_on[4] = {0};
    if (Controller->HeaterEnabled)
      strcpy(heat_on, "on ");
    else
      strcpy(heat_on, "off");
    char oc_on[7] = {0};
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

    if (readable)
    {
      uprintf("\rC%u: address: %2s   offsetcor:%7s   heater:%3s   sensor:%s\r",
               Controller->HeaterNumber, address, oc_on, heat_on, sensor);
      uprintf("C%u: kp=%5.2f   ep=% 6.1f    temp=%8s  PwmPeriod=%5.3fs\r",
               Controller->HeaterNumber, Controller->PID.Config.Kp, 100 * Controller->PID.Ep, last, Controller->PwmPeriod_ms/1000.0f);
      uprintf("C%u: kd=%5.2f   ed=% 6.1f     avg=%8s    \r",
               Controller->HeaterNumber, Controller->PID.Config.Kd, 100 * Controller->PID.Ed, average);
      uprintf("C%u: ki=%5.2f   ei=% 6.1f slewtar=%8s    SlewLim=%4.2f d/m\r",
               Controller->HeaterNumber, Controller->PID.Config.Ki, 100 * Controller->PID.Ei, sltarget, Controller->PID.Config.SlewLimit_degpermin);
      uprintf("C%u: il=%5.2f  eff=% 6.1f  target=%8s\r",
               Controller->HeaterNumber, Controller->PID.Config.Il, 100 * Controller->PID.Effort, target);
    }
    else
    {
      if ((Controller->HeaterNumber == 1) && (autoflood == false))
        ShowRawHeaderHeaterController();
      if (autoflood)
        uprintf("!C");
      else
        uprintf("C");
      uprintf("%u\t%5.2f\t%5.2f\t%5.2f\t%4.2f\t",
               Controller->HeaterNumber, Controller->PID.Config.Kp, Controller->PID.Config.Kd, Controller->PID.Config.Ki, Controller->PID.Config.Il);
      uprintf("%7.1f\t%7.1f\t%7.1f\t%7.1f\t",
               100 * Controller->PID.Ep, 100 * Controller->PID.Ed, 100 * Controller->PID.Ei, 100 * Controller->PID.Effort);
      uprintf("%7.3f\t%8s\t%8s\t%8s\t%8s\t%2s\t%5.3f\t%s\t%s\t%s\r",
               0.0, last, average, sltarget, target, address, Controller->PwmPeriod_ms/1000.0f, oc_on, heat_on, sensor);
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

    uprintf("Comp%u\t%5.2f\t%5.2f\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r",
        index+1, Compensator->voltage, Compensator->wavelength, last, average, compensating, useaverage, address, enabled, sensor);
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
  uprintf("\tkp\tkd\tki\tli\tep\ted\tei\teffort\tcurr\tinstant_temp\taverage_temp\tslew_target\tfinal_target\ti2c\tperiod\toffset\theater\tsensor\r");
}

//Show the Raw Header for the Compensator
void ShowRawHeaderCompensator(void)
{
  uprintf("Comp\tPeak2Peak\tWave\tTemp\tAvg\tAuto\tUseAverage\ti2c\tenabled\tsensor\r");
}


//Show the Raw GPIO header
void ShowRawHeaderGPIO(void)
{
  uprintf( "GPIO\tEnabled\r");
}

//Show the Raw Header for the Bipolar Output
void ShowRawHeaderBipolarOutput(void)
{
  uprintf( "Bipolar\tfrequency\tpulses\tPeak2Peak\tEnabled\r");
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
  uprintf("!T%.4f\t%.4f\t%.4f\t%.4f\r",
     Controllers[0].Sensor.Temperature[0],
     Controllers[1].Sensor.Temperature[0],
     Controllers[2].Sensor.Temperature[0],
     Controllers[3].Sensor.Temperature[0]);
  ShowAllHeaterController(&Controllers[0], false, true);
  ShowAllHeaterController(&Controllers[1], false, true);
  ShowAllHeaterController(&Controllers[2], false, true);
  ShowAllHeaterController(&Controllers[3], false, true);
}


//Case Switched to Process the User Input
//=================================================================================================
//Parse the input from the main Menu
//Top Level Commands
// SET_Heater_[Temp]  -- Sets the target temperature for all controllers
// SET_HEATER_ON -- Turns on the temperature controllers
// SET_HEATER_OFF -- Turns off the temperature controllers
// SET_WAVE_[Wavelength] -- Sets the wavelength for all compensators
// SET_HEATER_[Controller]_KP_[KP] -- Sets the controller to be configured
// SET_HEATER_[Controller]_KD_[KD] -- Sets the controller to be configured
// SET_HEATER_[Controller]_KI_[KI] -- Sets the controller to be configured
// SET_TUNE_ON -- Turns on the tuning for all controllers
// SET_TUNE_OFF -- Turns off the tuning for all controllers
// GET_HK -- Gets the housekeeping data

void SET_WAVE(struct sTuningControlBoard * s, char* input){
    //SET_WAVE_[Wavelength] check if the input is a valid wavelength
    float f = 0;
    if (sscanf(input, "set_wave_%f", &f) == 1)
    {
      for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++)
      {
        s->Compensator[i].wavelength = f;
        //Float should be rounded to 2 decimal places
        uprintf("Compensator %u Wavelength set to %f\r", i, f);
      }
      return;
    }
    uprintf("Invalid Command: %s\r", input);
}

void SET_TUNE(struct sTuningControlBoard * s, char* input){
    //SET_TUNE_ON
    if (strcmp(input, "set_tune_on") == 0)
    {
      for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++)
      {
    	s->Compensator[i].compensate = true;
        s->Compensator[i].Enable = true;
        uprintf("Compensator %u Tuning Enabled\r", i);
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
        uprintf("Compensator %u Tuning Disabled\r", i);
      }
      return;
    }
    uprintf("Invalid Command: %s\r", input);
}

void SET_SLOPE(struct  sTuningControlBoard *s, char* input){
	float f = 0.0;
	if (sscanf(input, "set_slope_%f", &f) == 1){
		for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++){
			s->Compensator[i].Stage.slope = f;
			uprintf("Compensator %u Slope Set %f\r", i, f);
		}
	    return;
	}
	uprintf("Invalid Command: %s\r", input);
}

void SET_INT(struct  sTuningControlBoard *s, char* input){
	float f = 0.0;
	if (sscanf(input, "set_INT_%f", &f) == 1){
		for (uint8_t i = 0; i < NUMOFCOMPENSATORS; i++){
			s->Compensator[i].Stage.intercept = f;
			uprintf("Compensator %u INT Set %f\r", i, f);
		}
	    return;
	}
	uprintf("Invalid Command: %s\r", input);
}

void SET_HEATER(struct  sTuningControlBoard *s, char* input){
    
	unsigned int u = 0;
  float f = 0;
  //Turn on the Temperature Controllers
  if (strcmp(input, "set_heater_on") == 0){
      for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
      {
        s->HeaterControllers[i].HeaterEnabled = true;
        uprintf("Heater %u Enabled\r", i);
      }
      return;
    }
  //SET_TEMP_OFF
  if (strcmp(input, "set_heater_off") == 0)
  {
    for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
    {
      s->HeaterControllers[i].HeaterEnabled = false;
      uprintf("Heater %u Disabled\r", i);
    }
    return;
  }


    if (sscanf(input, "set_heater_%u_kp_%f", &u, &f) == 2)
    {
      if (u <= NUMOFHEATERCONTROLLERS)
      {
        s->HeaterControllers[u-1].PID.Config.Kp = f;
        uprintf("Heater %u Kp set to %f\r", u, f);
      }
      return;
    }
    //SET_HEATER_[Controller]_KD_[KD]
    if (sscanf(input, "set_heater_%u_kd_%f", &u, &f) == 2)
    {
      if (u <= NUMOFHEATERCONTROLLERS)
      {
        s->HeaterControllers[u-1].PID.Config.Kd = f;
        uprintf("Heater %u Kd set to %f\r", u, f);
      }
      return;
    }
    //SET_HEATER_[Controller]_KI_[KI]
    if (sscanf(input, "set_heater_%u_ki_%f", &u, &f) == 2)
    {
      if (u <= NUMOFHEATERCONTROLLERS)
      {
        s->HeaterControllers[u-1].PID.Config.Ki = f;
        uprintf("Heater %u Ki set to %f\r", u, f);
      }
      return;
    }
    //SET_heater_[Temp] check if the input is a valid temperature
    if (sscanf(input, "set_heater_%f", &f) == 1)
    {
      for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
      {
        s->HeaterControllers[i].PID.Config.Target = f;
        uprintf("Heater %u Target set to %f\r", i, f);
      }
      return;
    }
    uprintf("Invalid Command: %s\r", input);
}
//Any Command with SET_ Prefix will be parsed here
void SET_Processing_Tree(struct sTuningControlBoard * s, char* input){
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
  if (strncmp(input, "set_slope_", 9) == 0){
	  SET_SLOPE(s, input);
	  return;
  }
  if (strncmp(input, "set_int_", 9) == 0){
  	  SET_INT(s, input);
	  return;
  }
  uprintf("Invalid Command: %s\r", input);
}
//Prints Non-Readable Houskeeping data in the Following Format
//H1\tH1.KP\H1.KI\tH1.KD\tH1.IL\tH1.EP\tH1.ED\tH1.EI\tH1.Effort\tH1.Current\tH1.Instant_Temp\tH1.Average_Temp\tH1.Slew_Target\tH1.Final_Target\tH1.I2C\tH1.Period\tH1.Offset\tH1.Heater\tH1.Sensor\t
//H2 ""
//H3 ""
//C1\tC1.Peak2Peak\tC1.Wave\tC1.Temp\tC1.Avg\tC1.Auto\tC1.UseAverage\tC1.i2c\tC1.enabled\tC1.sensor\tC1.StageSize"
//C2 ""
//C3 ""
//C4 ""
//C5 ""
//C6 ""
void ShowHousKeeping(struct sTuningControlBoard * s){
  char heater[10];
  for (uint8_t i = 0; i < NUMOFHEATERCONTROLLERS; i++)
  {
    if(s->HeaterControllers[i].HeaterEnabled){
      strcpy(heater, "ENABLED");
    }else{
      strcpy(heater, "DISABLED");
    }
    uprintf("H%u\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%s\r",
    i+1, s->HeaterControllers[i].PID.Config.Kp, s->HeaterControllers[i].PID.Config.Ki, 
    s->HeaterControllers[i].PID.Config.Kd, s->HeaterControllers[i].Sensor.Average, 
    s->HeaterControllers[i].PID.Config.Target, heater);
  }
  for(uint8_t i = 0; i < NUMOFCOMPENSATORS; i++){
    if(s->Compensator[i].Enable){
      strcpy(heater, "ENABLED");
    }else{
      strcpy(heater, "DISABLED");
    }
    uprintf("C%u\t%3.2f\t%3.2f\t%2.2f\t%s\r",i+1, s->Compensator[i].wavelength,
    s->Compensator[i].voltage, s->Compensator[i].Stage.stageSize, heater);
  }
  
}

//Parse the Get Commands
void GET_Processing_Tree(struct sTuningControlBoard * s, char* input){
  //GET_HK
  if (strcmp(input, "get_hk") == 0)
  {
	  ShowHousKeeping(s);
    return;
  }
  //GET_TEMP
  if (strcmp(input, "get_temp") == 0)
  {
    //ShowTemperature();
    return;
  }
  //GET_WAVE
  if (strcmp(input, "get_wave") == 0)
  {
    //ShowWavelength();
    return;
  }
  //GET_TUNE
  if (strcmp(input, "get_tune") == 0)
  {
    //ShowTuning();
    return;
  }
  //GET_CONT_[Controller]
  uint8_t u = 0;
  if (sscanf(input, "get_cont_%hhu", &u) == 1)
  {
    if (u < NUMOFHEATERCONTROLLERS)
    {
      ShowAllHeaterController(&s->HeaterControllers[u], true, false);
    }
    return;
  }
  uprintf("Invalid Command\r");
}

void ProcessUserInput_MainMenu(struct sTuningControlBoard * s,char* input){
  //SET Commands
  if (strncmp(input, "set_", 4) == 0)
  {
    SET_Processing_Tree(s, input);
    return;
    
  }

  //GET Commands
  if (strncmp(input, "get_", 4) == 0)
  {
    GET_Processing_Tree(s, input);
    return;
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
    uprintf("Wiped Configuration\r");
  }

  //This resets the TCB
  else if (strcmp(input, "b") == 0)
  {
    uprintf("Bouncing...\r");
    HAL_Delay(1000);
    NVIC_SystemReset();
  }
  //This loads the TCB from the EEPROM
  else if (strcmp(input, "l") == 0)
  {
    //LoadConfig();
    uprintf("no EEPROM Cannot Load Configuration\r");
  }
  //This saves the TCB to the EEPROM
  else if (strcmp(input, "s") == 0)
  {
    //SaveConfig(); TODO add implementation for SD Card here
    uprintf("no EEPROM Cannot Save Config\r");
  }
  //This prints the help menu
  else if (strcmp(input, "h") == 0)
  {
    ShowMainHelp();
  }
  else
  {
    uprintf("Unknown Command: %s\r", input);
  }
}

//Parse the input from the Compensator Context Menu
void ProcessUserInput_CompensatorMenu(struct sTuningControlBoard * s,char* input)
{
  uint8_t u = 0;
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
      uprintf("No Compensator Selected\r");
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
    uprintf("No controller selected.\r");
    return;
  }

  //Turn on the Temperature Controllers
  if (strcmp((char*) input, "e") == 0)
  {
      uprintf("Compensator %d Enabled.\r", UI_Compensator+1);
      Compensator_enableChannel(&s->Compensator[UI_Compensator], true);
      return;
   }

  //Turn off the Temperature Controllers
  if (strcmp((char*) input, "d") == 0)
  {
    //Check to see if the controller is a Compensator
    if (UI_Compensator < 6){
      uprintf("Compensator %d Disabled.\r", UI_Compensator+1);
      Compensator_enableChannel(&s->Compensator[UI_Compensator], false);
      return;
    }
  }

  if (strcmp((char*) input, "co") == 0){
    if(s->Compensator[UI_Compensator].compensate){
      s->Compensator[UI_Compensator].compensate = false;
      uprintf("Compensator %d Auto Compensating Off.\r", UI_Compensator+1);
    } else {
      s->Compensator[UI_Compensator].compensate = true;
      uprintf("Compensator %d Auto Compensating On.\r", UI_Compensator+1);
    }
    return;
  }

  if (sscanf((char*) input, "%c%f", &c, &f) == 2){
    u = (uint16_t) f;
    switch (c){
      case 'c':
        uprintf("Invalid Compensator Number.\r");
        return;
        break;
      case 'a':
        //Set the sensor address
        SetSensor(&s->Compensator[UI_Compensator].Sensor, u);
        uprintf("Compensator %d Sensor Address Set to %d.\r", UI_Compensator+1, u);
        return;
        break;

      case 'v':
        //Check the Voltage is greater than 0 and less than max peak to peak
        if (f >= 0 && f < s->Compensator[UI_Compensator].Channel.max_peak2peak){
          s->Compensator[UI_Compensator].voltage = f;
          s->Compensator[UI_Compensator].compensate = false;
          //Print out the string with the Compensator number and voltage
          uprintf("Compensator %d Voltage Set to %f.\r", UI_Compensator+1, f);
          break;
          return;
        } else {
          uprintf("Invalid Voltage.\r");
          break;
          return;
        }

      case 'w':
        //Set the wavelength
        //Check that the wavelength is greater than 0 and less than 500
        if (f > 656 && f < 657){
          s->Compensator[UI_Compensator].wavelength = f;
          uprintf("Compensator %d Wavelength Set to %f.\r", UI_Compensator+1, f);
          break;
          return;
        } else {
          uprintf("Invalid Wavelength. Must be between 0 and 500\r");
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
			uprintf("Invalid Stage.\r");
			break;
		}
		uprintf("Compensator %d Stage Set to %f.\r", UI_Compensator+1, f);
		break;

      default:
        uprintf("Unknown Command.\r");
        break;
        return;
    }
  }
}

//Parse the input from the Controller Context Menu
void ProcessUserInput_HeaterControllerMenu(struct sHeaterController HeaterControllers[NUMOFHEATERCONTROLLERS], char* input)
{
  uint16_t i = 0;
  uint16_t u = 0;
  char c1, c2;
  float f = 0;

  if ((strcmp(input, "u") == 0) || (strcmp(input, "/") == 0))
  {
    for (i=0; i<NUMOFHEATERCONTROLLERS; i++)
      ShowAllHeaterController(&HeaterControllers[i], true, false);
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
    for (i=0; i<NUMOFHEATERCONTROLLERS; i++)
      ShowAllHeaterController(&HeaterControllers[i], false, false);
    return;
  }

  if (strcmp(input, "bounce") == 0)
  {
    NVIC_SystemReset();
  }


  if ((strcmp(input, "1") == 0) || (strcmp(input, "c1") == 0))
  {
    SelectedHeaterController = 0;
    ShowHeaterControllerConfig(&HeaterControllers[SelectedHeaterController]);
    return;
  }

  if ((strcmp(input, "2") == 0) || (strcmp(input, "c2") == 0))
  {
    SelectedHeaterController = 1;
    ShowHeaterControllerConfig(&HeaterControllers[SelectedHeaterController]);
    return;
  }

  if ((strcmp(input, "3") == 0) || (strcmp(input, "c3") == 0))
  {
    SelectedHeaterController = 2;
    ShowHeaterControllerConfig(&HeaterControllers[SelectedHeaterController]);
    return;
  }

  if ((strcmp(input, "4") == 0) || (strcmp(input, "c4") == 0))
  {
    SelectedHeaterController = 3;
    ShowHeaterControllerConfig(&HeaterControllers[SelectedHeaterController]);
    return;
  }
  if (strcmp(input, "ec") == 0)
  {
	  uprintf("Controller %i heater output enabled.\r", SelectedHeaterController + 1);
    HeaterControllers[SelectedHeaterController].HeaterEnabled = true;
    HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
    return;
  }

  if (strcmp(input, "dc") == 0)
  {
	uprintf("Controller %i heater output disabled.\r", SelectedHeaterController + 1);
    HeaterControllers[SelectedHeaterController].HeaterEnabled = false;
    return;
  }

  if (strcmp(input, "eo") == 0)
  {
    uprintf("Offset correction enabled.\r");
    HeaterControllers[SelectedHeaterController].PID.Config.OffsetCorrectionEnabled = true;
    return;
  }

  if (strcmp(input, "do") == 0)
  {
    uprintf("Offset correction disabled.\r");
    HeaterControllers[SelectedHeaterController].PID.Config.OffsetCorrectionEnabled = false;
    HeaterControllers[SelectedHeaterController].PID.OffsetCorrection = 0.0f;
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
          uprintf("Address set to 0b 10 01 00 0x.\r");
          HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001000;
          HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        case 10:
          uprintf("Address set to 0b 10 01 01 0x.\r");
          HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001010;
          HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        case 1:
          uprintf("Address set to 0b 10 01 00 1x.\r");
          HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001001;
          HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        case 11:
          uprintf("Address set to 0b 10 01 01 1x.\r");
          HeaterControllers[SelectedHeaterController].Sensor.Address = 0b1001011;
          HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
          return;
          break;
        default:
          uprintf("Invalid Address.\r");
          return;
          break;
      }
    }
    if ((c1 == 'k') && (c2 == 'p'))
    {
      if (f < 0)
        uprintf("Invalid value.\r");
      else
      {
      	uprintf("kp set to %f.\r", f);
        HeaterControllers[SelectedHeaterController].PID.Config.Kp = f;
      }
      return;
    }
    if ((c1 == 'k') && (c2 == 'i'))
    {
      if (f < 0)
        uprintf("Invalid value.\r");
      else
      {
        uprintf("ki set to %f.\r", f);
        HeaterControllers[SelectedHeaterController].PID.Config.Ki = f;
      }
      return;
    }
    if ((c1 == 'k') && (c2 == 'd'))
    {
      if (f < 0)
        uprintf("Invalid value.\r");
      else
      {
    	uprintf("kd set to %f.\r", f);
        HeaterControllers[SelectedHeaterController].PID.Config.Kd = f;
      }
      return;
    }
/*    if ((c1 == 't') && (c2 == 'o'))
    {
      uprintf("Watchdog timeout set to %u seconds.\r", u);
      uprintf(output);
      WatchdogTimeout = u;
      return;
    }
    */
    if ((c1 == 'p') && (c2 == 'p'))
    {
      if ((f < 20) || (f > 1000))
        uprintf("Invalid value.\r");
      else
      {
    	uprintf("PWM period is %.3fs (~ %.1f Hz).\r", (f/1000), (1000/f));
        HeaterControllers[SelectedHeaterController].PwmPeriod_ms = f;
      }
      return;
    }
    if ((c1 == 'i') && (c2 == 'l'))
    {
      if (f < 0)
        uprintf("Invalid value.\r");
      else
      {
    	uprintf("Integrator limit set to %f.\r", f);
        HeaterControllers[SelectedHeaterController].PID.Config.Il = f;
      }
      return;
    }
    if ((c1 == 't') && (c2 == 'g'))
    {
      uprintf("Target temperature set to %f.\r", f);
      HeaterControllers[SelectedHeaterController].PID.Config.Target = f;
      HeaterControllers[SelectedHeaterController].PID.NeedRefresh = true;
      return;
    }
    if ((c1 == 's') && (c2 == 'l'))
    {
      uprintf("Slew limit set to %.2f deg/min.\r", f);
      HeaterControllers[SelectedHeaterController].PID.Config.SlewLimit_degpermin = f;
      return;
    }
  }
  uprintf("Unknown command: %s\r",(char*) input);
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
      uprintf("No GPIO selected.\r");
    else
    {
      /*
      ShowSensor(&TCB.Controller);
      ShowControllerConfig(&TCB.Controller);
      ShowEffort(&TCB.Controller);
      uprintf("\r");
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
    //uprintf("Controller 2 not implemented.\r");
    //ShowControllerConfig(Controllers);
    return;
  }
  if ((strcmp(buffer, "3") == 0) || (strcmp(buffer, "g3") == 0))
  {
    UI_GPIO = 2;
    ShowGPIOConfig(&s->GPIO[UI_GPIO], UI_GPIO);
    //uprintf("Controller 3 not implemented.\r");
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
    uprintf("No GPIO selected.\r");
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
    uprintf("GPIO enabled.\r");
    GPIO_SetState(&s->GPIO[UI_GPIO], true);
    return;
  }
  //Disable the Controllers
  if (strcmp(buffer, "d") == 0)
  {
    uprintf("GPIO disabled.\r");
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
        uprintf("Invalid controller number.\r");
        return;
        break;
      
      default:
        break;
    }
  }
  uprintf("Unknown command.\r");
  return;

}

//Process User Input for the Bipolar Output Menu
void ProcessUserInput_BipolarOutputMenu(struct sTuningControlBoard * s, char * buffer){
  
  uint8_t u = 0;
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
      uprintf("No Bipolar Output selected.\r");
    else
    {
      /*
      ShowSensor(&TCB.Controller);
      ShowControllerConfig(&TCB.Controller);
      ShowEffort(&TCB.Controller);
      uprintf("\r");
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
    uprintf("No Bipolar Output selected.\r");
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
    uprintf("BipolarOutput enabled.\r");
    BipolarOutput_Enable(&s->BipolarOutput[UI_BipolarOutput], true);
    return;
  }
  //Disable the Controllers
  if (strcmp((char*) buffer, "d") == 0)
  {
    uprintf("BipolarOutput disabled.\r");
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
        uprintf("Invalid controller number.\r");
        return;
        break;
      case 'f':
        if (f < 0)
          uprintf("Invalid value.");
        else
        {
          uprintf("Frequency set to %f.\r", f);
          BipolarOutput_SetFrequency(&s->BipolarOutput[UI_BipolarOutput], f);
        }
        break;
      case 'p':
        if (f < 0)
          uprintf("Invalid value.");
        else
        {
          uprintf("Pulses set to %f.\r", f);
          BipolarOutput_SetPulses(&s->BipolarOutput[UI_BipolarOutput], u);
        }
        break;

       case 'v':
        //Check the Voltage is greater than 0 and less than max peak to peak
        if (f >= 0 && f < s->BipolarOutput[UI_BipolarOutput].Channel.max_peak2peak){
          BipolarOutput_SetVoltage(&s->BipolarOutput[UI_BipolarOutput], f);
          //Print out the string with the Compensator number and voltage
          uprintf("BipolarOutput %d Voltage Set to %f.\r", UI_BipolarOutput+1, f);
          break;
          return;
        } else {
          uprintf("Invalid Voltage.\r");
          break;
          return;
        }

        break;
      default:
        break;
    }
  }
  uprintf("Unknown command.\r");
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
void TranslateUserInput_ControllerMenu(struct sTuningControlBoard * s, char * input){
  //Make everything lowercase
  for (int i=0; input[i]; i++){
    input[i] = tolower(input[i]);
  }
  replacestr(input, "=", "");
  replacestr(input, " ", "");
  replacestr(input, " ", "");
  replacestr(input, " ", "");
  replacestr(input, " ", "");
  replacestr(input, " ", "");
  ProcessUserInput_HeaterControllerMenu(s->HeaterControllers, input);
}



//Headers to Show the User to let them Know where they are
//=================================================================================================
void ShowMainMenuHeader(void)
{
  uprintf("Main Menu\r");
  uprintf("=========\r");
}

void ShowCompensatorMenuHeader(void)
{
  uprintf("Compensator Menu\r");
  uprintf("================\r");
}

void ShowControllerMenuHeader(void)
{
  uprintf("Controller Menu\r");
  uprintf("===============\r");
}

void ShowGPIOMenuHeader(void){
	uprintf("GPIO Menu\r");
	uprintf("===============\r");
}

void ShowBipolarOutputMenuHeader(void){
  uprintf("Bipolar Output Menu\r");
  uprintf("===============\r");
}

//Help Menus for End User
//=================================================================================================
//Main Menu help Text
void ShowMainHelp(void)
{
    uprintf("\rLFDI TCB Firmware v1.5\r");
    uprintf("\rLFDI TCB Hardware Rev v1\r");
    ShowMainMenuHeader();
    uprintf("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\r");
    uprintf("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\r");
    uprintf("\r");
    uprintf("Controller      -- Open The Controller Context Menu\r");
    uprintf("Compensator     -- Open The Compensator Context Menu\r");
    uprintf("Update          -- shows the status of all of the controllers and Compensators\r");
    uprintf("Raw             -- shows an easily parsable version of Update\r");
    uprintf("Wipe            -- wipes the existing configuration and load new defaults\r");
    uprintf("Bounce          -- performs a power-cycle / reboot on the system\r");
    uprintf("Load            -- reloads the previously saved values (automatic at power-on)\r");
    uprintf("Save            -- saves the currently configured values\r");
    uprintf("\r");
}

//Show the Controller Context Menu
void ShowHeaterControllerHelp(void){
    ShowControllerMenuHeader();
    uprintf("\rQHC Firmware v2.0\r");
    uprintf("Commands can be upper or lower case. Spaces are ignored.\r\r");
    uprintf("C n     -- selects a controller to configure (and show configuration)\r");
    uprintf("AD nn    -- sets the address of the temperature sensor (00, 01, 10, or 11)\r");
    uprintf("KP n.nn  -- sets the proportional gain\r");
    uprintf("KD n.nn  -- sets the derivative gain\r");
    uprintf("KI n.nn  -- sets the integral gain\r");
    uprintf("IL n.nn  -- sets the integral limit (0-1)\r");
    uprintf("TG n.nn  -- sets the target temperature\r");
    uprintf("PP n     -- sets the PWM period (in milliseconds, 20ms resolution, 1000ms max)\r");
    uprintf("TO n     -- sets the watchdog timeout in seconds\r");
    uprintf("SL n.nn  -- sets the slew limit in degrees per minute\r");
    uprintf("EC       -- enable controller (heater output)\r");
    uprintf("DC       -- disable controller (heater output)\r");
    uprintf("EO       -- enable offset correction\r");
    uprintf("DO       -- disable offset correction\r");
    uprintf("U        -- shows the status of all of the controllers\r");
    uprintf("R        -- shows an easily parsable version of Update\r");
    uprintf("Bounce   -- performs a power-cycle / reboot on the system\r");
    uprintf("*        -- toggle automatic data flood\r\r");
}

//Show the Compensator Context Menu
void ShowCompensatorHelp(void){
    ShowCompensatorMenuHeader();
    uprintf("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\r");
    uprintf("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\r");
    uprintf("\r");
    uprintf("volt            -- Peak to Peak Voltage output\r");
    uprintf("comp            -- Toggle Auto Compensation output\r");
    uprintf("wave            -- Set the Wavelength to Compensation to\r");
    uprintf("enable          -- enable or disable the controller\r");
    uprintf("stage		   -- Set Stage Size 2.6/5.4/10.8");
    uprintf("disable         -- disable the controller\r");
    uprintf("address         -- i2c address of the sensor\r");
    uprintf("raw             -- shows an easily parsable version of the Information\r");
    uprintf("main            -- return to the main menu\r");
    uprintf("\r");
}

//Show the GPIO Context Menu
void ShowGPIOHelp(void){
    ShowGPIOMenuHeader();
    uprintf("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\r");
    uprintf("\"gpio=1\", \"gpio 1\", \"gpio1\", \"g1\" are all treated the same.\r");
    uprintf("\r");
    uprintf("gpio            -- GPIO Pin to set\r");
    uprintf("enable          -- enable or disable the controller\r");
    uprintf("disable         -- disable the controller\r");
    uprintf("input           -- set the pin to input\r");
    uprintf("output          -- set the pin to output\r");
    uprintf("raw             -- shows an easily parsable version of the Information\r");
    uprintf("main            -- return to the main menu\r");
    uprintf("\r");

}


void ShowBipolarOutputHelp(void){
  ShowBipolarOutputMenuHeader();
  uprintf("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\r");
  uprintf("\"bipolar=1\", \"bipolar 1\", \"bipolar1\", \"b1\" are all treated the same.\r");
  uprintf("\r");
  uprintf("bipolar         -- Bipolar Output to set\r");
  uprintf("enable          -- enable or disable the controller\r");
  uprintf("disable         -- disable the controller\r");
  uprintf("voltage         -- set the voltage of the Bipolar Output\r");
  uprintf("frequency       -- set the frequency of the Bipolar Output\r");
  uprintf("pulses          -- set the number of pulses of the Bipolar Output\r");
  uprintf("raw             -- shows an easily parsable version of the Information\r");
  uprintf("main            -- return to the main menu\r");
  uprintf("\r");
}


//Case Switched to Set the Sensors
//=================================================================================================
//General Set sensor method
void SetSensor(struct sTMP117 * sSensor, uint8_t u){
  switch (u){
    case 0:
      uprintf("Address set to 0b 10 01 00 0x.\r");
      sSensor->Address = 0b1001000;
      break;
    case 10:
      uprintf("Address set to 0b 10 01 01 0x.\r");
      sSensor->Address = 0b1001010;
      break;
    case 1:
      uprintf("Address set to 0b 10 01 00 1x.\r");
      sSensor->Address = 0b1001001;
      break;
    case 11:
      uprintf("Address set to 0b 10 01 01 1x.\r");
      sSensor->Address = 0b1001011;
      break;
    default:
      uprintf("Invalid Address.\r");
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
