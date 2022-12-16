/*
  @
  @   Date               :        30.11.2018 / Sunday
  @
  @   Contact            :        Writing by Muhammet Rasit KIYAK                    @https://www.linkedin.com/in/mrstkyk/
  @                               mrstkyk@gmail.com
  @
  @   Description        :        This Library for TMP117/TMP116 Temperature Sensor on Texas Instruments
  @                               Dependency library is HAL for STM32 series (__STM32xx_HAL_I2C_H)
  @
*/

#include "Temp_Sensor.h"




//Read the Temperature fromn the TMP117 Temperature Register and convert it to a float
float TMP117_get_Temp(I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      uint8_t data[2];
      uint16_t temp;
      float temperature;
      HAL_I2C_Master_Transmit(&i2c, TMP117_DeviceID, TempReg, 1, 100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c, TMP117_DeviceID, data, 2, 100);
      temp = ((uint16_t)data[0] << 8) | data[1];
      
      //Convert the unsigned temp to a signed temp
      if (temp & 0x8000)
      {
            temp = ~temp + 1;
            temperature = (float)temp * 0.0078125;
            temperature = -temperature;
      }
      else{temperature = (float)temp * 0.0078125;}

      return (float)temperature;
}

/*
   @Brief         Get Configuration
   @Description   Get Configuration Register Value
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_Configuration(I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[2];
      buf[0]=ConfigReg;
      //Read the Configuration Register
      HAL_I2C_Master_Transmit(&i2c, TMP117_DeviceID|0x01, buf, 1, 100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c, TMP117_DeviceID, buf, 2, 100);
      return ((uint16_t)buf[0] << 8) | buf[1];
      
}

/*
   @Brief         Set Configuration
   @Description   Set Configuration Register for Features
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_Configuration(I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID, uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=ConfigReg;
      buf[1]=first;
      buf[2]=second;
      //Set the Configuration Register
      HAL_I2C_Master_Transmit(&i2c, TMP117_DeviceID, buf, 3, 100);
      HAL_Delay(1);
}

/*
   @Brief         Set HighLimit
   @Description   Set HighLimit for Alert
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_HighLimit(I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID, uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=TempHighReg;
      buf[1]=first;
      buf[2]=second;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);

}

/*
   @Brief         Get Highlimit
   @Description   Get Highlimit Register Value
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_HighLimit         (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=TempHighReg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief         Set LowLimit
   @Description   Set LowLimit for Alert
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_LowLimit                 (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID, uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=TempLowReg;
      buf[1]=first;     //Reset Value
      buf[2]=second;    //Reset Value

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);

}

/*
   @Brief         Get LowLimit
   @Description   Get Lowlimit Register Value
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_LowLimit          (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=TempLowReg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief         Get EEPROM Unlock Register Value
   @Description   Check EEPROM for Unlock/Lock
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_EEPROM_Unclock    (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=EEPROMULReg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief         Set EEPROM Unlock Register Value
   @Description   Active/Inactive for EEPROM read/write situation
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM_Unlock            (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID,uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=EEPROMULReg;
      buf[1]=first;
      buf[2]=second;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);

}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM1                  (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID,uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=EEPROM1Reg;
      buf[1]=first;     //Reset Value
      buf[2]=second;    //Reset Value

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_EEPROM1           (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=EEPROM1Reg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM2                  (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID,uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=EEPROM2Reg;
      buf[1]=first;
      buf[2]=second;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_EEPROM2           (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=EEPROM2Reg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);
}

/*
   @Brief
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_EEPROM3                  (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID, uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=EEPROM3Reg;
      buf[1]=first;
      buf[2]=second;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);
}

/*
   @Brief         Get EEPROM3 Value
   @Description
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_EEPROM3           (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=EEPROM3Reg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief         Set Temperature Offset Value
   @Description   Set Temperature Offset Value for Calibrating
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
                  uint8_t first     ->  [15:8]
                  uint8_t second    ->  [7:0]
   @Return value  void
 */
void TMP117_set_Temperature_Offset       (I2C_HandleTypeDef i2c,uint8_t TMP117_DeviceID,uint8_t first,uint8_t second)
{
      static uint8_t buf[3];
      buf[0]=TempOffset;
      buf[1]=first;
      buf[2]=second;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID,buf,2,100);
      HAL_Delay(1);

}

/*
   @Brief         Get Temperature Offset Value
   @Description   Get Temperature Offset Value for Calibrating
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_Temperature_Offset(I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=TempOffset;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief         Get ID Register
   @Description   Check Device ID for Error Handler
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  uint16_t
 */
uint16_t TMP117_get_ID_Register       (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      static uint8_t buf[3];
      buf[0]=IDReg;

      HAL_I2C_Master_Transmit(&i2c,TMP117_DeviceID|0x01,buf,0,100);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,TMP117_DeviceID,buf,1,100);
      return ((buf[0]<<8)|buf[1]);

}

/*
   @Brief         Custom Initialization
   @Description   Custom Parameters for Sensor
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  void
 */
void TMP117_Initialization               (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
	TMP117_set_Configuration(i2c,TMP117_DeviceID, 0x00, 0x02);
      //TMP117_set_Configuration(i2c,0x02,0x20);
      //TMP117_set_Temperature_Offset(i2c,0x00,0x00); //Default Value
      //TMP117_set_LowLimit(i2c,0x12,0x80);           //Set 10 Celcius
      //TMP117_set_HighLimit(i2c,0x51,0x20);          //Set 40 Celcius
}

/*
   @Brief         Default Initialization
   @Description   Default Parameters for Sensor
   @Parameter     I2C_HandleTypeDef ->  HAL_I2C Handle
   @Return value  void
 */
void TMP117_Initialization_DEFAULT       (I2C_HandleTypeDef i2c, uint8_t TMP117_DeviceID)
{
      TMP117_set_Configuration(i2c,TMP117_DeviceID,0x02,0x20);
      TMP117_set_Temperature_Offset(i2c,TMP117_DeviceID,0x00,0x00);
      TMP117_set_LowLimit(i2c,TMP117_DeviceID,0x80,0x00);
      TMP117_set_HighLimit(i2c,TMP117_DeviceID,0x60,0x00);
      TMP117_set_EEPROM_Unlock(i2c,TMP117_DeviceID,0x00,0x00);
}
