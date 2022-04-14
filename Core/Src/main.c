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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HD44780.h"
#include "button.h"
#include "menu.h"
#include <string.h>
#include "hmc5883l.h"
#include "DHT11_sensor.h"
#include "GPS.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
buttonenter_t Enter;
buttonnext_t Next;
buttonprev_t Prev;
buttonback_t Back;

HMC5883L_t Hmc5883l;
DHT_DataTypedef DHT11_Data;
UART_HandleTypeDef huart1;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t PWM = 43;
uint8_t  i ;
float k ;
uint8_t Message[64];
uint8_t MessageLength;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim3);
  	LCD_Init();
  	menu_refresh();
  	ButtonEnterInitKey(&Enter, key_enter_GPIO_Port, key_enter_Pin, 15);
  	ButtonEnterPressCallback(&Enter,key_enter_press);

  	ButtonNextInitKey(&Next, GPIOC, key_next_Pin, 20);
  	ButtonNextPressCallback(&Next,key_next_press);

  	ButtonPrevInitKey(&Prev, GPIOC, key_prev_Pin, 20);
  	ButtonPrevPressCallback(&Prev,key_prev_press);

  	ButtonBackInitKey(&Back, GPIOC, key_enterC10_Pin, 20);
  	ButtonBackPressCallback(&Back,key_back_press);
  	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1, PWM);  // od  1 do 35
  	HMC5883L_Init(&Hmc5883l, &hi2c1, HMC5883L_ADDRESS);
  	NEO6_Init(&GpsState, &huart1);
  	uint32_t Timer = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  /*
	 	  LCD_Cls();
	 	  	  LCD_Locate(7,0);
	 		if (HAL_GPIO_ReadPin(GPIOC, key_enterC10_Pin) == 0) {
	 			if (i >= 4 && i <= 13) {
	 				LCD_Locate(i, 1);
	 				//LCD_String("BBBB ");
	 				LCD_Char(0xff);
	 				i++;
	 			}
	 		}
	 */
	 	  NEO6_Task(&GpsState);

	 	 	  	  if((HAL_GetTick() - Timer) > 1000)
	 	 	  	  {
	 	 	  		  MessageLength = sprintf((char*)Message, "\033[2J\033[;H"); // Clear terminal and home cursor
	 	 	  		  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  		  if(NEO6_IsFix(&GpsState))
	 	 	  		  {
	 	 	  			  MessageLength = sprintf((char*)Message, "UTC Time: %02d:%02d:%02d\n\r", GpsState.Hour, GpsState.Minute, GpsState.Second);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Date: %02d.%02d.20%02d\n\r", GpsState.Day, GpsState.Month, GpsState.Year);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Latitude: %.4f %c\n\r", GpsState.Latitude, GpsState.LatitudeDirection);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Longitude: %.2f %c\n\r", GpsState.Longitude, GpsState.LongitudeDirection);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Altitude: %.2f m above sea level\n\r", GpsState.Altitude);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Speed: %.2f knots, %f km/h\n\r", GpsState.SpeedKnots, GpsState.SpeedKilometers);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Satelites: %d\n\r", GpsState.SatelitesNumber);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Dilution of precision: %.2f\n\r", GpsState.Dop);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Horizontal dilution of precision: %.2f\n\r", GpsState.Hdop);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

	 	 	  			  MessageLength = sprintf((char*)Message, "Vertical dilution of precision: %.2f\n\r", GpsState.Vdop);
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
	 	 	  		  }
	 	 	  		  else
	 	 	  		  {
	 	 	  			  MessageLength = sprintf((char*)Message, "No Fix\n\r");
	 	 	  			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
	 	 	  		  }

	 	 	  		  Timer = HAL_GetTick();
	 	 	  	  }



	 	  ButtonEnterTask(&Enter);
	 	  ButtonNextTask(&Next);
	 	  ButtonPrevTask(&Prev);
	 	  ButtonBackTask(&Back);

	 /*
	 		key_next_press();
	 		key_prev_press();
	 		key_enter_press();
	 		key_back_press();
	 		*/
	 	  	  // Measurement end
	 	  	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	 	  	  HAL_Delay(100);
	   }
  }
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == GpsState.neo6_huart)
	{
		NEO6_ReceiveUartChar(&GpsState);
	}
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

