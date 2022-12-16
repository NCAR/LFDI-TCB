#include "DAC.h"
#include "utils.h"

//Read/Write Should be OR-ed with the Register address
uint8_t WRITE = 0x00;
uint8_t READ = 0x80;
//Addresses of Registers (Last 5 bits)
uint8_t CONFIG = 0x00;
uint8_t ANALOG_MONITOR_SELECT = 0x01;
uint8_t GPIO_REGISTER =  0x02;
uint8_t OFFSET_A = 0x03;
uint8_t OFFSET_B = 0x04;
uint8_t SPI_MODE = 0x06;
uint8_t BROADCAST = 0x07;
uint8_t Set_DAC0 = 0x08;
uint8_t Set_DAC1 = 0x09;
uint8_t Set_DAC2 = 0x0A;
uint8_t Set_DAC3 = 0x0B;
uint8_t Set_DAC4 = 0x0C;
uint8_t Set_DAC5 = 0x0D;
uint8_t Set_DAC6 = 0x0E;
uint8_t Set_DAC7 = 0x0F;
uint8_t ZERO_0 = 0x10;
uint8_t GAIN_0 = 0x18;
uint8_t ZERO_1 = 0x11;
uint8_t GAIN_1 = 0x19;
uint8_t ZERO_2 = 0x12;
uint8_t GAIN_2 = 0x1A;
uint8_t ZERO_3 = 0x13;
uint8_t GAIN_3 = 0x1B;
uint8_t ZERO_4 = 0x14;
uint8_t GAIN_4 = 0x1C;
uint8_t ZERO_5 = 0x15;
uint8_t GAIN_5 = 0x1D;
uint8_t ZERO_6 = 0x16;
uint8_t GAIN_6 = 0x1E;
uint8_t ZERO_7 = 0x17;
uint8_t GAIN_7 = 0x1F;

//Config Register Bits For the MSB REGISTER
uint8_t AB = 0x80;
uint8_t LD = 0x40;
uint8_t RST = 0x20;
uint8_t PD_A = 0x10;
uint8_t PD_B = 0x08;
uint8_t SCE = 0x04;
uint8_t No_EFFECT = 0x02;
uint8_t GAIN_A = 0x01;
//Config Register Bits For the LSB REGISTER
uint8_t GAIN_B = 0x80;
uint8_t DSDO = 0x40;
const uint8_t NOP = 0x20;
uint8_t W2 = 0x10;
//NOP Command
//Used when recieving data from the DAC
uint8_t NOP_Send[3] = {0x00, 0x00, NOP};
//Max Voltage peak to peak
float MAX_PEAK2PEAK = 22.4;

//@brief: This function will set a configuration value to the configuration register
//@param: spi: The spi handle to use
//@return: None
void Set_Config(SPI_HandleTypeDef* spi){
	uint8_t Data[3] = {0x00,0x00, 0x00};
	uint8_t read_command[3] = {READ|CONFIG, 0x00, 0x00};
	Set_nRST_high(false);
	HAL_Delay(10);
	Set_nRST_high(true);
	
	Send_Command(spi, read_command);
	Recieve_Data(spi, Data);
	//Set the DAC to use a gain of 1 for both a and b registers meaning.
	//Make sure SCE is 0. This will make it so that the DAC will use the Values direclty written to it
	//rather than trying to use the individual offset and gain of each register to re-adjust
	//the value
	//Gain_A = 1 -> gain is set to 4xVref GAIN_A = 0 -> gain is set to 6xVref
	
	uint8_t tx_data[3] = {WRITE|CONFIG, AB, 0x00};
	Send_Command(spi, tx_data);
	Send_Command(spi, read_command);
	Recieve_Data(spi, Data);

	return;

}

//@brief: This function will get the Value of the configuration register
//@param: spi: The spi handle to use
//@return: The value of the configuration register
uint16_t Get_Config(SPI_HandleTypeDef* spi){
	uint8_t Data[3] = {0x00,0x00, 0x00};
	uint8_t read_command[3] = {READ|CONFIG, 0x00, 0x00};
	Send_Command(spi, read_command);
	Recieve_Data(spi, Data);
	return (Data[1] << 8) | Data[2];
}

//@brief: This function will ramp all the DACs from 0 to FFFF
void Ramp_DACs(SPI_HandleTypeDef* spi, uint8_t DAC_NUM , uint16_t increment, uint16_t lower_bound, uint16_t upper_bound){
	for(uint16_t Value = lower_bound; Value <= upper_bound; Value = Value+increment){
		Set_DAC_Value(spi, DAC_NUM, Value);
		Set_nLDAC_high(true);
		Set_nLDAC_high(false);
		Set_nLDAC_high(true);
		HAL_Delay(10);
	}
}
//@brief: This function will send a command to the DAC
//        The command is a 3 byte array
//        The first byte is the address of the register
//        The second byte is the MSB of the register
//        The third byte is the LSB of the register
//@param: spi: The spi handle to use
//@param: command: The command to send
//@return: none
void Send_Command(SPI_HandleTypeDef* spi, uint8_t* Command){
	HAL_StatusTypeDef hal_status;
	Set_nCS_high(false);
	hal_status = HAL_SPI_Transmit(spi, Command, 3, 1);
	while (!(spi->Instance->SR & SPI_SR_TXE));
	Set_nCS_high(true);
	Delay_us(10);
}

//@brief: This function recieves data from the DAC and stores it in the data array
//@param: spi: The spi handle
//@param: Data: buffer to store the data in
//@return: None
void Recieve_Data(SPI_HandleTypeDef* spi, uint8_t* Data){
	HAL_StatusTypeDef hal_status;
	Set_nCS_high(false);
	hal_status = HAL_SPI_TransmitReceive(spi,NOP_Send, Data, 3, 100);
	while ((spi->Instance->SR & SPI_SR_RXNE));
	Set_nCS_high(true);
	Delay_us(10);

}

//@brief: This function sets the value of the given dac
//@param: DAC_Num: The DAC number to set the offset of
//@param: val: The value to set the dac to
//@param spi: The spi handle to use
//@return: None
void Set_DAC_Value(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val){
	//Make sure DAC is between 0 and 7
	if(DAC_Num > 7){
		return;
	}
	uint8_t DAC_REG = Set_DAC0+DAC_Num;

	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {WRITE|DAC_REG, (val >> 8), (val & 0xFF)};
	
	Send_Command(spi, tx_data);
	
}

//@brief: Set the DAC to the max value
//@param: DAC_Num: The DAC number to set the offset of
//@param spi: The spi handle to use
//@return: None
void Set_DAC_Max(SPI_HandleTypeDef* spi, uint8_t DAC_Num){
	uint8_t DAC_REG = Set_DAC0+DAC_Num;
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {WRITE|DAC_REG, 0xFF, 0xFF};
	Send_Command(spi, tx_data);
}
void Set_DAC_Min(SPI_HandleTypeDef* spi, uint8_t DAC_Num){
	uint8_t DAC_REG = Set_DAC0+DAC_Num;
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {WRITE|DAC_REG, 0x00, 0x00};
	Send_Command(spi, tx_data);
}

//@brief: This function sets the value of the offset of the given dac
//@param: DAC_Num: The DAC number to set the offset of
//@param: val: The value to set the offset to
//@param spi: The spi handle to use
//@return: None
void Set_DAC_Zero(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val){
	//Make sure DAC is between 0 and 7
	if(DAC_Num > 7){
		return;
	}
	uint8_t DAC_REG = ZERO_0+(DAC_Num*2);
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {WRITE|DAC_REG, (val>>8)&0xFF, val&0xFF};
	Send_Command(spi, tx_data);
	
}

//@brief: This function sets the value of the gain of the given dac
//@param: DAC_Num: The DAC number to set the gain of
//@param: val: The value to set the gain to
//@param spi: The spi handle to use
//@return: None
void Set_DAC_Gain(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t val){
	//Make sure DAC is between 0 and 7
	if(DAC_Num > 7){
		return;
	}
	uint8_t DAC_REG = GAIN_0+(DAC_Num*2);
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {WRITE|DAC_REG, (val>>8)&0xFF, val&0xFF};
	Send_Command(spi, tx_data);
	
}

//@brief: This function sets the value of all the DACs to 0x0000
//@param spi: The spi handle to use
//@return: None
void Set_All_DACs_to_Zero(SPI_HandleTypeDef* spi){
	
	for (int i = Set_DAC0; i <= Set_DAC7; i++){
		uint8_t tx_data[3] = {WRITE|i, 0x7f, 0xff};
		Send_Command(spi, tx_data);	
	}
				  Set_nLDAC_high(true);
				  Set_nLDAC_high(false);
				  Set_nLDAC_high(true);
}

//@brief: This function sets the value of the of the given dac
//@param: DAC_Num: The DAC number to set the offset of
//@param: val: The value of the Dac Output
//@param spi: The spi handle to use
//@return: None
void Get_DAC_Value(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t* val){
	//Make sure DAC is between 0 and 7
	if(DAC_Num > 7){
		return;
	}
	uint8_t DAC_REG = Set_DAC0+DAC_Num;
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {READ|DAC_REG, 0, 0};
	Send_Command(spi, tx_data);
	uint8_t rx_data[3] = {0,0,0};
	Recieve_Data(spi, rx_data);
	//Convert the last 2 bytes to a 16 bit value
	*val = (rx_data[1]<<8)|rx_data[2];
	
}

//@brief: This Function will set the offset register of the the DACs 0-3 to the given value
//@param: val: The value to set the offset to
//@param spi: The spi handle to use
//@return: None
//@Note: The offset register is 16 bits wide and is in two's compliment format
//@Note: The offset registers have a default value of 0x999A in Dual Supply mode but can be tactory trimmed +/- 10 LSB
void Set_OffsetA_Reg(SPI_HandleTypeDef* spi, uint16_t val){
	uint8_t tx_data[3] = {WRITE|OFFSET_A, (val>>8)&0xFF, val&0xFF};
	Send_Command(spi, tx_data);
	
}


//@brief: This Function will set the offset register of the the DACs 4-7 to the given value
//@param: val: The value to set the offset to
//@param spi: The spi handle to use
//@return: None
//@Note: The offset register is 16 bits wide and is in two's compliment format
//@Note: The offset registers have a default value of 0x999A in Dual Supply mode but can be tactory trimmed +/- 10 LSB
void Set_OffsetB_Reg(SPI_HandleTypeDef* spi, uint16_t val){
	uint8_t tx_data[3] = {WRITE|OFFSET_B, (val>>8)&0xFF, val&0xFF};
	Send_Command(spi, tx_data);
	
}

//@brief: This Function will get the offset register of the the DACs 0-3
//@param: val: Where the Value is returned to
//@param spi: The spi handle to use
//@return: None
void Get_OffsetA_Reg(SPI_HandleTypeDef* spi, uint16_t* val){
	
	uint8_t tx_data[3] = {READ|OFFSET_A, 0, 0};
	Send_Command(spi, tx_data);
	uint8_t rx_data[3] = {0,0,0};
	Recieve_Data(spi, rx_data);
	//Convert the last 2 bytes to a signed 16 bit value
	*val = (rx_data[1]<<8)|rx_data[2];
	
}

//@brief: This Function will get the offset register of the the DACs 4-7
//@param: val: Where the Value is returned to
//@param spi: The spi handle to use
//@return: None
void Get_OffsetB_Reg(SPI_HandleTypeDef* spi, uint16_t* val){	
	uint8_t tx_data[3] = {READ|OFFSET_B, 0, 0};
	Send_Command(spi, tx_data);
	uint8_t rx_data[3] = {0,0,0};
	Recieve_Data(spi, rx_data);
	//Convert the last 2 bytes to a 16 bit value
	*val = (rx_data[1]<<8)|rx_data[2];
	
}

//@brief: This function gets the value of the offset of the given dac
//@param: DAC_Num: The DAC number to set the offset of
//@param: val: the buffer to store the of the offset
//@param spi: The spi handle to use
//@return: None
void Get_DAC_Zero(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t* val){
	//Make sure DAC is between 0 and 7
	if(DAC_Num > 7){
		return;
	}
	uint8_t DAC_REG = ZERO_0+(DAC_Num*2);
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {READ|DAC_REG, 0x00, 0x00};
	Send_Command(spi, tx_data);
	uint8_t rx_data[3] = {0,0,0};
	Recieve_Data(spi, rx_data);
	//Convert the last 2 bytes to a 16 bit value
	*val = (rx_data[1]<<8)|rx_data[2];
	
}

//@brief: This function gets the value of the gain of the given dac
//@param: DAC_Num: The DAC number to set the gain of
//@param: val: buffer to store the Value of the Gain
//@param spi: The spi handle to use
//@return: None
void Get_DAC_Gain(SPI_HandleTypeDef* spi, uint8_t DAC_Num, uint16_t* val){
	//Make sure DAC is between 0 and 7
	if(DAC_Num > 7){
		return;
	}
	uint8_t DAC_REG = GAIN_0+(DAC_Num*2);
	//Set the DAC to the correct value by adding the DAC number to the Set_DAC0 command
	uint8_t tx_data[3] = {READ|DAC_REG, 0x00, 0x00};
	Send_Command(spi, tx_data);
	uint8_t rx_data[3] = {0,0,0};
	Recieve_Data(spi, rx_data);
	//Convert the last 2 bytes to a 16 bit value
	*val = (rx_data[1]<<8)|rx_data[2];
}

//@brief: Initializes the DAC
//@param: None
//@return: None
void Setup_DAC(void){
	Set_nWakeUp_high(true);
	Set_nRST_high(true);
	Set_nCLR_high(true);
	Set_nCS_high(true);//As per documentation Should be Pulled high As soon as its powered on
}

//@brief: This function sets the value of the offset of the offset register
//@param: spi: The spi pointer
//@return: None
void Set_Offset(SPI_HandleTypeDef* spi){
	uint8_t tx_data[3] = {WRITE|OFFSET_A, 0x15, 0xF7};
	Send_Command(spi, tx_data);
	tx_data[0]=WRITE|OFFSET_B;
	Send_Command(spi, tx_data);

}

//@brief: This function Reads all of the registers in the DAC
//@param: spi: The spi handle to use
//@return: None
void Read_All_Registers(SPI_HandleTypeDef* spi){

	//Request a read from the Configuration register default value is 8000h
	uint8_t Read_Command[3] = {READ|CONFIG, 0x00, 0x00};
	
	//Set The Rx_data to be all ff so that we can tell if it changes
	uint8_t rx_data[3] = {0xff, 0xff, 0xff};
	uint8_t i=0;
	for (i=CONFIG; i<GAIN_7; i++)
	{
		Read_Command[0] = READ|i;
		//Send the Read Command
		Send_Command(spi, Read_Command);
		Recieve_Data(spi, rx_data);
		printf("Register %02x: %02x %02x %02x\n",i, rx_data[0],rx_data[1],rx_data[2]);
	}
}

//@brief: This function sets the value nWakeup pin
//@param: High: The value to set the pin to
//@return: None
void Set_nWakeUp_high(bool HIGH){
	if(HIGH){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, RESET);
	}
}


//@brief: This function returns the Value of the nWakeUp pin
//@param: None
//@return: The Value of the nWakeUp pin
bool Get_nWakeUp(){
	bool state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8);
	return state;
}

//@brief: This function sets the nLDAC pin to the given state
//@param: HIGH: The state to set the pin to
//@return: None
void Set_nLDAC_high(bool HIGH){
	if(HIGH){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, SET);
	}else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
	}
}

//@brief: This function returns the Value of the nLDAC pin
//@param: None
//@return: The Value of the nLDAC pin
bool Get_nLDAC(){
	bool state = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9);
	return state;
}

//@brief: This function sets the nCLR pin to the given state
//@param: HIGH: The state to set the pin to
//@return: None
void Set_nCLR_high(bool HIGH){
	if(HIGH){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, RESET);
	}
}

//@brief: This function returns the Value of the nCLR pin
//@param: None
//@return: The Value of the nCLR pin
bool Get_nCLR(){
	bool state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10);
	return state;
}

//@brief: This function sets the nRST pin to the given state
//@param: HIGH: The state to set the pin to
//@return: None
void Set_nRST_high(bool HIGH){
	if(HIGH){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, RESET);
	}

}

//@brief: This function returns the Value of the nRST pin
//@param: None
//@return: The Value of the nRST pin
bool Get_nRST(){
	bool state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9);
	return state;
}

//@brief: This function sets the nCS pin to the given state
//@param: HIGH: The state to set the pin to
//@return: None
void Set_nCS_high(bool HIGH){
	if(HIGH){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, SET);
	}else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, RESET);
	}

}

//@brief: This function returns the Value of the nCS pin
//@param: None
//@return: The Value of the nCS pin
bool Get_nCS(){
	bool state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
	return state;
}


//Reset the DAC Values using the nRST Pin
void Hardware_Reset(void){
	Set_nRST_high(false);
	Delay_us(10);
	Set_nRST_high(true);
	Delay_us(10);
}


//Syncronous Update of LDAC using the nLDAC Pin NOTE: Only Needed if nLDAC is tied High
void Syncronous_Update(void){
	Set_nLDAC_high(false);
	Delay_us(10);
	Set_nLDAC_high(true);
	Delay_us(10);
}

void Set_Voltage_Peak_to_Peak(float* voltage, uint16_t* values){
	//The max Voltage Peak to Peak is 22.4V so we need to scale the voltage to fit in 16 bits
	//Find the upper and lower bounds of the voltage with Zero Bias
	//Divide the voltage by the max voltage to get a percentage and cast to a 16 bit int
	float percentage = (*voltage/(float)MAX_PEAK2PEAK)/2;
	uint16_t upper_bound = 0xFFFF/2 + percentage*0xFFFF;
	uint16_t lower_bound = 0xFFFF/2 - percentage*0xFFFF;



	// uint16_t upper_bound = (uint16_t((voltage/MAX_PEAK2PEAK))*0xFFFF)+(0xFFFF/2);
	// uint16_t lower_bound = (0xFFFF/2)-((voltage/MAX_PEAK2PEAK)*0xFFFF);
	values[0] = upper_bound;
	values[1] = lower_bound;

}

