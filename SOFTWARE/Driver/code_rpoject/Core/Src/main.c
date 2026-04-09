/* USER CODE BEGIN Header */
#define TEST_FOC_OPEN_LOOP 1
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usart.h"
#include "opamp.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <FOC/foc_utils.h>
#include <application/app_utils.h>
#include <application/comm_handler.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
__IO float32_t vopamp1 = 0.0f;
__IO float32_t vopamp2 = 0.0f;
__IO float32_t vopamp3 = 0.0f;

__IO float32_t precise_vbus = 0.0f;

__IO float32_t ID_input = 0.0f;
__IO float32_t IQ_input = 0.0f;

__IO float32_t ID_feedback = 0.0f;
__IO float32_t IQ_feedback = 0.0f;

__IO float32_t CNT_val = 0;
__IO float32_t elec_angle = 0.0f;
__IO float32_t meca_angle = 0.0f;


__IO float32_t voltage_Ud = 0.0f;
__IO float32_t voltage_Uq = 0.0f;

__IO float32_t rpm_val = 0.0f;

__IO float32_t val_in_500Hz_a = 0.0f;
__IO float32_t val_in_500Hz_b = 0.0f;

__IO float32_t val_in_10kHz_a = 0.0f;
__IO float32_t val_in_10kHz_b = 0.0f;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
FSM_HandleTypeDef application_control;
FOC_HandleTypeDef foc_control_system;
COM_HandleTypeDef communication_system;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ENC_HandleTypeDef encoder;
MOT_HandleTypeDef motor;
ANG_HandleTypeDef analog;
Diagram_signal_t  inner_signal;
PID_Control_t     pid_control_system;

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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_LPUART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_OPAMP1_Init();
  MX_OPAMP2_Init();
  MX_OPAMP3_Init();
  MX_TIM2_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  foc_init(&foc_control_system, &htim1, &htim3, &hadc1, &hadc2);
  comm_init(&communication_system, &hlpuart1);
  application_init(&application_control, &foc_control_system, &communication_system);

  foc_control_system.encoder = &encoder;
  foc_control_system.motor   = &motor;
  foc_control_system.analog  = &analog;
  foc_control_system.inner_signal = &inner_signal;

  HAL_OPAMP_Start(&hopamp1);
  HAL_OPAMP_Start(&hopamp2);
  HAL_OPAMP_Start(&hopamp3);

  application_setup(&application_control);

  HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);  // 20kHz
  HAL_NVIC_SetPriority(TIM2_IRQn,          1, 0);  // 500Hz

  HAL_TIM_Base_Start_IT(&htim2);

  HAL_Delay(500);
//  encoder_counter_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ENC_CS_Z_Pin)
    {
    	update_encoder_interrupt(foc_control_system.encoder, foc_control_system.timer2);
    }
}

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//    if (huart->Instance == LPUART1) {
//        tx_busy = 0;
//    }
//}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
#if TEST_FOC_OPEN_LOOP
	if (htim->Instance == TIM2) {
		exec_open_loop_500Hz(&application_control);
		val_in_500Hz_a = foc_control_system.inner_signal->Ualpha;
		val_in_500Hz_b = foc_control_system.inner_signal->Ubeta;

//		rpm_val = foc_control_system.encoder->rpm_measured;

	}
	if(htim->Instance == TIM1){
		exec_loop_10kHz(&application_control);
		val_in_10kHz_a = foc_control_system.Id_averaged;
		val_in_10kHz_b = foc_control_system.Iq_averaged;
		//		HAL_GPIO_TogglePin(TEST_ADC_GPIO_Port, TEST_ADC_Pin);
	}
#endif
}


void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc) {

	if(hadc->Instance == ADC1){
		precise_vbus = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);

		vopamp1 = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
		foc_control_system.analog->Ic = (float32_t)vopamp1 * 0.0148f - 23.202f;

//		HAL_GPIO_TogglePin(TEST_ADC_GPIO_Port, TEST_ADC_Pin);
	}
	if(hadc->Instance == ADC2){
		vopamp2 = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
		vopamp3 = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
		foc_control_system.analog->Ib = (float32_t)vopamp2 * 0.0148f - 23.202f;
		foc_control_system.analog->Ia = (float32_t)vopamp3 * 0.0148f - 23.202f;
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
