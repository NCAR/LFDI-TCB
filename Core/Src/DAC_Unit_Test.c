//Unit Test For the DAC Implementation


//Includes
#include "DAC_Unit_Test.h"


uint8_t unit_test_Default_Values(SPI_HandleTypeDef* spi, uint8_t *Register, uint8_t *Default_Value)
{
    uint8_t Data[3] = {0x00, 0x00, 0x00};
    Send_Command(spi, Register);
    Recieve_Data(spi, Data);

    //Check if the default register was written to
    
    printf("Default Register: %x, %x, %x\n",Data[0], Data[1], Data[2]);
    if((Data[1] == Default_Value[1])&&(Data[2] == Default_Value[2]))
    {
        printf("Default Register Test Passed\n");
        return 0;
    }
    else
    {
        printf("Default Register Test Failed\n");
        return 1;
    }
    
}

uint8_t unit_test_Config_Register_Default(SPI_HandleTypeDef* spi){
    uint8_t Config_Register[3] = {READ|CONFIG, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi,Config_Register, Default_Value);
}

uint8_t unit_test_ANALOG_MONITOR_SELECT_Register_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ANALOG_MONITOR_SELECT, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi,DAC_Register, Default_Value);
}

uint8_t unit_test_GPIO_Register_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GPIO_REGISTER, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0xE0, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_OFFSET_A_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|OFFSET_A, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x99, 0x9B};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}
uint8_t unit_test_OFFSET_B_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|OFFSET_B, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x99, 0x9A};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_SPI_MODE_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|SPI_MODE, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_BROADCAST_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|BROADCAST, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_0_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC0, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_1_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC1, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_2_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC2, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_3_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC3, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_4_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC4, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_5_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC5, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_6_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC6, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_DAC_7_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|Set_DAC7, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_0_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_0, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_1_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_1, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_2_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_2, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_3_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_3, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_4_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_4, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_5_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_5, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_6_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_6, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_ZERO_7_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|ZERO_7, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x00, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_0_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_0, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_1_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_1, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_2_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_2, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_3_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_3, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_4_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_4, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_5_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_5, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_6_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_6, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_GAIN_7_Default(SPI_HandleTypeDef* spi){
    uint8_t DAC_Register[3] = {READ|GAIN_7, 0x00, 0x00};
    uint8_t Default_Value[3] = {0x00, 0x80, 0x00};
    return unit_test_Default_Values(spi, DAC_Register, Default_Value);
}

uint8_t unit_test_Default_Values_All_Registers(SPI_HandleTypeDef* spi){
    uint8_t result = 0;
    result += unit_test_Config_Register_Default(spi);
    result += unit_test_ANALOG_MONITOR_SELECT_Register_Default(spi);
    result += unit_test_GPIO_Register_Default(spi);
    result += unit_test_OFFSET_A_Default(spi);
    result += unit_test_OFFSET_B_Default(spi);
    result += unit_test_BROADCAST_Default(spi);
    result += unit_test_SPI_MODE_Default(spi);
    result += unit_test_DAC_0_Default(spi);
    result += unit_test_DAC_1_Default(spi);
    result += unit_test_DAC_2_Default(spi);
    result += unit_test_DAC_3_Default(spi);
    result += unit_test_DAC_4_Default(spi);
    result += unit_test_DAC_5_Default(spi);
    result += unit_test_DAC_6_Default(spi);
    result += unit_test_DAC_7_Default(spi);
    result += unit_test_ZERO_0_Default(spi);
    result += unit_test_ZERO_1_Default(spi);
    result += unit_test_ZERO_2_Default(spi);
    result += unit_test_ZERO_3_Default(spi);
    result += unit_test_ZERO_4_Default(spi);
    result += unit_test_ZERO_5_Default(spi);
    result += unit_test_ZERO_6_Default(spi);
    result += unit_test_ZERO_7_Default(spi);
    result += unit_test_GAIN_0_Default(spi);
    result += unit_test_GAIN_1_Default(spi);
    result += unit_test_GAIN_2_Default(spi);
    result += unit_test_GAIN_3_Default(spi);
    result += unit_test_GAIN_4_Default(spi);
    result += unit_test_GAIN_5_Default(spi);
    result += unit_test_GAIN_6_Default(spi);
    result += unit_test_GAIN_7_Default(spi);
    return result;
}
