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
The Compensator uses one TMP117 and one DAC Channel

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

## Low-Level
These Structure will mostly be controlling Hardware and peripeherials attached to the TCB

### DAC
The Structure is To control the DAC8718 this will preform entire device functionality like setting up at start up adjusting registers and actually reading and writting to the device registers
```
//Struct to hold the DAC Information
struct sDAC{
	SPI_HandleTypeDef* spi;
	uint16_t config;
	uint16_t offset_a;
	uint16_t offset_b;
	bool Configured;
	uint8_t State;
	bool Ready;
	uint16_t Errors;
	float max_peak2peak;
	struct sDAC_Channel DAC_Channels[8];

};
```
### DAC Channel

The Dac channel structure is mostlyt for organizational purposes. It stores the upper and lower bound that that Channel should be going between as well as its current state. 
```
//Struct to hold the DAC Channels Information
struct sDAC_Channel{
	uint16_t upper_bound;
	uint16_t lower_bound;
	uint8_t DAC_number;
    bool state_high;
	bool enabled;
	float max_peak2peak;
};
```
### TMP117
This helps command control and store data for the TMP117 Temperature sensor

```
struct sTMP117
{
  uint8_t Address;
  I2C_HandleTypeDef* Interface;
  double Temperature[64];
  double Average;
  bool Configured;
  uint8_t Index;
  uint8_t SamplesInAverage;
  uint8_t State;
  double LastTemperature;
  bool Ready;
  uint16_t Errors;
};
```


### PID
A software level implementation of a pid Controller
```
struct sPID
{
  float LastP[256];
  float DeltaT, Ep, Ei, Ed, Effort;
  uint8_t IntegratorCount;
  struct sPID_Config Config;
};
```
