/*
 * beep.h
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: cheng
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BEEP_H__
#define __BEEP_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

#ifdef __cplusplus
 extern "C" {
#endif

#define BEEP_OUT_W(state)	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, (GPIO_PinState)state)
#define BEEP_OUT_ON()		BEEP_OUT_W(1)
#define BEEP_OUT_OFF()		BEEP_OUT_W(0)
#define BEEP_OUT_TOGGLE()	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6)

uint8_t beep_init(void);
void beep_oscillation(uint32_t half_cycle_ms, uint32_t time_ms);
void beep_oscillation_cnt(void);
void beep_oscillation_clear(void);
uint8_t beep_oscillation_get_state(void);

#ifdef __cplusplus
}
#endif

#endif /* __BEEP_H__ */

