#include "key_user.h"
#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "beep.h"
#include "e18_ms1_ipx.h"

static KEY_t gKeyTbl[KEY_MAX];	//你需要的按键最多个数

static const KeyEventCallback gCallback[KEY_MAX][KEY_CALLBACK_NUM] =
{
	//第一维是按键最多个数；第二维是每个按键的回调函数
	//共四个回调函数，依次顺序为短按、长按、抬起和连发函数
	{ key_lock_press_cb, key_lock_long_cb, key_lock_release_cb, key_lock_retrig_cb },
	{ key_light_press_cb, key_light_long_cb, key_light_release_cb, key_light_retrig_cb },
};

void key_gpio_config(void)
{

}

u8_t key_init(void)
{
	load_key_tbl(gKeyTbl, gCallback);

	return 0;
}

//key_lock
__weak void key_lock_press_cb(void)
{
//	uint8_t buffer[20];
//
//	if(e18_ms1_ipx.nwk_state == 1 && e18_ms1_ipx_send_flag == 0)
//	{
//		//点播模式
//		buffer[0] = 0xFC;
//		buffer[1] = 0x0C;
//		buffer[2] = 0x03;
//		buffer[3] = 0x02;
//		buffer[4] = 0x00;	//协调器网络短地址固定为0x0000
//		buffer[5] = 0x00;
//		buffer[6] = 0xAF;	//帧头
//		buffer[7] = 0x06;	//长度, 不包括帧头和帧尾
//		buffer[8] = 0x01;	//命令类型
//		buffer[9] = 0x00;	//参数, 无效时置0
//		buffer[10] = 0x00;
//		buffer[11] = 0x00;
//		buffer[12] = 0x00;
//		buffer[13] = 0xAD;	//帧尾
//		usart1_send_data(buffer, 14);
//
//		e18_ms1_ipx_send_key = 0;
//		e18_ms1_ipx_overtime_flag = 0;
//		e18_ms1_ipx_overtime_cnt = 0;
//		e18_ms1_ipx_send_flag = 1;
//
//		led_blink_clear(0);
//		led_blink(0, 100, 1000);
//		beep_oscillation_clear();
//		beep_oscillation(100, 500);
//	}

	led_blink_clear(0);
	led_blink(0, 100, 1000);
//	beep_oscillation_clear();
//	beep_oscillation(100, 500);
}

__weak void key_lock_long_cb(void)
{

}

__weak void key_lock_release_cb(void)
{

}

__weak void key_lock_retrig_cb(void)
{

}

//key_light
__weak void key_light_press_cb(void)
{
//	uint8_t buffer[20];
//
//	if(e18_ms1_ipx.nwk_state == 1 && e18_ms1_ipx_send_flag == 0)
//	{
//		//点播模式
//		buffer[0] = 0xFC;
//		buffer[1] = 0x0C;
//		buffer[2] = 0x03;
//		buffer[3] = 0x02;
//		buffer[4] = 0x00;	//协调器网络短地址固定为0x0000
//		buffer[5] = 0x00;
//		buffer[6] = 0xAF;	//帧头
//		buffer[7] = 0x06;	//长度, 不包括帧头和帧尾
//		buffer[8] = 0x02;	//命令类型
//		buffer[9] = 0x00;	//参数(4byte), 无效时置0
//		buffer[10] = 0x00;
//		buffer[11] = 0x4E;	//默认点亮外置灯20s
//		buffer[12] = 0x20;
//		buffer[13] = 0xAD;	//帧尾
//		usart1_send_data(buffer, 14);
//
//		e18_ms1_ipx_send_key = 1;
//		e18_ms1_ipx_overtime_flag = 0;
//		e18_ms1_ipx_overtime_cnt = 0;
//		e18_ms1_ipx_send_flag = 1;
//
//		led_blink_clear(1);
//		led_blink(1, 100, 1000);
//		beep_oscillation_clear();
//		beep_oscillation(100, 500);
//	}

	led_blink_clear(1);
	led_blink(1, 100, 1000);
//	beep_oscillation_clear();
//	beep_oscillation(100, 500);
}

__weak void key_light_long_cb(void)
{


}

__weak void key_light_release_cb(void)
{

}	

__weak void key_light_retrig_cb(void)
{

}
