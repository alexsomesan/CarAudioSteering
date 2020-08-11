/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  /* USER CODE END Header */

  /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <config.h>
#include "memory.h"
#include "math.h"
#include "SEGGER_RTT.h"
#include "ad5272.h"
#include "statemachine.h"
#include "commands.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FL_AVG_READY  0x00000001
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint16_t *readouts;

volatile uint32_t average = 0;
volatile uint32_t intFlags = 0;


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
    readouts = malloc(sizeof(uint16_t)*BUT_BUF_LEN);
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
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_ADC_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */

    HAL_ADCEx_Calibration_Start(&hadc);

    InitDigipot();
    InitCommands();

    HAL_ADC_Start_DMA(&hadc, (uint32_t*)readouts, BUT_BUF_LEN);

    uint32_t avg = 4096;
    char avgstr[10];

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // wait for DMA interrupt
        while (0 == (intFlags & FL_AVG_READY));

        avg = average;
        intFlags ^= FL_AVG_READY;

        sprintf(avgstr, "%ld\r\n", avg);
        SEGGER_RTT_TerminalOut(1, avgstr);

        ProcessInput(avg);

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

    }
    free(readouts);
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct ={ 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct ={ 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit ={ 0 };

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
    RCC_OscInitStruct.HSI14CalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* myadc) {
    for (int i = BUT_BUF_LEN / 2 ; i < BUT_BUF_LEN; i++) {
        average += readouts[i];
    }
    average /= BUT_BUF_LEN / 2;
#if VARIO_FILTER
    uint32_t maxVar = 0;
    for (int i = BUT_BUF_LEN / 2 ; i < BUT_BUF_LEN; i++) {
        uint32_t diff = abs((uint32_t)readouts[i] - average);
        if (diff > maxVar) {
            maxVar = diff;
        }
    }
    char maxstr[10];
    SEGGER_RTT_TerminalOut(2, strcat(itoa(maxVar, maxstr, 10), "\r\n"));
    if (maxVar > MAX_VARIATION) 
        return;
#endif
    intFlags |= FL_AVG_READY;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* myadc) {
    for (int i = 0; i < BUT_BUF_LEN / 2; i++) {
        average += readouts[i];
    }
    average /= BUT_BUF_LEN / 2;
#if VARIO_FILTER
    uint32_t maxVar = 0;
    for (int i = 0; i < BUT_BUF_LEN / 2; i++) {
        uint32_t diff = abs((uint32_t)readouts[i] - average);
        if (diff > maxVar) {
            maxVar = diff;
        }
    }
    char maxstr[10];
    SEGGER_RTT_TerminalOut(2, strcat(itoa(maxVar, maxstr, 10), "\r\n"));
    if (maxVar > MAX_VARIATION) 
        return;
#endif
    intFlags |= FL_AVG_READY;
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
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
