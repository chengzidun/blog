/*
 * e18_ms1_ipx.c
 *
 *  Created on: 2019年1月8日
 *      Author: bocon
 */

#include "e18_ms1_ipx.h"
#include "usart.h"
#include "led.h"
#include "beep.h"

uint8_t e18_ms1_ipx_send_flag = 0;
uint8_t e18_ms1_ipx_send_key = 0;
uint8_t e18_ms1_ipx_overtime_flag = 0;
uint32_t e18_ms1_ipx_overtime_cnt = 0;

e18_ms1_ipx_t e18_ms1_ipx;

uint8_t e18_ms1_ipx_init(void)
{
	uint8_t buffer[MAXQSIZE];
	uint16_t rx_size;

	//配置为终端
	buffer[0] = 0xFD;
	buffer[1] = 0x02;
	buffer[2] = 0x01;
	buffer[3] = 0x02;
	buffer[4] = 0xFF;
	usart1_send_data(buffer, 5);
	HAL_Delay(50);
	rx_size = usart1_receive_data(buffer);
	if(rx_size == 0x02 && buffer[0] == 0xFA && buffer[1] == 0x01)
	{

	}

	//配置信道
	buffer[0] = 0xFD;
	buffer[1] = 0x02;
	buffer[2] = 0x0A;
	buffer[3] = E18_MS1_IPX_CHANNEL_MIN;
	buffer[4] = 0xFF;
	usart1_send_data(buffer, 5);
	HAL_Delay(50);
	rx_size = usart1_receive_data(buffer);
	if(rx_size == 0x02 && buffer[0] == 0xFA && buffer[1] == 0x0A)
	{

	}

	//重启设备
	buffer[0] = 0xFD;
	buffer[1] = 0x01;
	buffer[2] = 0x12;
	buffer[3] = 0xFF;
	usart1_send_data(buffer, 4);
	HAL_Delay(50);
	rx_size = usart1_receive_data(buffer);
	if(rx_size == 0x02 && buffer[0] == 0xFA && buffer[1] == 0x12)
	{

	}
	HAL_Delay(1000);

	//读取设备所有数据
	buffer[0] = 0xFE;
	buffer[1] = 0x01;
	buffer[2] = 0xFE;
	buffer[3] = 0xFF;
	usart1_send_data(buffer, 4);
	HAL_Delay(100);
	rx_size = usart1_receive_data(buffer);
	if(rx_size == 0x2E && buffer[0] == 0xFB)
	{
		uint8_t *pbuffer = (uint8_t*)(&e18_ms1_ipx);
		for(uint8_t i = 0; i < 45; i++)
			pbuffer[i] = buffer[i+1];
	}

	e18_ms1_ipx_send_flag = 0;
	e18_ms1_ipx_send_key = 0;
	e18_ms1_ipx_overtime_flag = 0;
	e18_ms1_ipx_overtime_cnt = 0;

	return 0;
}

void e18_ms1_ipx_send_overtime_cnt(void)
{
	if(e18_ms1_ipx_send_flag == 1)
	{
		if(e18_ms1_ipx_overtime_flag == 0)
		{
			if(++e18_ms1_ipx_overtime_cnt > 1999)	//累计超过2s则超时
			{
				e18_ms1_ipx_overtime_cnt = 0;
				e18_ms1_ipx_overtime_flag = 1;
			}
		}
	}
}

void e18_ms1_ipx_overtime_alarm(void)
{
	//超时报警
	if(e18_ms1_ipx_send_flag == 1 && e18_ms1_ipx_overtime_flag == 1)
	{
		led_blink_clear(e18_ms1_ipx_send_key);
		led_blink(e18_ms1_ipx_send_key, 200, 3000);
		beep_oscillation_clear();
		beep_oscillation(200, 3000);

		e18_ms1_ipx_send_flag = 0;
		e18_ms1_ipx_overtime_flag = 0;
		e18_ms1_ipx_overtime_cnt = 0;
	}
}

void e18_ms1_ipx_protocol_handle(uint8_t *pdata, uint16_t size)
{
	uint8_t buffer[MAXQSIZE];
	uint16_t len = size;
	static uint8_t nonetwork_cnt = 0;

	if(len == 2)	//该类信息是模块主动上报MCU
	{
		if(pdata[0] == 0xFF && pdata[1] == 0x00)	//无网络或失去网络
		{
			e18_ms1_ipx.nwk_state = 0;
			led_blink_clear(0);
			led_blink(0, 500, 0);
			if(++nonetwork_cnt > 4)
			{
				//重启模块
				buffer[0] = 0xFD;
				buffer[1] = 0x01;
				buffer[2] = 0x12;
				buffer[3] = 0xFF;
				usart1_send_data(buffer, 4);
				nonetwork_cnt = 0;
			}
		}

		else if(pdata[0] == 0xFF && pdata[1] == 0xFF)	//协调器设备建立网络
		{
			led_blink_clear(0);
			led_blink(0, 50, 1000);
		}
		else if(pdata[0] == 0xFF && pdata[1] == 0xAA)	//设备加入网络
		{
			e18_ms1_ipx.nwk_state = 1;
			nonetwork_cnt = 0;
			led_blink_clear(0);
			led_blink(0, 200, 1000);
		}
		else if(pdata[0] == 0xF7 && pdata[1] == 0xFF)		//串口数据格式错误
		{

		}
	}
	else
	{
		if(len == 5)	//接收答应(最后两字节是协调器网络短地址, 固定是0x0000)
		{
			if(pdata[0] == 0xBF)
			{
				//接收正常清除标志
				e18_ms1_ipx_send_flag = 0;
				e18_ms1_ipx_overtime_flag = 0;
				e18_ms1_ipx_overtime_cnt = 0;
				switch(pdata[1])	//跟原先发送命令一致
				{
					case 0x01:	//开锁应答
						switch(pdata[2])	//返回错误码
						{
							case 0:	//成功

							break;
							case 1:	//失败
							break;
							case 2:	//忙碌
							break;
						}
					break;
					case 0x02:	//闪灯应答
						switch(pdata[2])	//返回错误码
						{
							case 0:	//成功

							break;
							case 1:	//失败
								led_blink_clear(1);
								led_blink(1, 200, 3000);
								beep_oscillation_clear();
								beep_oscillation(200, 3000);
							break;
							case 2:	//忙碌
							break;
						}
					break;
				}
			}
		}

	}
}

