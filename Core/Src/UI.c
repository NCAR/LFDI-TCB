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

uint8_t UI_Controller = 9;


//Get the User Input and process it
//@param Controller: pointer to the controller struct
//@param buffer: pointer to the buffer
void ProcessUserInput(struct sController* Controller, uint8_t* buffer, struct sTuningControlBoard* TCB)
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


//  printf("New String: %s\n", buffer);

  if ((strcmp((char*) buffer, "?") == 0) || (strcmp((char*) buffer, "help") == 0))
  {
    USBSendString("\nLFDI TCB Firmware v1.2\n");
    USBSendString("Commands can be upper or lower case. Variables can be set with an equals sign or space or nothing.\n");
    USBSendString("\"channel=1\", \"channel 1\", \"channel1\", \"c1\" are all treated the same.\n");
    USBSendString("\n");
//    USBSendString("Controller = n  -- chooses which controller to configure\n");
    USBSendString("Channel = n     -- chooses which channel to configure\n");
    USBSendString("Address = nn    -- sets the address of the temperature sensor (00, 01, 10, or 11)\n");
    USBSendString("kP = n.nn       -- sets the proportional gain\n");
    USBSendString("kD = n.nn       -- sets the derivative gain\n");
    USBSendString("kI = n.nn       -- sets the integral gain\n");
    USBSendString("Li = n.nn       -- sets the integral gain limit\n");
    USBSendString("History = nn    -- sets the number of points used in the integral (1-255)\n");
    USBSendString("Target = n      -- sets the target temperature\n");
    USBSendString("Temperature = n -- sets the target temperature\n");
    USBSendString("Freq = n        -- sets the PWM frequency (40 = 1 second)\n");
    USBSendString("Enable          -- starts the temperature control loop\n");
    USBSendString("Disable         -- stops the temperature control loop\n");
    USBSendString("Update          -- shows the status of all of the controllers\n");
    USBSendString("Raw             -- shows an easily parsable version of Update\n");
    USBSendString("Wipe            -- wipes the existing configuration and load new defaults\n");
    USBSendString("Bounce          -- performs a power-cycle / reboot on the system\n");
    USBSendString("Load            -- reloads the previously saved values (automatic at power-on)\n");
    USBSendString("Save            -- saves the currently configured values\n");
    USBSendString("Voltage         -- Set the Peak to Peak Voltage to output on DAC Channel 0\n");
    USBSendString("\n");
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
      ShowAll(Controller, true);
    }
    return;
  }

  //Print the Status all the Controllers
  if ((strcmp((char*) buffer, "u") == 0) || (strcmp((char*) buffer, "/") == 0))
  {
    ShowAll(Controller, true);
    return;
  }
  //Print the Status all the Controllers in non readable format
  if (strcmp((char*) buffer, "r") == 0)
  {
    ShowAll(Controller, false);
    return;
  }
  //Save the Configuration of a controller
  if (strcmp((char*) buffer, "s") == 0)
  {

    USBSendString("Cannot Save, No EEPROM.\n");
    return;
  }

  if (strcmp((char*) buffer, "l") == 0)
  {
    USBSendString("Cannot load Configuration, No EEPROM.\n");
    return;
  }

  if (strcmp((char*) buffer, "b") == 0)
  {
    NVIC_SystemReset();
  }

  if (strcmp((char*) buffer, "w") == 0)
  {
    Controller_WipeConfig(Controller);
    USBSendString("Configuration wiped.\n");
    return;
  }

  if ((strcmp((char*) buffer, "1") == 0) || (strcmp((char*) buffer, "c1") == 0))
  {
    UI_Controller = 0;
    ShowControllerConfig(Controller);
    return;
  }

  if ((strcmp((char*) buffer, "2") == 0) || (strcmp((char*) buffer, "c2") == 0))
  {
    UI_Controller = 1;
    ShowControllerConfig(Controller);
    return;
  }

  if ((strcmp((char*) buffer, "3") == 0) || (strcmp((char*) buffer, "c3") == 0))
  {
    UI_Controller = 2;
    ShowControllerConfig(Controller);
    return;
  }

  if ((strcmp((char*) buffer, "4") == 0) || (strcmp((char*) buffer, "c4") == 0))
  {
    UI_Controller = 3;
    ShowControllerConfig(Controller);
    return;
  }

  if (UI_Controller == 9)
  {
    USBSendString("No controller selected.\n");
    return;
  }


  if (strcmp((char*) buffer, "e") == 0)
  {
    USBSendString("Controller enabled.\n");
    Controller->PID.Config.Enabled = true;
    return;
  }

  if (strcmp((char*) buffer, "d") == 0)
  {
    USBSendString("Controller disabled.\n");
    Controller->PID.Config.Enabled = false;
    return;
  }

  if (sscanf((char*) buffer, "%c%f", &c, &f) == 2)
  {
    u = (uint8_t) f;
    switch (c)
    {
      case 'c':
        // we shouldn't get here if a valid number was used
        USBSendString("Invalid controller number.\n");
        return;
        break;
      case 'a':
        switch (u)
        {
          case 0:
            USBSendString("Address set to 0b 10 01 00 0x.\n");
            Controller->Sensor.Address = 0b1001000;
            return;
            break;
          case 10:
            USBSendString("Address set to 0b 10 01 01 0x.\n");
            Controller->Sensor.Address = 0b1001010;
            return;
            break;
          case 1:
            USBSendString("Address set to 0b 10 01 00 1x.\n");
            Controller->Sensor.Address = 0b1001001;
            return;
            break;
          case 11:
            USBSendString("Address set to 0b 10 01 01 1x.\n");
            Controller->Sensor.Address = 0b1001011;
            return;
            break;
          default:
            USBSendString("Invalid Address.\n");
            return;
            break;
        }
        // reset our history since we're changing temperature sensors
        Controller->Sensor.Ready = false;
        Controller->Sensor.Average = -273;
        Controller->Sensor.LastTemperature = -273;
        Controller->Sensor.Configured = false;
        Controller->Sensor.State = 0;
        Controller->PID.IntegratorCount = 0;
        break;
      case 'p':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "kp set to %f.\n", f);
          USBSendString(output);
          Controller->PID.Config.Kp = f;
        }
        return;
        break;

      case 'i':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "ki set to %f.\n", f);
          USBSendString(output);
          Controller->PID.Config.Ki = f;
        }
        return;
        break;

      case 'd':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "kd set to %f.\n", f);
          USBSendString(output);
          Controller->PID.Config.Kd = f;
        }
        return;
        break;

      case 'f':
        if ((f < 0) || (f > 600))
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Frequency set to %.0f (%.2f Hz).\n", f, f/40);
          USBSendString(output);
          Controller->PID.Config.Frequency = f;
        }
        return;
        break;


      case 'l':
        if (f < 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "Li set to %f.\n", f);
          USBSendString(output);
          Controller->PID.Config.Li = f;
        }
        return;
        break;
      case 'h':
        if (u == 0)
          USBSendString("Invalid value.");
        else
        {
          snprintf(output, 200, "History set to %u.\n", u);
          USBSendString(output);
          Controller->PID.Config.History = u;
        }
        return;
        break;

      case 't':
        snprintf(output, 200, "Target temperature set to %f.\n", f);
        USBSendString(output);
        Controller->PID.Config.TargetP = f;
        return;
        break;

      case 'v':
    	  snprintf(output, 2000, "Voltage of DAC Channel 0 set to %f.\n", f);
    	  USBSendString(output);
    	  //Need to implement Voltage for the Dac Channels
    	  Set_Voltage_Peak_to_Peak(TCB->DAC8718, 0, f);
    	  return;
    	  break;

      default:
        break;
    }
  }
  USBSendString("Unknown command.\n");
  return;
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

