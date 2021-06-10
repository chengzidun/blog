/*
 * gpio.h
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: cheng
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H__
#define __LED_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

#ifdef __cplusplus
 extern "C" {
#endif

#define LED_RUN_W(state)	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)state)
#define LED_RUN_ON()		LED_RUN_W(1)
#define LED_RUN_OFF()		LED_RUN_W(0)
#define LED_RUN_TOGGLE()	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4)

#define LED_LIGHT_W(state)	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, (GPIO_PinState)state)
#define LED_LIGHT_ON()		LED_LIGHT_W(1)
#define LED_LIGHT_OFF()		LED_LIGHT_W(0)
#define LED_LIGHT_TOGGLE()	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)

uint8_t led_init(void);
void led_blink(uint8_t number, uint32_t half_cycle_ms, uint32_t time_ms);
void led_blink_cnt(void);
void led_blink_clear(uint8_t number);
uint8_t led_blink_get_state(uint8_t number);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */

