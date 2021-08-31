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
#include "ad5272.h"
#include "statemachine.h"
#include "commands.h"

#ifdef FEAT_FLASH_STORAGE
#include "flashstorage.h"
#endif

#ifdef DEBUG
#include "stdio.h"
#include "SEGGER_RTT.h"
#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FL_AVG_READY 0x00000001
#define FL_URX_READY 0x00000002
#define FL_UTX_READY 0x00000004

#define FL_FWD_ADC 0x00000001
#define FL_HANDSHK 0x00000002

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint16_t *readouts;
static uint8_t *rxBuff;
static uint8_t *txBuff;

volatile uint32_t average = 0;
volatile uint32_t intFlags = 0;
volatile uint32_t stateFlags = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t scanI2C();
extern uint8_t AD5272Addr;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  readouts = malloc(sizeof(uint16_t) * BUT_BUF_LEN);
  rxBuff = malloc(sizeof(uint8_t) * RX_BUF_LEN);
  txBuff = malloc(sizeof(uint8_t) * TX_BUF_LEN);

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
  // AD5272Addr = scanI2C() << 1;
  HAL_ADCEx_Calibration_Start(&hadc);

  // InitDigipot();
#ifdef DEBUG
  InitCommandNames();
  char avgstr[40];
  sprintf(avgstr, "I2C: %0X\r\n", AD5272Addr);
  SEGGER_RTT_TerminalOut(2, avgstr);
#endif

  HAL_ADC_Start_DMA(&hadc, (uint32_t *)readouts, BUT_BUF_LEN);
  HAL_UART_Receive_DMA(&huart1, rxBuff, RX_BUF_LEN);

  uint32_t avg = 4096;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */ 
  while (1)
  {
    // wait for DMA interrupt
    while (0 == (intFlags & FL_AVG_READY))
    {
      if (intFlags & FL_URX_READY)
      {
#ifdef DEBUG
        sprintf(avgstr, "UART: %0X%0X%0X%0 X\r\n", rxBuff[0], rxBuff[1], rxBuff[2], rxBuff[3]);
        SEGGER_RTT_TerminalOut(2, avgstr);
#endif
        switch (rxBuff[0])
        {
        case 0x01: // Manual pot output
          if (1 == rxBuff[1])
          {
            HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
          }
          else
          {
            HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_RESET);
          }
          uint16_t pv = ((uint16_t)rxBuff[2]) << 8 | (uint16_t)rxBuff[3];
          AD5272_command_write(&hi2c1, AD5272Addr, AD5272_RDAC_WRITE, pv);
          if (0x3FF == pv)
          {
            HAL_GPIO_WritePin(TIP_SW_GPIO_Port, TIP_SW_Pin, GPIO_PIN_RESET);
          }
          else
          {
            HAL_GPIO_WritePin(TIP_SW_GPIO_Port, TIP_SW_Pin, GPIO_PIN_SET);
          }
          break;
        case 0x02: // Monitor ADC
          if (rxBuff[1] == 0)
          {
            stateFlags &= ~FL_FWD_ADC;
          }
          else
          {
            stateFlags |= FL_FWD_ADC;
            intFlags |= FL_UTX_READY;
          }
          break;
        case 0xFA:
          if (rxBuff[1] == 0xFB && rxBuff[2] == 0xFC && rxBuff[3] == 0xFD){
            intFlags |= FL_UTX_READY;
#ifdef DEBUG
          SEGGER_RTT_TerminalOut(2, "Got handshake request\r\n");
#endif
          txBuff[0] = 'e';
          txBuff[1] = 'l';
          txBuff[2] = 'l';
          txBuff[3] = 'o';
          intFlags ^= FL_UTX_READY;
          HAL_UART_Transmit_DMA(&huart1, txBuff, TX_BUF_LEN); // Acknowledge connection
          stateFlags |= FL_HANDSHK;
          intFlags |= FL_UTX_READY;
#ifdef DEBUG
          SEGGER_RTT_TerminalOut(2, "Sent handshake response\r\n");
#endif
          }
          break;
        default:
#ifdef DEBUG
          SEGGER_RTT_TerminalOut(2, "Unknown UART command\r\n");
#endif
          break;
        }
        intFlags ^= FL_URX_READY;
      }
    }

    avg = average;
    intFlags ^= FL_AVG_READY;

    if ((intFlags & FL_UTX_READY) && (stateFlags & FL_FWD_ADC) && (stateFlags & FL_HANDSHK))
    {
      txBuff[0] = 0x02;
      txBuff[1] = (avg >> 8) & 0xFF;
      txBuff[2] = avg & 0xFF;
      txBuff[3] = 0;
      intFlags ^= FL_UTX_READY;
      HAL_UART_Transmit_DMA(&huart1, txBuff, TX_BUF_LEN);
    }

#ifdef DEBUG
    sprintf(avgstr, "%ld\r\n", avg);
    SEGGER_RTT_TerminalOut(1, avgstr);
#endif
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14 | RCC_OSCILLATORTYPE_HSE;
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
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *myadc)
{
  for (int i = BUT_BUF_LEN / 2; i < BUT_BUF_LEN; i++)
  {
    average += readouts[i];
  }
  average /= BUT_BUF_LEN / 2;
#if VARIO_FILTER
  uint32_t maxVar = 0;
  for (int i = BUT_BUF_LEN / 2; i < BUT_BUF_LEN; i++)
  {
    uint32_t diff = abs((uint32_t)readouts[i] - average);
    if (diff > maxVar)
    {
      maxVar = diff;
    }
  }
  char maxstr[10];
#ifdef DEBUG
  SEGGER_RTT_TerminalOut(2, strcat(itoa(maxVar, maxstr, 10), "\r\n"));
#endif
  if (maxVar > MAX_VARIATION)
    if (maxVar > MAX_VARIATION) 
  if (maxVar > MAX_VARIATION)
    if (maxVar > MAX_VARIATION) 
  if (maxVar > MAX_VARIATION)
    return;
#endif
  intFlags |= FL_AVG_READY;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *myadc)
{
  for (int i = 0; i < BUT_BUF_LEN / 2; i++)
  {
    average += readouts[i];
  }
  average /= BUT_BUF_LEN / 2;
#if VARIO_FILTER
  uint32_t maxVar = 0;
  for (int i = 0; i < BUT_BUF_LEN / 2; i++)
  {
    uint32_t diff = abs((uint32_t)readouts[i] - average);
    if (diff > maxVar)
    {
      maxVar = diff;
    }
  }
  char maxstr[10];
#ifdef
  SEGGER_RTT_TerminalOut(2, strcat(itoa(maxVar, maxstr, 10), "\r\n"));
#endif
  if (maxVar > MAX_VARIATION)
    if (maxVar > MAX_VARIATION) 
  if (maxVar > MAX_VARIATION)
    if (maxVar > MAX_VARIATION) 
  if (maxVar > MAX_VARIATION)
    return;
#endif
  intFlags |= FL_AVG_READY;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  intFlags |= FL_URX_READY;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  intFlags |= FL_UTX_READY;
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
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
