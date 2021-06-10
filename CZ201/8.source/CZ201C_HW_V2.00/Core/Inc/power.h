/*
 * power.h
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: cheng
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POWER_H__
#define __POWER_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

#ifdef __cplusplus
 extern "C" {
#endif

#define POWER_AC_STATE_IN()		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)

uint8_t power_init(void);
uint8_t power_get_ac_state(void);
void power_ac_state_cnt(void);

#ifdef __cplusplus
}
#endif

#endif /* __POWER_H__ */

