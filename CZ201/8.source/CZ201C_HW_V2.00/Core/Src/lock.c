/*
 * lock.c
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: bocon
 */

#include "lock.h"

static uint8_t lock_flag = 0;
static uint32_t lock_delay_ms = 500;

uint8_t lock_init(void)
{
	lock_flag = 0;
	lock_delay_ms = 500;

	return 0;
}

void lock_unlocking(uint32_t delay_ms)
{
	if(lock_flag == 0)
	{
		LOCK_CTR_ON();
		lock_delay_ms = delay_ms;
		lock_flag = 1;
	}
}

void lock_unlocking_time_cnt(void)
{
	static uint32_t cnt = 0;

	if(lock_flag == 1)
	{
		if(++cnt > lock_delay_ms)
		{
			LOCK_CTR_OFF();
			lock_flag = 0;
			cnt = 0;
		}
	}
}

void lock_unlocking_clear(void)
{
	lock_flag = 0;
	lock_delay_ms = 500;
	LOCK_CTR_OFF();
}

uint8_t lock_get_state(void)
{
	return lock_flag;
}
