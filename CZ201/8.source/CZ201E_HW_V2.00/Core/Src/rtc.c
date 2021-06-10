/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "stdio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

rtc_date_t rtc_date;
rtc_time_t rtc_time;

/* RTC init function */
uint8_t rtc_init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
//		Error_Handler();
	}

	//第一次设置RTC时间
//	rtc_date.year = 2021;
//	rtc_date.month = 4;
//	rtc_date.day = 9;
//	rtc_date.weekday = 5;
//
//	rtc_time.hours = 19;
//	rtc_time.minutes = 05;
//	rtc_time.seconds = 0;
//	rtc_time.ms = 0;
//
//	rtc_set_date(rtc_date);
//	rtc_set_time(rtc_time);
//
//	rtc_get_time_date();

	return 0;
}

uint8_t rtc_set_date(rtc_date_t date)
{
	RTC_DateTypeDef sdate;

	sdate.Year = date.year - 2000;
	sdate.Month = date.month;
	sdate.Date = date.day;
	sdate.WeekDay = date.weekday;

	return HAL_RTC_SetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
}

uint8_t rtc_get_date(rtc_date_t *date)
{
	RTC_DateTypeDef sdate;

	if(HAL_RTC_GetDate(&hrtc,  &sdate, RTC_FORMAT_BIN) != HAL_OK) return 1;

	date->year = sdate.Year + 2000;
	date->month = sdate.Month;
	date->day = sdate.Date;
	date->weekday = sdate.WeekDay;

	return 0;
}

uint8_t rtc_set_time(rtc_time_t time)
{
	RTC_TimeTypeDef stime;

	stime.Hours = time.hours;
	stime.Minutes = time.minutes;
	stime.Seconds = time.seconds;
	stime.SubSeconds = time.ms;
	stime.TimeFormat = RTC_HOURFORMAT12_PM;
	stime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	stime.StoreOperation = RTC_STOREOPERATION_RESET;

	return HAL_RTC_SetTime(&hrtc, &stime, RTC_FORMAT_BIN);
}

uint8_t rtc_get_time(rtc_time_t *time)
{
	RTC_TimeTypeDef stime;

	if(HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN) != HAL_OK) return 1;

	time->hours = stime.Hours;
	time->minutes = stime.Minutes;
	time->seconds = stime.Seconds;
	time->ms = stime.SubSeconds;

	return 0;
}

/**
* @brief	设置RTC时间函数
* @param	s_rtc_date	日期
* @param	s_rtc_time	时间
* @retval	0：成功	1：失败
*/
uint8_t rtc_set_time_date(rtc_date_t date,rtc_time_t time)
{
	rtc_set_date(date);
	rtc_set_time(time);

	return 0;
}

uint8_t rtc_get_time_date(void)
{
	if(rtc_get_time(&rtc_time)) return 1;	//该系列必须些序列读
	if(rtc_get_date(&rtc_date)) return 1;

	return 0;
}

void rtc_printf_string(void)
{
	rtc_get_time_date();
	printf("RTC = %d年%d月%d日 %02d:%02d:%02d 星期%d\r\n", rtc_date.year, rtc_date.month, rtc_date.day,
			rtc_time.hours, rtc_time.minutes, rtc_time.seconds, rtc_date.weekday);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
