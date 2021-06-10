/*
 * delay.c
 *
 *  Created on: 2019Äê1ÔÂ8ÈÕ
 *      Author: cheng
 */

#include "delay.h"
#include "key.h"
#include "softtimer.h"
#include "led.h"
#include "beep.h"
#include "power.h"
#include "e18_ms1_ipx.h"

void HAL_SYSTICK_Callback(void)
{
	timer_exec();
	timer_service();
	key_scan();
	led_blink_cnt();
	beep_oscillation_cnt();
	power_ac_state_cnt();
	e18_ms1_ipx_send_overtime_cnt();
}


