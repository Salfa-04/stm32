/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "oled.h"

uint32_t adc_buff;
uint8_t oled_buff[2], rx_buff[1];

uint8_t hello_buff[] = "Welcome, System Starting...";

uint8_t digit[][16] = {
    // 4
    {0x90, 0x52, 0x34, 0x10, 0xFF, 0x10, 0x34, 0x52, 0x80, 0x70, 0x8F, 0x08, 0x08, 0xF8, 0x08, 0x00},
    {0x82, 0x9A, 0x56, 0x63, 0x22, 0x52, 0x8E, 0x00, 0x80, 0x40, 0x33, 0x0C, 0x33, 0x40, 0x80, 0x00}, /*"数",0*/

    {0x10, 0x0C, 0x04, 0x24, 0x24, 0x24, 0x25, 0x26, 0xA4, 0x64, 0x24, 0x04, 0x04, 0x14, 0x0C, 0x00},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x82, 0x7F, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00}, /*"字",1*/

    {0x20, 0x20, 0x20, 0xBE, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBE, 0x20, 0x20, 0x20, 0x00},
    {0x00, 0x80, 0x80, 0xAF, 0xAA, 0xAA, 0xAA, 0xFF, 0xAA, 0xAA, 0xAA, 0xAF, 0x80, 0x80, 0x00, 0x00}, /*"量",2*/

    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"：",3*/
};

uint8_t welcome[][16] = {
    // 6
    {0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x20, 0x40, 0x80, 0x00, 0x00},
    {0x08, 0x04, 0x03, 0x00, 0x00, 0x40, 0x80, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0E, 0x00}, /*"小",0*/

    {0x04, 0x84, 0xE4, 0x5C, 0x44, 0xC4, 0x00, 0xC0, 0x38, 0x00, 0xFF, 0x00, 0x08, 0x10, 0x60, 0x00},
    {0x02, 0x01, 0x7F, 0x10, 0x10, 0x3F, 0x81, 0x80, 0x40, 0x40, 0x27, 0x10, 0x0C, 0x03, 0x00, 0x00}, /*"砂",1*/

    {0x04, 0x24, 0x44, 0x84, 0x64, 0x9C, 0x40, 0x30, 0x0F, 0xC8, 0x08, 0x08, 0x28, 0x18, 0x00, 0x00},
    {0x10, 0x08, 0x06, 0x01, 0x82, 0x4C, 0x20, 0x18, 0x06, 0x01, 0x06, 0x18, 0x20, 0x40, 0x80, 0x00}, /*"欢",2*/

    {0x40, 0x40, 0x42, 0xCC, 0x00, 0x00, 0xFC, 0x04, 0x02, 0x00, 0xFC, 0x04, 0x04, 0xFC, 0x00, 0x00},
    {0x00, 0x40, 0x20, 0x1F, 0x20, 0x40, 0x4F, 0x44, 0x42, 0x40, 0x7F, 0x42, 0x44, 0x43, 0x40, 0x00}, /*"迎",3*/

    {0x20, 0x10, 0x08, 0xFC, 0x23, 0x10, 0x88, 0x67, 0x04, 0xF4, 0x04, 0x24, 0x54, 0x8C, 0x00, 0x00},
    {0x40, 0x30, 0x00, 0x77, 0x80, 0x81, 0x88, 0xB2, 0x84, 0x83, 0x80, 0xE0, 0x00, 0x11, 0x60, 0x00}, /*"您",4*/

    {0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"！",5*/
};

void LED_ON(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
  OLED_ShowString(40, 4, " ON", 16);
}

void LED_OFF(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
  OLED_ShowString(40, 4, "OFF", 16);
}

void HUM_ON(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  OLED_ShowString(56, 6, " ON", 16);
}

void HUM_OFF(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  OLED_ShowString(56, 6, "OFF", 16);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART1)
  {
    if (*rx_buff == 0x00)
      LED_ON();
    else if (*rx_buff == 0x01)
      LED_OFF();
    else if (*rx_buff == 0x02)
      HUM_ON();
    else if (*rx_buff == 0x03)
      HUM_OFF();
    else
      HAL_UART_Transmit(&huart1, "未知命令！", 16, 1000);
  }

  HAL_UART_Transmit(&huart1, "执行完毕\r\n", 15, 1000);
  HAL_UART_Receive_IT(&huart1, rx_buff, sizeof(rx_buff));
}
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
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  OLED_Init();
  OLED_Display_On();
  OLED_Clear();

  HUM_ON();
  LED_ON();
  OLED_ShowString(0, 0, hello_buff, 16);
  HAL_Delay(700);
  OLED_Clear();
  HUM_OFF();
  LED_OFF();
  HAL_Delay(300);

  OLED_ShowHzkS(0, 0, welcome, 6);
  OLED_ShowHzkS(0, 2, digit, 4);
  OLED_ShowString(0, 4, "LED:", 16);
  OLED_ShowString(0, 6, "HUMMER:", 16);
  OLED_ShowString(64, 2, "None", 16);

  HAL_ADC_Start(&hadc1);
  HAL_UART_Receive_IT(&huart1, rx_buff, sizeof(rx_buff));
  HAL_UART_Transmit(&huart1, hello_buff, sizeof(hello_buff), 1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
    {
      if ((adc_buff = HAL_ADC_GetValue(&hadc1)) > 2000)
      {
        LED_ON();
        HUM_ON();
      }
      else
      {
        HUM_OFF();
        LED_OFF();
      }
      sprintf((char *)oled_buff, "%d", adc_buff);
      OLED_ShowString(64, 2, oled_buff, 16);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

#ifdef USE_FULL_ASSERT
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
