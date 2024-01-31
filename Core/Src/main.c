/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PID.h"
#include "TuningControlBoard.h"
#include "stringfifo.h"
#include "UI.h"
#include "TMP117.h"
#include "DAC.h"
#include "funcs.h"
#include "DAC_Unit_Test.h"
#include "stm32f4xx_hal.h"
#include "usbd_cdc_if.h"
#include "TemperatureCorrectionTable.h"

//#include "usbd_cdc_if.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

  
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi4;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim9;

/* USER CODE BEGIN PV */


//These are all from Damons Code----------------------------
volatile bool DoSampleTMP117 = false;
volatile bool DoCalculateOffsetCorrection = false;

// we keep our own local copy of these
// we put this here for quick interrupt access
volatile uint16_t HeaterTick[NUMOFHEATERCONTROLLERS] = {0};
volatile uint16_t HeaterSubtick[NUMOFHEATERCONTROLLERS] = {0};
volatile uint16_t HeaterTickDivider[NUMOFHEATERCONTROLLERS] = {0};
volatile uint8_t HeaterDwell[NUMOFHEATERCONTROLLERS] = {0};

volatile uint16_t Ticks_OffsetCalculation = 0;
volatile uint8_t Ticks_TMP117 = 0;
volatile uint16_t Tick_ms = 0;
volatile uint16_t ElapsedSeconds = 0;

volatile bool AutoFlood = false;
//End Damons Code-----------------------------------
struct sTuningControlBoard TCB;

struct sStringFIFO USBFIFO;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI4_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM6_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//This Interrupt is called every .25ms Will Toggle the State of the Dac Channels
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  uint16_t i;
  // Check which version of the timer triggered this callback and toggle LED
  //This is the Timer for the DAC Compensator. Should happen 4000 times a second
	if (htim == &htim2 ){
		//Synchronous Update of the DACs
		for (i = 0; i < NUMOFCOMPENSATORS; i++){
		  if(TCB.Compensator[i].Channel.enabled){
			if(TCB.Compensator[i].Channel.state_high){

			  Set_DAC_Value(&TCB.DAC8718, TCB.Compensator[i].Channel.DAC_number, TCB.Compensator[i].Channel.lower_bound);
			  TCB.Compensator[i].Channel.state_high = false;
			}else{
			  Set_DAC_Value(&TCB.DAC8718, TCB.Compensator[i].Channel.DAC_number, TCB.Compensator[i].Channel.upper_bound);
			  TCB.Compensator[i].Channel.state_high = true;
			}
		  }
		}//End For
		//This is a General Purpose Timer for Bipolar Output
		//Go Through Every Bipolar Output
		for (i = 0; i < NUMOFBipolarOutputs; i++){
		  //If the Bipolar Output is enabled
		  if(TCB.BipolarOutput[i].Enabled && TCB.BipolarOutput[i].Pulses > 0){
			//If the Timer is 0 then toggle the output
			if(TCB.BipolarOutput[i].Timer == 0){
			  if(TCB.BipolarOutput[i].Channel.state_high){
				Set_DAC_Value(&TCB.DAC8718, TCB.BipolarOutput[i].Channel.DAC_number, TCB.BipolarOutput[i].Channel.lower_bound);
				TCB.BipolarOutput[i].Channel.state_high = false;
				TCB.BipolarOutput[i].Pulses--;
			  }else{
				Set_DAC_Value(&TCB.DAC8718, TCB.BipolarOutput[i].Channel.DAC_number, TCB.BipolarOutput[i].Channel.upper_bound);
				TCB.BipolarOutput[i].Channel.state_high = true;
			  }
			  //Reload the Timer
			  BipolarOutput_TimerReload(&TCB.BipolarOutput[i]);
			//If the Timer is not 0 then decrement the timer
			}else{
			  TCB.BipolarOutput[i].Timer--;
			}
		  }//End Bipolar Output Update
		}//End For loop


		Syncronous_Update();
  }//End Timer 2





  //Timer For Heaters  
  if (htim == &htim6)
  {
    for (i=0; i<NUMOFHEATERCONTROLLERS; i++)
    {
      HeaterSubtick[i] += 1;

      if (HeaterSubtick[i] >= HeaterTickDivider[i])
      {
        HeaterTick[i] = (HeaterTick[i] + 1) % 200;
        HeaterSubtick[i] = 0;
      }
    }

    for (i=0; i<NUMOFHEATERCONTROLLERS; i++)
      if ((HeaterTick[i] > HeaterDwell[i])
        && (HeaterTick[i] < (200 - HeaterDwell[i])))
        HeaterController_SetHeater(i, true);
      else
        HeaterController_SetHeater(i, false);


  }//End Timer 3


  //Clock Tick for Sampling TMP117 
  if (htim == &htim4)
  {
    Tick_ms = (Tick_ms + 1) % 1000;
    // this should be after the ClockTick increment
    if (Tick_ms == 0)
    {
      ElapsedSeconds++;
      Ticks_OffsetCalculation++;
    }

    if (++Ticks_TMP117 >= 130)
    {
      Ticks_TMP117 = 0;
      DoSampleTMP117 = true;
    }

    if (Ticks_OffsetCalculation > 60)
    {
      Ticks_OffsetCalculation = 0;
      DoCalculateOffsetCorrection = true;
    }

  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char buffer[50];
  int i;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  StringFIFOInit(&USBFIFO);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  InitDWTTimer(); // we need this for delay_us

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI4_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_I2C2_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
  TCB_InitStruct(&TCB, &hi2c1, &hi2c2, &hspi4);
  for (i = 0; i < 6; i++){
    TCB.DAC8718.DAC_Channels[i].enabled = true;
    TCB.DAC8718.DAC_Channels[i].DAC_number = i;
    TCB.DAC8718.DAC_Channels[i].lower_bound = 0x7FFF;
    TCB.DAC8718.DAC_Channels[i].upper_bound = 0x7FFF;
    TCB.DAC8718.DAC_Channels[i].state_high = false;
  }
  HAL_Delay(500);
  printf("-- REBOOT --\n");

 // HAL_TIM_Base_Start_IT(&htim2);
  
  for(i = 0; i< NUMOFHEATERCONTROLLERS; i++)
      HeaterController_SetHeater(i, true);
  for(i = 0; i< NUMOFHEATERCONTROLLERS; i++)
      HeaterController_SetHeater(i, false);


  for(i = 0; i< NUMOFHEATERCONTROLLERS; i++){
    TMP117_Configure(&TCB.HeaterControllers[i].Sensor);
  }
  for(i = 0; i< NUMOFCOMPENSATORS; i++){
	  TMP117_Configure(&TCB.Compensator[i].Sensor);
  }

  HAL_TIM_Base_Start_IT(&htim2); //DAC Timer
  HAL_TIM_Base_Start_IT(&htim6); // Heater Timer
  HAL_TIM_Base_Start_IT(&htim4); // Main Timer


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // float voltage = 0;
  // float voltage2 = 0;

  //SUPER LOOP!!!!
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  uint16_t voltage;
//	  uint16_t voltage2;
    //Cycle through the DAC channels and set them to the opposite state and increase and decrease the voltage
//     for (voltage = 0; voltage < TCB.DAC8718.max_peak2peak; voltage += 0.1){
//		   for(uint8_t j = 0; j < 3; j++){
//		 	  Set_Voltage_Peak_to_Peak(&TCB.DAC8718.DAC_Channels[j], &voltage);
//		   }
//		   voltage2 = TCB.DAC8718.max_peak2peak - voltage;
//		   for(uint8_t j = 3; j < 6; j++){
//		 	  Set_Voltage_Peak_to_Peak(&TCB.DAC8718.DAC_Channels[j], &voltage2);
//		   }
//		   //HAL_Delay(100);
//	   }



    // we keep a global copy of this for the timer interrupt
    for (i=0; i<NUMOFHEATERCONTROLLERS; i++)
      HeaterTickDivider[i] = MAX(1, TCB.HeaterControllers[i].PwmPeriod_ms / 20);


    //Set the heater to the opposite state its currently in
	  //Just to Test. Here is the




		Compensator_Update(&TCB.Compensator[0]);
		Compensator_Update(&TCB.Compensator[1]);
		Compensator_Update(&TCB.Compensator[2]);
		Compensator_Update(&TCB.Compensator[3]);
		Compensator_Update(&TCB.Compensator[4]);
		Compensator_Update(&TCB.Compensator[5]);


		//This is set by an interrupt timer. When this is true the system will go through and take a sample
		if (DoSampleTMP117){
			//If there are a ton of error restart the I2C bus to see if that helps
			DoSampleTMP117 = false;
			//For each of the compensators
			for(uint8_t i = 0; i<NUMOFCOMPENSATORS;i++){
				//If there are errors Restart the Buses
				if (TCB.Compensator[i].Sensor.Errors > 10){
					MX_I2C1_Init();
					MX_I2C2_Init();
				}
				if (TCB.Compensator[i].Sensor.Configured){
					TMP117_GetTemperature(&TCB.Compensator[i].Sensor);
				}else{
					TMP117_Configure(&TCB.Compensator[i].Sensor);
				}
			}//End Compensator For loop
			//For all of the HeaterControllers
			for (int i = 0; i < NUMOFHEATERCONTROLLERS; i++){
				if (TCB.HeaterControllers[i].Sensor.Errors > 10){
					MX_I2C1_Init();
					MX_I2C2_Init();
				}
				if (TCB.HeaterControllers[i].Sensor.Configured){
					TMP117_GetTemperature(&TCB.HeaterControllers[i].Sensor);
				}else{
					TMP117_Configure(&TCB.HeaterControllers[i].Sensor);
				}
        HeaterController_Step(&TCB.HeaterControllers[i]);
			}//End Controller For loop
		  }//End do Sample


    if (DoCalculateOffsetCorrection)
    {
      DoCalculateOffsetCorrection = false;
      for (i=0; i<4; i++)
        if (TCB.HeaterControllers[i].HeaterEnabled)
          PID_PerformOffsetCorrection(&TCB.HeaterControllers[i].PID);
    }

		if (StringFIFORemove(&USBFIFO, buffer) == 0)
		{
		  ProcessUserInput(&TCB, buffer);
		}

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 420;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 4200;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1680;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 100;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 420;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 50;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|Heater_Enable_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Heater_Enable_1_Pin|DAC_nWake_Pin|DAC_nLDAC_Pin|DAC_nCS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Heater_Enable_3_Pin|DAC_nRST_Pin|DAC_nCLR_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(nLDAC_GPIO_Port, nLDAC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Heater_Enable_1_Pin DAC_nWake_Pin DAC_nLDAC_Pin DAC_nCS_Pin */
  GPIO_InitStruct.Pin = Heater_Enable_1_Pin|DAC_nWake_Pin|DAC_nLDAC_Pin|DAC_nCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Heater_Enable_2_Pin Heater_Enable_3_Pin DAC_nRST_Pin DAC_nCLR_Pin */
  GPIO_InitStruct.Pin = Heater_Enable_2_Pin|Heater_Enable_3_Pin|DAC_nRST_Pin|DAC_nCLR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : nLDAC_Pin */
  GPIO_InitStruct.Pin = nLDAC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(nLDAC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 PD12 PD13
                           PD14 PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* This allows printf to write to the debugger and USB */
/* USB needs #include "usbd_cdc_if.h" */
/* ITM Stimulus port 0 must be enabled in your trace settings */
int _write(int file, char *ptr, int len)
{
  int i, res;

  for (i=0;i<20; i++)
  {
    res = CDC_Transmit_FS((uint8_t*) ptr, len);  // USB out
    if (res == USBD_OK)
      break;
  }

  for (int i = 0; i < len; i++)
    ITM_SendChar((*ptr++));              // debugger out
  return len;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
