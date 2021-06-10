/*
 * lock.h
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: cheng
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOCK_H__
#define __LOCK_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

#ifdef __cplusplus
 extern "C" {
#endif

#define LOCK_CTR_W(state)	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (GPIO_PinState)state)
#define LOCK_CTR_ON()		LOCK_CTR_W(1)
#define LOCK_CTR_OFF()		LOCK_CTR_W(0)

uint8_t lock_init(void);
void lock_unlocking(uint32_t delay_ms);
void lock_unlocking_time_cnt(void);
void lock_unlocking_clear(void);
uint8_t lock_get_state(void);

#ifdef __cplusplus
}
#endif

#endif /* __LOCK_H__ */

