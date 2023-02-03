# LFDI-TCB
Software for the Lyot Filter Demonstration Instrument Tuning Control Board

The Software is organized in an Object oriented Fashion to help coordinate Functionality of the TCB

## Top Level Structure
### Tuning Control Board Structure

The The Top level structure representing the Entire TCB with the Top Level Structure called TuningControlBoard 
This Structure Contains 6 Compensators and 1 Controller which Can be extended in the Future

From TuningControlBoard.h
```
struct sTuningControlBoard
{
  struct sDAC DAC8718;
  struct sTMP117 Sensor[7];
  
  uint8_t NumOfControllers;
  uint8_t NumOfCompensators;
  struct sController Controller[1];//INcrease this as we get more controllers
  struct sCompensator Compensator[6];
  //Camera Controller

};
```
This top level structure allows for coordination between the Functionalities of the Tuning Control Board The Compensators and the Controllers
Later on comera control and FLC control and be Coordinated as well


## Mid-level Structures
### Controller
The Controller Structure helps control the Heater conntected to the PCB.
It's composed of two hardware level and one software level structure
The Controler will control the Heat based onthe TMP117 Heater and the PID structure settings

```
struct sController
{
  uint8_t Heater;
  struct sTMP117 Sensor;
  struct sPID PID;
};
```

### Compensator
The Compensator controls the tuning of the LCVR based on the Temperature
```
//Structure that Pairs a specific Temperature Sensor with Specific DAC Channel
//And Performs Compensation algorythom to set output voltage
struct sCompensator
{
  struct sTMP117 Sensor;
  struct sDAC_Channel Channel;
  //is the Channel enabled
  bool Enable;
  //Should we be tryying to Compensate to a specific Wavelength
  bool compensate;
  //Use the Average stored in the TMP117 struct or the last temperature
  bool useAverage;

  //Wavelength to Compensate to
  float wavelength;
  //if we are not using the auto compensation what should this be outputting
  float voltage;

};
```
