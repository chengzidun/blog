/*
 * led.c
 *
 *  Created on: 2019年1月8日
 *      Author: cheng
 */

#include "led.h"

static uint8_t led_lock_flag = 0;
static uint32_t led_lock_half_cycle_ms = 500;
static uint32_t led_lock_time_ms = 1000;
static uint32_t led_lock_half_cycle_cnt = 0;
static uint32_t led_lock_time_cnt = 0;

static uint8_t led_light_flag = 0;
static uint32_t led_light_half_cycle_ms = 500;
static uint32_t led_light_time_ms = 1000;
static uint32_t led_light_half_cycle_cnt = 0;
static uint32_t led_light_time_cnt = 0;

uint8_t led_init(void)
{
	led_lock_flag = 0;
	led_lock_half_cycle_ms = 500;
	led_lock_time_ms = 1000;
	led_lock_half_cycle_cnt = 0;
	led_lock_time_cnt = 0;

	led_light_flag = 0;
	led_light_half_cycle_ms = 500;
	led_light_time_ms = 1000;
	led_light_half_cycle_cnt = 0;
	led_light_time_cnt = 0;

	return 0;
}

//1ms进入定时器, 2ms为一个周期, 最高频率是500Hz
//half_cycle_ms是点亮或熄灭时间, time_ms是闪烁有效时间
//half_cycle_ms=0点亮不闪烁, time_ms=0时间直到宇宙毁灭
void led_blink(uint8_t number, uint32_t half_cycle_ms, uint32_t time_ms)
{
	switch(number)
	{
		case 0:
			if(led_lock_flag == 0)
			{
				led_lock_half_cycle_ms = half_cycle_ms;
				led_lock_time_ms = time_ms;
				if(time_ms != 0 && time_ms < led_lock_half_cycle_ms) led_lock_time_ms = led_lock_half_cycle_ms << 1;
				LED_LOCK_ON();
				led_lock_flag = 1;
			}
		break;
		case 1:
			if(led_light_flag == 0)
			{
				led_light_half_cycle_ms = half_cycle_ms;
				led_light_time_ms = time_ms;
				if(time_ms != 0 && time_ms < led_light_half_cycle_ms) led_light_time_ms = led_light_half_cycle_ms << 1;
				LED_LIGHT_ON();
				led_light_flag = 1;
			}
		break;
	}
}

void led_blink_cnt(void)
{
	if(led_lock_flag == 1)
	{
		if(led_lock_time_ms == 0 || ++led_lock_time_cnt < led_lock_time_ms)
		{
			if(led_lock_half_cycle_ms != 0 && ++led_lock_half_cycle_cnt > led_lock_half_cycle_ms)
			{
				LED_LOCK_TOGGLE();
				led_lock_half_cycle_cnt = 0;
			}
		}
		else
		{
			led_lock_flag = 0;
			led_lock_half_cycle_cnt = 0;
			led_lock_time_cnt = 0;
			LED_LOCK_OFF();
		}
	}

	if(led_light_flag == 1)
	{
		if(led_light_time_ms == 0 || ++led_light_time_cnt < led_light_time_ms)
		{
			if(led_light_half_cycle_ms != 0 && ++led_light_half_cycle_cnt > led_light_half_cycle_ms)
			{
				LED_LIGHT_TOGGLE();
				led_light_half_cycle_cnt = 0;
			}
		}
		else
		{
			led_light_flag = 0;
			led_light_half_cycle_cnt = 0;
			led_light_time_cnt = 0;
			LED_LIGHT_OFF();
		}
	}
}

void led_blink_clear(uint8_t number)
{
	switch(number)
	{
		case 0:
			led_lock_flag = 0;
			led_lock_half_cycle_ms = 500;
			led_lock_time_ms = 1000;
			led_lock_half_cycle_cnt = 0;
			led_lock_time_cnt = 0;
			LED_LOCK_OFF();
		break;
		case 1:
			led_light_flag = 0;
			led_light_half_cycle_ms = 500;
			led_light_time_ms = 1000;
			led_light_half_cycle_cnt = 0;
			led_light_time_cnt = 0;
			LED_LIGHT_OFF();
		break;
	}
}

uint8_t led_blink_get_state(uint8_t number)
{
	uint8_t led_flag = 0;

	switch(number)
	{
		case 0:
			led_flag = led_lock_flag;
		break;
		case 1:
			led_flag = led_light_flag;
		break;
	}

	return led_flag;

}
