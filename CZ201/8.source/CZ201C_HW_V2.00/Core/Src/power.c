/*
 * power.c
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: bocon
 */

#include "power.h"

static uint8_t power_ac_flag = 0;
static uint8_t power_ac_state = 0;

uint8_t power_init(void)
{
	power_ac_state = POWER_AC_STATE_IN();
	power_ac_flag = 1;

	return 0;
}

uint8_t power_get_ac_state(void)
{
	return power_ac_state;
}

void power_ac_state_cnt(void)
{
	static uint32_t cnt = 0;

	if(power_ac_flag == 1)
	{
		if(POWER_AC_STATE_IN() != power_ac_state)
		{
			if(++cnt > 2000)
			{
				power_ac_state = POWER_AC_STATE_IN();
				cnt = 0;
			}
		}
		else
		{
			cnt = 0;
		}
	}
}
