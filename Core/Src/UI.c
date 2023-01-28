/*
 * UI.c
 *
 *  Created on: Nov 15, 2021
 *      Author: damonb
 */

#include "UI.h"
#include <ctype.h>
#include "funcs.h"
#include <math.h>
#include "DAC.h"
#include "TuningControlBoard.h"


#define CONTROLLER_MENU 1
#define COMPENSATOR_MENU 2
#define MAIN_MENU 0
uint8_t UI_Controller = 9;
uint8_t UI_Compensator = 9;
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
      ProcessUserInput_ControllerMenu(TCB, buffer);
      break;
    case COMPENSATOR_MENU:
      TranslateUserInput_CompensatorMenu(TCB, buffer);
      break;
    default:
      SUB_MENU = MAIN_MENU;
      USBSendString("\nAn Error Occured\n");
      break;
  }

}


//Show the configuration of the Mechanisms
//=================================================================================================
//Show the Configuration of a Controller
void ShowControllerConfig(struct sController* Controller, uint8_t index){
  char s1[12];
  char buffer[250];
  FormatTemperature(s1, Controller->PID.Config.TargetP);
  snprintf(buffer, 200, "Cont%u: kp=%6.2f  kd=%6.2f  ki=%6.2f  li=%6.2f  target=%8s  history= %3u  frequency= %04u  address=", index+1,
      Controller->PID.Config.Kp, Controller->PID.Config.Kd, Controller->PID.Config.Ki, Controller->PID.Config.Li, s1, Controller->PID.Config.History, Controller->PID.Config.Frequency);
  USBSendString(buffer);
  switch (Controller->Sensor.Address & 0x03)
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
  if (Controller->PID.Config.Enabled)
    USBSendString("  ENABLED\n");
  else
    USBSendString("  DISABLED\n");
}

//Show the Configuration of a Controller
void ShowCompensatorConfig(struct sCompensator* Compensator, uint8_t index){
  char s1[12];
  char s2[12];
  char buffer[300];
  FormatTemperature(s1, Compensator->Sensor.LastTemperature);
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


//Show an individual sensor
void ShowSensor(struct sController* Controller)
{
  char buffer[250];
  char s1[12], s2[12], s3[12];
  FormatTemperature(s1, Controller->Sensor.Average);
  FormatTemperature(s2, Controller->Sensor.LastTemperature);
  FormatTemperature(s3, Controller->PID.Config.TargetP);

  snprintf(buffer, 200, "C%u: temp=%8s  avg=%8s  target=%8s  sensor: ", Controller->Heater, s2, s1, s3);
  USBSendString(buffer);

  switch (Controller->Sensor.State)
  {
    case TMP117_STATE_UNKNOWN:
      USBSendString("Unknown\n");
      break;
    case TMP117_STATE_INITFAILED:
      USBSendString("Config failed\n");
      break;
    case TMP117_STATE_REQUESTNOACK:
      USBSendString("Req failed\n");
      break;
    case TMP117_STATE_RECEIVEFAIL:
      USBSendString("No response\n");
      break;
    case TMP117_STATE_VALIDTEMP:
      USBSendString("OK\n");
      break;
    default:
      break;
  }
}


//Show the Effort of a Controller
void ShowEffort(struct sController* Controller)
{
  char buffer[250];
  char s1[12], s2[12], s3[12];
  FormatTemperature(s1, Controller->Sensor.Average);
  FormatTemperature(s2, Controller->Sensor.LastTemperature);
  FormatTemperature(s3, Controller->PID.Config.TargetP);

  snprintf(buffer, 200, "C%u: Effort p=% 6.1f  d=% 6.1f  i=% 6.1f  total=% 6.1f%%\n", Controller->Heater,
      100 * Controller->PID.Ep, 100 * Controller->PID.Ed,
      100 * Controller->PID.Ei, 100 * Controller->PID.Effort);
  USBSendString(buffer);

}

//Show all the information for one of the mechanisims
//=============================================================================
//Show the Status of a Controller
//@brief Show the Status of a Controller
//@param Controller The Controller to show
//@param readable If true, show the status in human readable form
void ShowAllController(struct sController* Controller, bool readable)
{

    char target[12];
    FormatTemperature(target, Controller->PID.Config.TargetP);
    char address[3];
    switch (Controller->Sensor.Address & 0x03)
    {
      case 0: strcpy(address, "00"); break;
      case 1: strcpy(address, "01"); break;
      case 2: strcpy(address, "10"); break;
      case 3: strcpy(address, "11"); break;
      default: break;
    }
    char enabled[10];
    if (Controller->PID.Config.Enabled)
      strcpy(enabled, "ENABLED ");
    else
      strcpy(enabled, "DISABLED");
    char average[12], last[12], targetp[12];
    FormatTemperature(average, Controller->Sensor.Average);
    FormatTemperature(last, Controller->Sensor.LastTemperature);
    FormatTemperature(targetp, Controller->PID.Config.TargetP);

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

    static char buffer[250];

    if (readable)
    {
      snprintf(buffer, 200, "C%u: kp=%5.2f   ep=% 7.1f    temp=%8s  freq: %04u  %s\n", Controller->Heater, Controller->PID.Config.Kp, 100 * Controller->PID.Ep, last, Controller->PID.Config.Frequency, enabled);
      USBSendString(buffer);
      snprintf(buffer, 200,  "  : kd=%5.2f   ed=% 7.1f     avg=%8s  i2c: %2s\n", Controller->PID.Config.Kd, 100 * Controller->PID.Ed, average, address);
      USBSendString(buffer);
      snprintf(buffer, 200,  "  : ki=%5.2f   ei=% 7.1f  target=%8s  history=%3u\n", Controller->PID.Config.Ki, 100 * Controller->PID.Ei, target, Controller->PID.Config.History);
      USBSendString(buffer);
      snprintf(buffer, 200,  "  : li=%5.2f  eff=% 7.1f  sensor: %s\n\n", Controller->PID.Config.Li, 100 * Controller->PID.Effort, sensor);
      USBSendString(buffer);
    }
    else
    {
      //if (Controller->Heater == 1){
      snprintf(buffer, 200, "Cont%u\t%5.2f\t%5.2f\t%5.2f\t%7.1f\t%7.1f\t%7.1f\t%7.1f\t%8s\t%8s\t%8s\t%2s\t%03u\t%04u\t%s\t%s\n",
          Controller->Heater+1, Controller->PID.Config.Kp, Controller->PID.Config.Kd, Controller->PID.Config.Ki,
          100 * Controller->PID.Ep, 100 * Controller->PID.Ed, 100 * Controller->PID.Ei, 100 * Controller->PID.Effort,
          last, average, target, address, Controller->PID.Config.History, Controller->PID.Config.Frequency, enabled, sensor);
      USBSendString(buffer);
      //}

//      HAL_Delay(1); // don't butcher our buffer before we're done with it
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
    FormatTemperature(last, Compensator->Sensor.LastTemperature);

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

    
    snprintf(buffer, 200, "Comp%u\t%5.2f\t%5.2f\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
        index+1, Compensator->voltage, Compensator->wavelength, last, average, compensating, useaverage, address, enabled, sensor);
    USBSendString(buffer);
}

//Show Everything for the TCB
void ShowAllTCB(struct sTuningControlBoard* sTCB){
  ShowRawHeaderController();
  for(uint8_t i = 0; i < sTCB->NumOfControllers; i++){
    ShowAllController(&sTCB->Controller[i], false);
  }
  ShowRawHeaderCompensator();
  for(uint8_t i = 0; i < sTCB->NumOfCompensators; i++){
    ShowAllCompensator(&sTCB->Compensator[i], false, i);
  }
}
//Show the Hearders for the Raw Data
//=================================================================================================
//Prints the header for the raw data
void ShowRawHeaderController(void)
{
  static char buffer[250];
  snprintf(buffer, 200,  "Cont\tkp\tkd\tki\tep\ted\tei\teffort\ttemp\taverage\ttarget\ti2c\thist\tfreq\tenabled\tsensor\n");
  USBSendString(buffer);
//  HAL_Delay(1); // don't butcher our buffer before we're done with it
}

//Show the Raw Header for the Compensator
void ShowRawHeaderCompensator(void)
{
  static char buffer[250];
  snprintf(buffer, 200,  "Comp\tPeak2Peak\tWave\tTemp\tAvg\tAuto\tUseAverage\ti2c\tenabled\tsensor\n");
  USBSendString(buffer);
//  HAL_Delay(1); // don't butcher our buffer before we're done with it
}


//Formath the Temperature for the Display
//=================================================================================================
//Formats the Float to fit into the Temperature Display
void FormatTemperature(char* buffer, double temp)
{
  if (temp > -100)
    snprintf(buffer, 10, "% 7.3fC", temp);
  else
    snprintf(buffer, 10, "  error ");
}



//Case Switched to Process the User Input
//=================================================================================================
//Parse the input from the main Menu
void ProcessUserInput_MainMenu(struct sTuningControlBoard * s,char* input){

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
  }

  //TODO This Should print all of the interesting Values fromthe TCB
  else if (strcmp(input, "update") == 0 || strcmp(input, "u") == 0)
  {
    //ShowUpdate();
  }
  else if (strcmp(input, "raw") == 0 || strcmp(input, "r") == 0)
  {
    ShowAllTCB(s);
  }
  //This Should reset the TCB  to the default values
  else if (strcmp(input, "wipe") == 0 || strcmp(input, "w") == 0)
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


  //Turn on the Temperature Controller
  if (strcmp((char*) input, "e") == 0)
  {

      sprintf(output, "Compensator %d Enabled.\n", UI_Compensator+1);    
    	USBSendString(output);
      Compensator_enableChannel(&s->Compensator[UI_Compensator], true);
      return;
   }

  //Turn off the Temperature Controller
  if (strcmp((char*) input, "d") == 0)
  {
    //Check to see if the controller is a Compensator
    if (UI_Compensator < 3){
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
        if (f > 0 && f < 500){
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

      default:
        USBSendString("Unknown Command.\n");
        break;
        return;
    }
  }
}

//Parse the input from the Controller Context Menu
void ProcessUserInput_ControllerMenu(struct sTuningControlBoard * s,char* buffer)
{
  uint8_t u = 0;
  char output[250];
  char c;
  float f = 0;

  //Send  to main menu
  if (strcmp(buffer, "m") == 0)
  {
    SUB_MENU = MAIN_MENU;
    UI_Controller = 9;

    return;
  }
   if ((strcmp((char*) buffer, "?") == 0) || (strcmp((char*) buffer, "help") == 0))
  {
    ShowControllerHelp();
    if (UI_Controller == 9)
      USBSendString("No controller selected.\n");
    else
    {
      /*
      ShowSensor(&TCB.Controller);
      ShowControllerConfig(&TCB.Controller);
      ShowEffort(&TCB.Controller);
      USBSendString("\n");
      */
      ShowAllController(&s->Controller[UI_Controller], true);
    }
    return;
  }
  //Select the controller
  if ((strcmp((char*) buffer, "1") == 0) || (strcmp((char*) buffer, "c1") == 0))
  {
    UI_Controller = 0;
    ShowControllerConfig(&s->Controller[UI_Controller], UI_Controller);
    return;
  }
  if ((strcmp((char*) buffer, "2") == 0) || (strcmp((char*) buffer, "c2") == 0))
  {
    UI_Controller = 9;
    USBSendString("Controller 2 not implemented.\n");
    //ShowControllerConfig(Controller);
    return;
  }
  if ((strcmp((char*) buffer, "3") == 0) || (strcmp((char*) buffer, "c3") == 0))
  {

    UI_Controller = 9;
    USBSendString("Controller 3 not implemented.\n");
    //ShowControllerConfig(Controller);
    return;
  }
  if ((strcmp((char*) buffer, "4") == 0) || (strcmp((char*) buffer, "c4") == 0))
  {
    UI_Controller = 9;
    USBSendString("Controller 4 not implemented.\n");
    //ShowControllerConfig(Controller);
    return;
  }
  //If there is no valid Controller Selected
  if (UI_Controller == 9)
  {
    USBSendString("No controller selected.\n");
    return;
  }

  //Print the Status all the Controllers
  if ((strcmp((char*) buffer, "u") == 0) || (strcmp((char*) buffer, "/") == 0))
  {
    ShowAllController(&s->Controller[UI_Controller], true);
    return;
  }
  //Print the Status all the Controllers in non readable format
  if (strcmp((char*) buffer, "r") == 0)
  {
    ShowRawHeaderController();
    ShowAllController(&s->Controller[UI_Controller], false);//Todo Implement this
    return;
  }

  //Enable the Controller
  if (strcmp((char*) buffer, "e") == 0)
  {
    USBSendString("Controller enabled.\n");
    s->Controller[UI_Controller].PID.Config.Enabled = true;
    return;
  }
  //Disable the Controller
  if (strcmp((char*) buffer, "d") == 0)
  {
    USBSendString("Controller disabled.\n");
    s->Controller[UI_Controller].PID.Config.Enabled = false;
    return;
  }


  if (sscanf((char*) buffer, "%c%f", &c, &f) == 2)
  {
    //Convert the float to an integer
    u = (uint8_t) f;
    switch (c)
    {
      case 'c':
        // we shouldn't get here if a valid number was used
        USBSendString("Invalid controller number.\n");
        return;
        break;
      case 'a':
        SetSensor(&s->Controller[UI_Controller].Sensor, u);
        // reset our history since we're changing temperature sensors
        s->Controller[UI_Controller].PID.IntegratorCount = 0;
        break;
      //Set P Constant Valuse
      case 'p':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "kp set to %f.\n", f);
          USBSendString(output);
          s->Controller[UI_Controller].PID.Config.Kp = f;
        }
        return;
        break;
      //Set I Constant Value
      case 'i':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "ki set to %f.\n", f);
          USBSendString(output);
          s->Controller[UI_Controller].PID.Config.Ki = f;
        }
        return;
        break;
      //Set D Constant Value
      case 'd':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "kd set to %f.\n", f);
          USBSendString(output);
          s->Controller[UI_Controller].PID.Config.Kd = f;
        }
        return;
        break;
      //Set the Frequency
      case 'f':
        if ((f < 0) || (f > 600))
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Frequency set to %.0f (%.2f Hz).\n", f, f/40);
          USBSendString(output);
          s->Controller[UI_Controller].PID.Config.Frequency = f;
        }
        return;
        break;
      //Set the limit on the Integral
      case 'l':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Li set to %f.\n", f);
          USBSendString(output);
          s->Controller[UI_Controller].PID.Config.Li = f;
        }
        return;
        break;
      //Set the History Length
      case 'h':
        if (u == 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "History set to %u.\n", u);
          USBSendString(output);
          s->Controller[UI_Controller].PID.Config.History = u;
        }
        return;
        break;
      //Set the Target Temperature
      case 't':
        snprintf(output, 200, "Target temperature set to %f.\n", f);
        USBSendString(output);
        s->Controller[UI_Controller].PID.Config.TargetP = f;
        return;
        break;

      default:
        break;
    }
  }
  USBSendString("Unknown command.\n");
  return;

}

//format the User input
//=================================================================================================
//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_MainMenu(struct sTuningControlBoard * s,char* buffer)
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
  replacestr(buffer, "compensator", "comp");
  ProcessUserInput_MainMenu(s, buffer);

}

//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_CompensatorMenu(struct sTuningControlBoard * s,char * buffer)
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
  replacestr(buffer, "enable", "e");
  replacestr(buffer, "disable", "d");
  replacestr(buffer, "address", "a");
  replacestr(buffer, "main", "m");
  replacestr(buffer, "help", "h");
  replacestr(buffer, "raw", "r");
  ProcessUserInput_CompensatorMenu(s, buffer);
  
}

//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_ControllerMenu(struct sTuningControlBoard * s,char * buffer){
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
  replacestr(buffer, "channel", "c");
//  replacestr(buffer, "controller", "c");
  replacestr(buffer, "chan", "c");
//  replacestr(buffer, "con", "c");
  replacestr(buffer, "address", "a");
  replacestr(buffer, "kp", "p");
  replacestr(buffer, "kd", "d");
  replacestr(buffer, "ki", "i");
  replacestr(buffer, "li", "l");
  replacestr(buffer, "target", "t");
  replacestr(buffer, "temperature", "t");
  replacestr(buffer, "temp", "t");
  replacestr(buffer, "frequency", "f");
  replacestr(buffer, "freq", "f");
  replacestr(buffer, "enable", "n");
  replacestr(buffer, "disable", "f");
  replacestr(buffer, "history", "h");
  replacestr(buffer, "update", "u");
  replacestr(buffer, "raw", "r");
  replacestr(buffer, "main", "m");
  ProcessUserInput_ControllerMenu(s, buffer);
  
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


//Help Menus for End User
//=================================================================================================
//Main Menu help Text
void ShowMainHelp(void)
{
    USBSendString("\nLFDI TCB Firmware v1.3\n");
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
void ShowControllerHelp(void){
    ShowControllerMenuHeader();
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\n");
    USBSendString("\n");
    USBSendString("kp              -- proportional gain\n");
    USBSendString("kd              -- derivative gain\n");
    USBSendString("ki              -- integral gain\n");
    USBSendString("li              -- integral limit\n");
    USBSendString("target          -- target temperature\n");
    USBSendString("history         -- number of samples to average\n");
    USBSendString("frequency       -- number of times per second to update the heater\n");
    USBSendString("enable          -- enable or disable the controller\n");
    USBSendString("disable         -- disable the controller\n");
    USBSendString("address         -- i2c address of the sensor\n");
    USBSendString("raw             -- shows an easily parsable version of the Information\n");
    USBSendString("main            -- return to the main menu\n");
    USBSendString("\n");
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
    USBSendString("disable         -- disable the controller\n");
    USBSendString("address         -- i2c address of the sensor\n");
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
  
  sSensor->Ready = false;
  sSensor->Average = -273;
  sSensor->LastTemperature = -273;
  sSensor->Configured = false;
  sSensor->State = 0;
  return;
}
