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

uint8_t UI_Controller = 9;


//Get the User Input and process it
//@param Controller: pointer to the controller struct
//@param buffer: pointer to the buffer
void ProcessUserInput(struct sTuningControlBoard* TCB, uint8_t* buffer)
{
  uint16_t i = 0;
  uint8_t u = 0;
  char output[250];
  char c;
  float f = 0;
  for (i=0; buffer[i]; i++)
    buffer[i] = tolower(buffer[i]);
//  printf("String: %s\n", buffer);

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
  replacestr(buffer, "save", "s");
  replacestr(buffer, "load", "l");
  replacestr(buffer, "history", "h");
  replacestr(buffer, "update", "u");
  replacestr(buffer, "raw", "r");
  replacestr(buffer, "bounce", "b");
  replacestr(buffer, "wipe", "w");
  replacestr(buffer, "voltage", "v");
  replacestr(buffer, "heater", "heater");


//  printf("New String: %s\n", buffer);

  if ((strcmp((char*) buffer, "?") == 0) || (strcmp((char*) buffer, "help") == 0))
  {
    USBSendString("\nLFDI TCB Firmware v1.3\n");
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\n");
    USBSendString("\n");
//    USBSendString("Controller = n  -- chooses which controller to configure\n");
    USBSendString("Channel = n     -- chooses which Compensator channel to configure\n");
    USBSendString("Heater  = n     -- chooses the Heater channel to configure\n");
    USBSendString("Address = nn    -- sets the address of the temperature sensor of the current Compensator Channel or the Heater (00, 01, 10, or 11)\n");
    USBSendString("kP = n.nn       -- sets the proportional gain for the Selected Heater\n");
    USBSendString("kD = n.nn       -- sets the derivative gain for the Selected Heater\n");
    USBSendString("kI = n.nn       -- sets the integral gain for the Selected Heater\n");
    USBSendString("Li = n.nn       -- sets the integral gain limit for the Selected Heater\n");
    USBSendString("History = nn    -- sets the number of points used in the integral (1-255) for the Selected Heater\n");
    USBSendString("Target = n      -- sets the target temperature for the Selected Heater\n");
    USBSendString("Temperature = n -- sets the target temperature for the Selected Heater\n");
    USBSendString("Freq = n        -- sets the PWM frequency (40 = 1 second) for the Selected Heater\n");
    USBSendString("Enable          -- starts the temperature control loop for the Heater or the Compensator\n");
    USBSendString("Disable         -- stops the temperature control loop for the Heater or the Compensator\n");
    USBSendString("Update          -- shows the status of all of the controllers and compensators\n");
    USBSendString("Raw             -- shows an easily parsable version of Update\n");
    USBSendString("Wipe            -- wipes the existing configuration and load new defaults\n");
    USBSendString("Bounce          -- performs a power-cycle / reboot on the system\n");
    USBSendString("Load            -- reloads the previously saved values (automatic at power-on)\n");
    USBSendString("Save            -- saves the currently configured values\n");
    USBSendString("Voltage         -- Set the Peak to Peak Voltage to output on Compensator\n");
    USBSendString("\n");
    USBSendString("Compensate\n");
    
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
      ShowAll(&TCB->Controller, true);
    }
    return;
  }

  //Print the Status all the Controllers
  if ((strcmp((char*) buffer, "u") == 0) || (strcmp((char*) buffer, "/") == 0))
  {
    ShowAll(&TCB->Controller, true);
    return;
  }
  //Print the Status all the Controllers in non readable format
  if (strcmp((char*) buffer, "r") == 0)
  {
    ShowAll(&TCB->Controller, false);
    return;
  }

  //Save the Configuration of a controller
  if (strcmp((char*) buffer, "s") == 0)
  {

    USBSendString("Cannot Save, No EEPROM.\n");
    return;
  }

//Load the Configuration of a controller
  if (strcmp((char*) buffer, "l") == 0)
  {
    USBSendString("Cannot load Configuration, No EEPROM.\n");
    return;
  }

  //Reset the Controller
  if (strcmp((char*) buffer, "b") == 0)
  {
    NVIC_SystemReset();
  }

  //Wipe the Configuration of a controller
  if (strcmp((char*) buffer, "w") == 0)
  {
    Controller_WipeConfig(&TCB->Controller);
    USBSendString("Configuration wiped.\n");
    return;
  }

  //Choose the Compensator to Configure
  if ((strcmp((char*) buffer, "1") == 0) || (strcmp((char*) buffer, "c1") == 0))
  {
    UI_Controller = 0;
    ShowControllerConfig(&TCB->Controller);
    return;
  }
  if ((strcmp((char*) buffer, "2") == 0) || (strcmp((char*) buffer, "c2") == 0))
  {
    UI_Controller = 1;
    ShowControllerConfig(&TCB->Controller);
    return;
  }

  if ((strcmp((char*) buffer, "3") == 0) || (strcmp((char*) buffer, "c3") == 0))
  {
    UI_Controller = 2;
    ShowControllerConfig(&TCB->Controller);
    return;
  }

  if ((strcmp((char*) buffer, "4") == 0) || (strcmp((char*) buffer, "c4") == 0))
  {
    UI_Controller = 3;
    ShowControllerConfig(&TCB->Controller);
    return;
  }
  if ((strcmp((char*) buffer, "5") == 0) || (strcmp((char*) buffer, "c5") == 0))
  {
    UI_Controller = 4;
    ShowControllerConfig(&TCB->Controller);
    return;
  }
  if ((strcmp((char*) buffer, "6") == 0) || (strcmp((char*) buffer, "c6") == 0))
  {
    UI_Controller = 5;
    ShowControllerConfig(&TCB->Controller);
    return;
  }
  //This is for the Heater
  if ((strcmp((char*) buffer, "7") == 0) || (strcmp((char*) buffer, "c7") == 0))
  {
    UI_Controller = 6;
    ShowControllerConfig(&TCB->Controller);
    return;
  }
  
  
  if (UI_Controller == 9)
  {
    USBSendString("No controller selected.\n");
    return;
  }
  //Turn on the Temperature Controller
  if (strcmp((char*) buffer, "e") == 0)
  {
    if (UI_Controller == 6){
      USBSendString("Controller enabled.\n");
      TCB->Controller.PID.Config.Enabled = true;
      return;
    }
    //Check to see if the controller is a Compensator
    if (UI_Controller < 6){
      USBSendString("Compensator enabled.\n");
      TCB->Compensator[UI_Controller].Enable = true;
      return;
    }
  }
  //Turn off the Temperature Controller
  if (strcmp((char*) buffer, "d") == 0)
  {

    if (UI_Controller == 6){
      USBSendString("Controller disabled.\n");
      TCB->Controller.PID.Config.Enabled = false;
      return;
    }
    //Check to see if the controller is a Compensator
    if (UI_Controller < 6){
      USBSendString("Compensator disabled.\n");
      TCB->Compensator[UI_Controller].Enable = false;
      return;
    }
  }
  //if the incoming command is a {character}{Float}
  if (sscanf((char*) buffer, "%c%f", &c, &f) == 2)
  {
    //Convert number to an integer
    u = (uint8_t) f;
    //Switch on the character
    switch (c)
    {
      //User is trying to set the Channel
      case 'c':
        // we shouldn't get here if a valid number was used
        USBSendString("Invalid controller number.\n");
        return;
        break;
      //User is trying to set the Sensor Address
      case 'a':
        switch (u)
        {
          case 0:
            USBSendString("Address set to 0b 10 01 00 0x.\n");
            if(UI_Controller == 6){
              TCB->Controller.Sensor.Address = 0b1001000;
            }else{
              TCB->Compensator[UI_Controller].Sensor.Address = 0b1001000;
            }
            return;
            break;
          case 10:
            USBSendString("Address set to 0b 10 01 01 0x.\n");
            if(UI_Controller == 6){
              TCB->Controller.Sensor.Address = 0b1001010;
            }else{
              TCB->Compensator[UI_Controller].Sensor.Address = 0b1001010;
            }
            
            return;
            break;
          case 1:
            USBSendString("Address set to 0b 10 01 00 1x.\n");
            if(UI_Controller == 6){
              TCB->Controller.Sensor.Address = 0b1001001;
            }else{
              TCB->Compensator[UI_Controller].Sensor.Address = 0b1001001;
            }
            return;
            break;
          case 11:
            USBSendString("Address set to 0b 10 01 01 1x.\n");
            if(UI_Controller == 6){
              TCB->Controller.Sensor.Address = 0b1001011;
            }else{
              TCB->Compensator[UI_Controller].Sensor.Address = 0b1001011;
            }
            return;
            break;
          default:
            USBSendString("Invalid Address.\n");
            return;
            break;
        }
        // reset our history since we're changing temperature sensors
        if(UI_Controller == 6){
          TCB->Controller.Sensor.Ready = false;
          TCB->Controller.Sensor.Average = -273;
          TCB->Controller.Sensor.LastTemperature = -273;
          TCB->Controller.Sensor.Configured = false;
          TCB->Controller.Sensor.State = 0;
          TCB->Controller.PID.IntegratorCount = 0;
        }else{
          TCB->Compensator[UI_Controller].Sensor.Ready = false;
          TCB->Compensator[UI_Controller].Sensor.Average = -273;
          TCB->Compensator[UI_Controller].Sensor.LastTemperature = -273;
          TCB->Compensator[UI_Controller].Sensor.Configured = false;
          TCB->Compensator[UI_Controller].Sensor.State = 0;
        break;
      //Set KP
      case 'p':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "kp set to %f.\n", f);
          USBSendString(output);
          TCB->Controller.PID.Config.Kp = f;
        }
        return;
        break;
      //Set KI
      case 'i':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "ki set to %f.\n", f);
          USBSendString(output);
          TCB->Controller.PID.Config.Ki = f;
        }
        return;
        break;
      //Set KD
      case 'd':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "kd set to %f.\n", f);
          USBSendString(output);
          TCB->Controller.PID.Config.Kd = f;
        }
        return;
        break;
      //Set the Frequency at which the Duty Cycle is updated
      case 'f':
        if ((f < 0) || (f > 600))
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Frequency set to %.0f (%.2f Hz).\n", f, f/40);
          USBSendString(output);
          TCB->Controller.PID.Config.Frequency = f;
        }
        return;
        break;

      //Set the Limit of the Integrator
      case 'l':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Li set to %f.\n", f);
          USBSendString(output);
          TCB->Controller.PID.Config.Li = f;
        }
        return;
        break;
      //Set the History of the PID
      case 'h':
        if (u == 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "History set to %u.\n", u);
          USBSendString(output);
          TCB->Controller.PID.Config.History = u;
        }
        return;
        break;
      //Set the Target Temperature
      case 't':
        snprintf(output, 200, "Target temperature set to %f.\n", f);
        USBSendString(output);
        TCB->Controller.PID.Config.TargetP = f;
        return;
        break;
      //Set the Voltage of the DAC Channel
      case 'v':
    	  snprintf(output, 200, "Voltage of DAC Channel 0 set to %f.\n", f);
    	  USBSendString(output);
    	  if((UI_Controller > 5)||(UI_Controller < 0)){
          USBSendString("Invalid Controller.\n");
          return;
    	  }else{
        	TCB->Compensator[UI_Controller].voltage = f;
    	  }
    	  return;
    	  break;

      default:
        break;
    }
  }
  USBSendString("Unknown command.\n");
  return;
}
}
//Show the Configuration of a Controller
void ShowControllerConfig(struct sController* Controller)
{
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


//Show an individual sensor
void ShowSensor(struct sController* Controller)
{
  char buffer[250];
  char s1[12], s2[12], s3[12];
  FormatTemperature(s1, Controller->Sensor.Average);
  FormatTemperature(s2, Controller->Sensor.LastTemperature);

  snprintf(buffer, 200, "temp=%8s avg=%8s sensor: ", s2, s1);
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

//Show the Compensator of a Controller
//TODO Need to Fill this out
void ShowCompensator(struct sCompensator* Compensator)
{
  char buffer[250];
  char s1[12], s2[12], s3[12];
  FormatTemperature(s1, Compensator->Sensor.Average);
  FormatTemperature(s2, Compensator->Sensor.LastTemperature);
  FormatTemperature(s3, Compensator->voltage);

  //Todo Fill out here
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
//TODO Need to add to this
void ShowRawHeader(void)
{
  static char buffer[250];
  snprintf(buffer, 200,  "Chan\tkp\tkd\tki\tep\ted\tei\teffort\ttemp\taverage\ttarget\ti2c\thist\tfreq\tenabled\t       sensor\n");
  USBSendString(buffer);
//  HAL_Delay(1); // don't butcher our buffer before we're done with it
}


//Formats the Float to fit into the Temperature Display
void FormatTemperature(char* buffer, double temp)
{
  if (temp > -100){
    snprintf(buffer, 10, "% 7.3fC", temp);
  }else{
    snprintf(buffer, 10, "  error ");
  }
 }

