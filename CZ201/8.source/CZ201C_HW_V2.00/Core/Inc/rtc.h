/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
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
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */
typedef struct
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t weekday;
}rtc_date_t;

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t ms;
}rtc_time_t;

extern rtc_date_t rtc_date;
extern rtc_time_t rtc_time;
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
uint8_t rtc_init(void);
uint8_t rtc_set_date(rtc_date_t date);
uint8_t rtc_get_date(rtc_date_t *date);
uint8_t rtc_set_time(rtc_time_t time);
uint8_t rtc_set_time_date(rtc_date_t date,rtc_time_t time);
uint8_t rtc_get_time(rtc_time_t *time);
uint8_t rtc_get_time_date(void);

//address: 1~19
void rtc_write_backup_register(uint8_t address, uint32_t data);
uint32_t rtc_read_backup_register(uint8_t address);

void rtc_printf_string(void);

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
