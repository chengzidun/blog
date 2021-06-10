#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__

#include "stm32f0xx_hal.h"
#include "global.h"

void key_gpio_config(void);

/**
 * ���þ���Ӳ��
 */

//����һЩ�����ĳ�ʼ��������GPIO�����õ�
#define KEY_CONFIG()  	key_gpio_config()

/**
 * ���뱣֤�߼�: ���·���1, ̧�𷵻�0, ����Ҫ�İ�������0
 */
#define GET_KEY0()		(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))	//key_lock_in

/**
 * ���ò���
 */

//����Ҫ�Ļص��������Ϳ��Թص��Խ�ʡRAM�ռ�
#define KEY_PRESS_CALLBACK_EN			1		//ʹ�ܴ�������ʱ�Ļص�����
#define KEY_LONG_CALLBACK_EN			1		//ʹ�ܴ��������Ļص�����
#define KEY_UP_CALLBACK_EN				1 		//ʹ�ܴ���̧��Ļص�����
#define KEY_RETRIG_CALLBACK_EN			1		//ʹ�ܴ��������Ļص�����
#define KEY_CALLBACK_NUM				4		//ÿ���������ĵĻص���������
 
#define KEY_LONG_PERIOD					1000	//������ʱ��=KEY_LONG_PERIOD * Ttmr (TtmrΪ��ʱ������һ���жϵ�ʱ��)
#define KEY_RETRIG_PERIOD				100		//�����ļ��ʱ��=KEY_RETRIG_PERIOD * Ttmr
#define KEY_MAX							1		//֧�ֵİ�������

#endif
