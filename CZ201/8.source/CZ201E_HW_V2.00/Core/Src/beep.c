/*
 * beep.c
 *
 *  Created on: 2019年1月8日
 *      Author: bocon
 */

#include "beep.h"

static uint8_t beep_flag = 0;
static uint32_t beep_half_cycle_ms = 500;
static uint32_t beep_time_ms = 1000;
static uint32_t beep_half_cycle_cnt = 0;
static uint32_t beep_time_cnt = 0;

uint8_t beep_init(void)
{
	beep_flag = 0;
	beep_half_cycle_ms = 500;
	beep_time_ms = 1000;
	beep_half_cycle_cnt = 0;
	beep_time_cnt = 0;

	return 0;
}

//1ms进入定时器, 2ms为一个周期, 最高频率是500Hz
//half_cycle_ms是点亮或熄灭时间, time_ms是闪烁有效时间
//half_cycle_ms=0点亮不闪烁, time_ms=0时间直到宇宙毁灭
void beep_oscillation(uint32_t half_cycle_ms, uint32_t time_ms)
{
	if(beep_flag == 0)
	{
		beep_half_cycle_ms = half_cycle_ms;
		beep_time_ms = time_ms;
		if(time_ms != 0 && time_ms < beep_half_cycle_ms) beep_time_ms = beep_half_cycle_ms << 1;
		BEEP_OUT_ON();
		beep_flag = 1;
	}
}

void beep_oscillation_cnt(void)
{
	if(beep_flag == 1)
	{
		if(beep_time_ms == 0 || ++beep_time_cnt < beep_time_ms)
		{
			if(beep_half_cycle_ms != 0 && ++beep_half_cycle_cnt > beep_half_cycle_ms)
			{
				BEEP_OUT_TOGGLE();
				beep_half_cycle_cnt = 0;
			}
		}
		else
		{
			beep_flag = 0;
			beep_half_cycle_cnt = 0;
			beep_time_cnt = 0;
			BEEP_OUT_OFF();
		}
	}
}

void beep_oscillation_clear(void)
{
	beep_flag = 0;
	beep_half_cycle_ms = 500;
	beep_time_ms = 1000;
	beep_half_cycle_cnt = 0;
	beep_time_cnt = 0;
	BEEP_OUT_OFF();
}

uint8_t beep_oscillation_get_state(void)
{
	return beep_flag;
}
