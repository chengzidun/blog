/*
 * e18_ms1_ipx.c
 *
 *  Created on: 2019��1��8��
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

	//����Ϊ�ն�
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

	//�����ŵ�
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

	//�����豸
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

	//��ȡ�豸��������
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
			if(++e18_ms1_ipx_overtime_cnt > 1999)	//�ۼƳ���2s��ʱ
			{
				e18_ms1_ipx_overtime_cnt = 0;
				e18_ms1_ipx_overtime_flag = 1;
			}
		}
	}
}

void e18_ms1_ipx_overtime_alarm(void)
{
	//��ʱ����
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

	if(len == 2)	//������Ϣ��ģ�������ϱ�MCU
	{
		if(pdata[0] == 0xFF && pdata[1] == 0x00)	//�������ʧȥ����
		{
			e18_ms1_ipx.nwk_state = 0;
			led_blink_clear(0);
			led_blink(0, 500, 0);
			if(++nonetwork_cnt > 4)
			{
				//����ģ��
				buffer[0] = 0xFD;
				buffer[1] = 0x01;
				buffer[2] = 0x12;
				buffer[3] = 0xFF;
				usart1_send_data(buffer, 4);
				nonetwork_cnt = 0;
			}
		}

		else if(pdata[0] == 0xFF && pdata[1] == 0xFF)	//Э�����豸��������
		{
			led_blink_clear(0);
			led_blink(0, 50, 1000);
		}
		else if(pdata[0] == 0xFF && pdata[1] == 0xAA)	//�豸��������
		{
			e18_ms1_ipx.nwk_state = 1;
			nonetwork_cnt = 0;
			led_blink_clear(0);
			led_blink(0, 200, 1000);
		}
		else if(pdata[0] == 0xF7 && pdata[1] == 0xFF)		//�������ݸ�ʽ����
		{

		}
	}
	else
	{
		if(len == 5)	//���մ�Ӧ(������ֽ���Э��������̵�ַ, �̶���0x0000)
		{
			if(pdata[0] == 0xBF)
			{
				//�������������־
				e18_ms1_ipx_send_flag = 0;
				e18_ms1_ipx_overtime_flag = 0;
				e18_ms1_ipx_overtime_cnt = 0;
				switch(pdata[1])	//��ԭ�ȷ�������һ��
				{
					case 0x01:	//����Ӧ��
						switch(pdata[2])	//���ش�����
						{
							case 0:	//�ɹ�

							break;
							case 1:	//ʧ��
							break;
							case 2:	//æµ
							break;
						}
					break;
					case 0x02:	//����Ӧ��
						switch(pdata[2])	//���ش�����
						{
							case 0:	//�ɹ�

							break;
							case 1:	//ʧ��
								led_blink_clear(1);
								led_blink(1, 200, 3000);
								beep_oscillation_clear();
								beep_oscillation(200, 3000);
							break;
							case 2:	//æµ
							break;
						}
					break;
				}
			}
		}

	}
}

