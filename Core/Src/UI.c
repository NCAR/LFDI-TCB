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
void ProcessUserInput(struct sTuningControlBoard* TCB, uint8_t* buffer){
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

//Show the Configuration of a Controller
void ShowControllerConfig(struct sController* Controller){
  char s1[12];
  char buffer[250];
  FormatTemperature(s1, Controller->PID.Config.TargetP);
  snprintf(buffer, 200, "C%u: kp=%6.2f  kd=%6.2f  ki=%6.2f  li=%6.2f  target=%8s  history= %3u  frequency= %04u  address=", Controller->Heater,
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
  char buffer[250];
  FormatTemperature(s1, Compensator->Sensor.LastTemperature);
  FormatTemperature(s2, Compensator->Sensor.Average);
  snprintf(buffer, 200, "C%u: Peak2Peak=%6.2f  Wavelength=%6.2f  Temperature_Inst=%8s Temperature_Average=%8s address=",index+1, Compensator->voltage,
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

//Show the Status of a Controller
//@brief Show the Status of a Controller
//@param Controller The Controller to show
//@param readable If true, show the status in human readable form
void ShowAll(struct sController* Controller, bool readable)
{
    uint32_t ADCVal = 0;
    uint8_t i;
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
        ShowRawHeader();
      snprintf(buffer, 200, "C%u\t%5.2f\t%5.2f\t%5.2f\t%7.1f\t%7.1f\t%7.1f\t%7.1f\t%8s\t%8s\t%8s\t%2s\t%03u\t%04u\t%s\t%s\n",
          Controller->Heater, Controller->PID.Config.Kp, Controller->PID.Config.Kd, Controller->PID.Config.Ki,
          100 * Controller->PID.Ep, 100 * Controller->PID.Ed, 100 * Controller->PID.Ei, 100 * Controller->PID.Effort,
          last, average, target, address, Controller->PID.Config.History, Controller->PID.Config.Frequency, enabled, sensor);
      USBSendString(buffer);
      //}

//      HAL_Delay(1); // don't butcher our buffer before we're done with it
    }
}


//Prints the header for the raw data
void ShowRawHeader(void)
{
  static char buffer[250];
  snprintf(buffer, 200,  "Chan\t  kp\t  kd\t  ki\t    ep\t    ed\t    ei\t  effort temp\t         average          target        i2c\thist\tfreq\tenabled\t       sensor\n");
  USBSendString(buffer);
//  HAL_Delay(1); // don't butcher our buffer before we're done with it
}


//Formats the Float to fit into the Temperature Display
void FormatTemperature(char* buffer, double temp)
{
  if (temp > -100)
    snprintf(buffer, 10, "% 7.3fC", temp);
  else
    snprintf(buffer, 10, "  error ");
}


//Main Menu help Text
void ShowMainHelp(void)
{
    USBSendString("\nLFDI TCB Firmware v1.3\n");
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
    USBSendString("\nController Context Menu\n");
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
    USBSendString("back            -- return to the main menu\n");
    USBSendString("\n");
}


//Show the Compensator Context Menu
void ShowCompensatorHelp(void){
    USBSendString("\nCompensator Context Menu\n");
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\n");
    USBSendString("\n");
    USBSendString("volt            -- Peak to Peak Voltage output\n");
    USBSendString("comp            -- Toggle Auto Compensation output\n");
    USBSendString("wave            -- Set the Wavelength to Compensation to\n");
    USBSendString("enable          -- enable or disable the controller\n");
    USBSendString("disable         -- disable the controller\n");
    USBSendString("address         -- i2c address of the sensor\n");
    USBSendString("back            -- return to the main menu\n");
    USBSendString("\n");
}

//Parse the input from the main Menu
void ProcessUserInput_MainMenu(struct sTuningControlBoard * s,uint8_t* input){

  //Pull up the Controller Sub menu
  if (strcmp(input, "cont") == 0)
  { 
    SUB_MENU = CONTROLLER_MENU;
  }

  //Pull up the Compensator Sub menu
  else if (strcmp(input, "comp") == 0)
  { 
    SUB_MENU = COMPENSATOR_MENU;
  }

  //TODO This Should print all of the interesting Values fromthe TCB
  else if (strcmp(input, "update") == 0 || strcmp(input, "u") == 0)
  {
    //ShowUpdate();
  }
  else if (strcmp(input, "raw") == 0 || strcmp(input, "r") == 0)
  {
    //ShowRaw();
  }
  //This Should reset the TCB back to the default values
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
void ProcessUserInput_CompensatorMenu(struct sTuningControlBoard * s,uint8_t* input)
{
  uint16_t i = 0;
  uint8_t u = 0;
  char output[250];
  char c;
  float f = 0;

  //Send back to main menu
  if (strcmp(input, "b") == 0)
  {
    SUB_MENU = MAIN_MENU;
    return;
  }
  //Show Help
  else if (strcmp(input, "h") == 0)
  {
    ShowCompensatorHelp();
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
        s->Compensator[UI_Compensator].voltage = f;
        s->Compensator[UI_Compensator].compensate = false;
        //Print out the string with the Compensator number and voltage
        sprintf(output, "Compensator %d Voltage Set to %f.\n", UI_Compensator+1, f);    
    	  USBSendString(output);
    
        break;
        return;

      case 'w':
        s->Compensator[UI_Compensator].wavelength = f;
        sprintf(output, "Compensator %d Wavelength Set to %f.\n", UI_Compensator+1, f);
        USBSendString(output);
        break;
        return;
      default:
        USBSendString("Unknown Command.\n");
        break;
        return;
    }
  }
}

//Parse the input from the Controller Context Menu
void ProcessUserInput_ControllerMenu(struct sTuningControlBoard * s,uint8_t* input)
{
  ShowControllerHelp();
  USBSendString("Controller Menu\n");
  USBSendString("Not Implemented Yet\n");
  SUB_MENU = MAIN_MENU;

}



//Trranslate the input buffers to be one letter commands pass onto the case switch menu
void TranslateUserInput_MainMenu(struct sTuningControlBoard * s,uint8_t* buffer)
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
void TranslateUserInput_CompensatorMenu(struct sTuningControlBoard * s,uint8_t * buffer)
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
  replacestr(buffer, "back", "b");
  replacestr(buffer, "help", "h");
  ProcessUserInput_CompensatorMenu(s, buffer);
  
}


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
