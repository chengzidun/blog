/*
 * e18_ms1_ipx.c
 *
 *  Created on: 2019��1��8��
 *      Author: bocon
 */

#include "e18_ms1_ipx.h"
#include "usart.h"
#include "led.h"
#include "lock.h"
#include "power.h"

e18_ms1_ipx_t e18_ms1_ipx;

uint8_t e18_ms1_ipx_init(void)
{
	uint8_t buffer[MAXQSIZE];
	uint16_t rx_size;

//	����ΪЭ����
	buffer[0] = 0xFD;
	buffer[1] = 0x02;
	buffer[2] = 0x01;
	buffer[3] = E18_MS1_IPX_DEV_TYPE_C;
	buffer[4] = 0xFF;
	usart1_send_data(buffer, 5);
	HAL_Delay(50);
	rx_size = usart1_receive_data(buffer);
	if(rx_size == 0x02 && buffer[0] == 0xFA && buffer[1] == 0x01)
	{

	}

//	�����ŵ�
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

	return 0;
}

void e18_ms1_ipx_protocol_handle(uint8_t *pdata, uint16_t size)
{
	uint8_t buffer[20];
	uint16_t len = size;
	uint32_t time_ms;
	static uint8_t nonetwork_cnt = 0;

	if(len == 2)	//������Ϣ��ģ�������ϱ�MCU
	{
		if(pdata[0] == 0xFF && pdata[1] == 0x00)	//�������ʧȥ����
		{
			e18_ms1_ipx.nwk_state = 0;
			led_blink_clear(0);
			led_blink(0, 500, 0);
			if(++nonetwork_cnt > 9)
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
			e18_ms1_ipx.nwk_state = 1;
			nonetwork_cnt = 0;
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
		if(len == 10)
		{
			if(pdata[0] == 0xAF && pdata[1] == 6 && pdata[7] == 0xAD)
			{
				switch(pdata[2])
				{
					case 0x01:	//����
						lock_unlocking(500);
						//�㲥ģʽ(�̵�ַ��ʽ)
						buffer[0] = 0xFC;
						buffer[1] = 0x07;
						buffer[2] = 0x03;
						buffer[3] = 0x02;
						buffer[4] = pdata[8];
						buffer[5] = pdata[9];
						buffer[6] = 0xBF;
						buffer[7] = 0x01;
						buffer[8] = 0x00;
						usart1_send_data(buffer, 9);
					break;
					case 0x02:	//����
						if(power_get_ac_state() == 1)	//���е����õƲŵ���
						{
							time_ms = pdata[3] << 24 | pdata[4] << 16 | pdata[5] << 8 | pdata[6];
							led_blink_clear(1);
							led_blink(1, 0, time_ms);
						}

						//�㲥ģʽ(�̵�ַ��ʽ)
						buffer[0] = 0xFC;
						buffer[1] = 0x07;
						buffer[2] = 0x03;
						buffer[3] = 0x02;
						buffer[4] = pdata[8];
						buffer[5] = pdata[9];
						buffer[6] = 0xBF;
						buffer[7] = 0x02;
						buffer[8] = 0x00;
						if(power_get_ac_state() == 0)	//���е緵��ʧ��
							buffer[8] = 0x01;
						usart1_send_data(buffer, 9);
					break;
				}
			}
		}

	}
}
