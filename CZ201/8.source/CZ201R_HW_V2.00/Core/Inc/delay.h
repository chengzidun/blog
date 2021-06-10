/*
 * delay.h
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: cheng
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H__
#define __DELAY_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

#ifdef __cplusplus
 extern "C" {
#endif

void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H__ */
