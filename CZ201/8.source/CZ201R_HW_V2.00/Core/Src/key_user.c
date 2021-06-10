#include "key_user.h"
#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "beep.h"
#include "e18_ms1_ipx.h"

static KEY_t gKeyTbl[KEY_MAX];	//����Ҫ�İ���������

static const KeyEventCallback gCallback[KEY_MAX][KEY_CALLBACK_NUM] =
{
	//��һά�ǰ������������ڶ�ά��ÿ�������Ļص�����
	//���ĸ��ص�����������˳��Ϊ�̰���������̧�����������
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
//		//�㲥ģʽ
//		buffer[0] = 0xFC;
//		buffer[1] = 0x0C;
//		buffer[2] = 0x03;
//		buffer[3] = 0x02;
//		buffer[4] = 0x00;	//Э��������̵�ַ�̶�Ϊ0x0000
//		buffer[5] = 0x00;
//		buffer[6] = 0xAF;	//֡ͷ
//		buffer[7] = 0x06;	//����, ������֡ͷ��֡β
//		buffer[8] = 0x01;	//��������
//		buffer[9] = 0x00;	//����, ��Чʱ��0
//		buffer[10] = 0x00;
//		buffer[11] = 0x00;
//		buffer[12] = 0x00;
//		buffer[13] = 0xAD;	//֡β
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
//		//�㲥ģʽ
//		buffer[0] = 0xFC;
//		buffer[1] = 0x0C;
//		buffer[2] = 0x03;
//		buffer[3] = 0x02;
//		buffer[4] = 0x00;	//Э��������̵�ַ�̶�Ϊ0x0000
//		buffer[5] = 0x00;
//		buffer[6] = 0xAF;	//֡ͷ
//		buffer[7] = 0x06;	//����, ������֡ͷ��֡β
//		buffer[8] = 0x02;	//��������
//		buffer[9] = 0x00;	//����(4byte), ��Чʱ��0
//		buffer[10] = 0x00;
//		buffer[11] = 0x4E;	//Ĭ�ϵ������õ�20s
//		buffer[12] = 0x20;
//		buffer[13] = 0xAD;	//֡β
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
