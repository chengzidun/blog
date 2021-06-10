/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "squeue.h"
/* USER CODE BEGIN Includes */
#define USART1_RX_BUFFER_SIZE		MAXQSIZE

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern uint16_t usart1_rx_size;
extern uint8_t usart1_rx_buffer[USART1_RX_BUFFER_SIZE];

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
void usart1_init(uint32_t baud);
uint8_t usart1_send_data(uint8_t *pdata, uint16_t size);
uint8_t usart1_send_string(const char *pdata);
uint16_t usart1_receive_data(uint8_t *pdata);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
