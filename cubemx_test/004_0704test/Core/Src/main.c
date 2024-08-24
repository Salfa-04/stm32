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
#include "gpio.h"
#include "rtc.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_uart.h"
#include "tim.h"
#include "usart.h"
#include <stdint.h>

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
#define SENT(data, len) HAL_UART_Transmit(&huart1, (uint8_t *)data, len, 1000)

extern int sprintf(char *, const char *, ...);
extern unsigned int strlen(const char *);
extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);

RTC_TimeTypeDef time = {0, 0, 0}, timer[5];

/// current_timer: 当前定时器序号，从 0 到 4
///
/// smh_position: 当前设置模式，0H, 1M, 2S
///
/// states_code:
///   定时器状态，0 待命Standby，1 设置Setting，2 运行Running，3 暂停Pause
uint8_t current_timer = 0, smh_position = 0, states_code[5] = {0};

// 0H, 1M, 2S
uint8_t line1[8] = "\n\n\nNo 0\n", line2[32];
uint8_t states[][15] = {
    "       Standby\n",
    "       Setting\n",
    "       Running\n",
    "       Pause\n",
};

uint8_t rxBuffer[5], tempbuffer[32];

// union data {
//   float value;
//   uint8_t bytes[4];
// } data = {0.0f};

void debug_display() {
  char temp[64];
  sprintf(temp, "current_timer: %d, smh_position: %d, states_code: %d\n",
          current_timer, smh_position, states_code[current_timer]);
  SENT(temp, strlen(temp));

  // SENT(data.bytes, sizeof(data.bytes)); // 打印乱码
}

void update_display() {

  char temp = ' ';

  if (states_code[current_timer] == 1) {
    if (smh_position == 0)
      temp = 'H';
    if (smh_position == 1)
      temp = 'M';
    if (smh_position == 2)
      temp = 'S';
  }

  line1[6] = current_timer + '1';
  SENT(line1, sizeof(line1));
  sprintf((char *)line2, "  %c  %02d : %02d : %02d\n", temp, time.Hours,
          time.Minutes, time.Seconds);
  SENT(line2, strlen((char *)line2));
  SENT(states[states_code[current_timer]], sizeof(*states));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    if (rxBuffer[0] == 0xAA) {
      if (rxBuffer[1] == 0x01 && states_code[current_timer] == 2) { // 修改时间
        time.Hours = rxBuffer[2];
        time.Minutes = rxBuffer[3];
        time.Seconds = rxBuffer[4];

        memcpy(timer + current_timer, &time, sizeof(time));
        update_display();
      }
    }
  }

  HAL_UART_Receive_IT(huart, rxBuffer, sizeof(rxBuffer));
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc) {
  if (hrtc->Instance == RTC) { // 秒中断
    if (states_code[current_timer] == 2) {
      if (time.Seconds > 0 || time.Minutes > 0 || time.Hours > 0) {
        if (time.Seconds > 0) {
          time.Seconds--;
        } else {
          if (time.Minutes > 0) {
            time.Minutes--;
            time.Seconds = 59;
          } else {
            if (time.Hours > 0) {
              time.Hours--;
              time.Minutes = 59;
              time.Seconds = 59;
            }
          }
        }
      } else {
        states_code[current_timer] = 0;
        HAL_GPIO_WritePin(GPIOA, LD1_Pin, GPIO_PIN_SET);
      }
    }

    // data.value += 0.01;

    update_display();
  }
}

void time_plus_one() {
  switch (smh_position) {
  case 0:
    time.Hours++;
    break;
  case 1:
    time.Minutes++;
    break;
  case 2:
    time.Seconds++;
    break;
  }

  if (time.Hours > 23)
    time.Hours = 0;
  if (time.Minutes > 59)
    time.Minutes = 0;
  if (time.Seconds > 59)
    time.Seconds = 0;
}

void scan_key() {
  if (!HAL_GPIO_ReadPin(GPIOB, B1_Pin)) {
    HAL_Delay(100);
    if (!HAL_GPIO_ReadPin(GPIOB, B1_Pin) &&
        (states_code[current_timer] == 0 || // can be used in standby mode
         states_code[current_timer] == 3)   // and pause mode
    ) {
      if (current_timer < 4)
        current_timer++;
      else
        current_timer = 0;

      memcpy(&time, timer + current_timer, sizeof(time));
    }

    while (!HAL_GPIO_ReadPin(GPIOB, B1_Pin))
      ; // 防抖

    update_display();
  }

  if (!HAL_GPIO_ReadPin(GPIOB, B2_Pin)) {
    HAL_Delay(100);
    if (!HAL_GPIO_ReadPin(GPIOB, B2_Pin) &&
        states_code[current_timer] == 1 // 确保已在 Setting 模式
    ) {
      if (smh_position < 2)
        smh_position++;
      else
        smh_position = 0;
    } else {
      // 否则进入 Setting 状态
      memcpy(&time, timer + current_timer, sizeof(time));
      states_code[current_timer] = 1;
      HAL_GPIO_WritePin(GPIOA, LD1_Pin, GPIO_PIN_RESET);
    }

    uint32_t tick = HAL_GetTick();
    while (!HAL_GPIO_ReadPin(GPIOB, B2_Pin) && (HAL_GetTick() - tick < 700))
      ;
    if (HAL_GetTick() - tick >= 700) { // 长按
      if (states_code[current_timer] == 1) {
        // 保存并退出 Setting 状态
        memcpy(timer + current_timer, &time, sizeof(time));
        states_code[current_timer] = 0;
      } else {
        // not in setting mode
        // 小问题，不用管
      }
    }

    while (!HAL_GPIO_ReadPin(GPIOB, B2_Pin))
      ; // 防抖

    update_display();
  }

  if (!HAL_GPIO_ReadPin(GPIOB, B3_Pin)) {
    HAL_Delay(100);
    if (states_code[current_timer] == 1) {
      if (!HAL_GPIO_ReadPin(GPIOB, B3_Pin))
        time_plus_one();

      uint32_t tick = HAL_GetTick();
      while (!HAL_GPIO_ReadPin(GPIOB, B3_Pin) && (HAL_GetTick() - tick < 700))
        ; // 检测长按
      if (HAL_GetTick() - tick >= 700) {
        while (!HAL_GPIO_ReadPin(GPIOB, B3_Pin)) {
          time_plus_one();

          HAL_Delay(100);
        }
      }
    } else {
      // if not in setting mode
      // It's a small matter, not important.
    }

    update_display();
  }

  if (!HAL_GPIO_ReadPin(GPIOB, B4_Pin)) { // B4: 启动键
    HAL_Delay(100);
    if (!HAL_GPIO_ReadPin(GPIOB, B4_Pin)) { // 短按
      // todo: 切换定时器状态: All(*) -> Running(2) -> Pause(3)
      smh_position = 0;

      if (states_code[current_timer] == 1) {        // 临时启动
        states_code[current_timer] = 2;             // 直接开润
      } else if (states_code[current_timer] != 2) { // not in running state
        memcpy(&time, timer + current_timer, sizeof(time));
        states_code[current_timer] = 2;
      } else { // 如果运行了(running state): 切换到暂停(pause state)
        states_code[current_timer] = 3;
        memcpy(timer + current_timer, &time, sizeof(time));
      }
    }

    uint32_t tick = HAL_GetTick();
    while (!HAL_GPIO_ReadPin(GPIOB, B4_Pin) && (HAL_GetTick() - tick < 700))
      ;
    if (HAL_GetTick() - tick >= 700) { // 长按
      // 定时器复位，回到 Standby 状态
      states_code[current_timer] = 0;
      smh_position = 0;
      memset(&time, 0, sizeof(time));
      memset(&timer[current_timer], 0, sizeof(timer[current_timer]));
    }

    while (!HAL_GPIO_ReadPin(GPIOB, B4_Pin))
      ;

    update_display();
  }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU
   * Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the
   * Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  SENT("Hello\n", 6);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_UART_Receive_IT(&huart1, rxBuffer, sizeof(rxBuffer));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    HAL_Delay(100);
    // debug_display();
    scan_key();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return
   * state
   */
  __disable_irq();
  while (1) {
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
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
     file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
