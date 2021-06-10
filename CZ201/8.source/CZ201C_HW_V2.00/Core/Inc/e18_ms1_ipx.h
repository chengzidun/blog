/*
 * e18_ms1_ipx.h
 *
 *  Created on: 2019��1��8��
 *      Author: cheng
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __E18_MS1_IPX_H__
#define __E18_MS1_IPX_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

#ifdef __cplusplus
 extern "C" {
#endif

/*---------------------------------------------------------------------------*/
//�豸����
#define E18_MS1_IPX_DEV_TYPE_C					0x00
#define E18_MS1_IPX_DEV_TYPE_R					0x01
#define E18_MS1_IPX_DEV_TYPE_D					0x02

//����״̬
#define E18_MS1_IPX_NWK_STATE_NO				0x00
#define E18_MS1_IPX_NWK_STATE_OK				0x01

//�������, 0x0000~0xFFFE�̶�����
#define E18_MS1_IPX_PAN_ID_RANDOM				0xFFFF

//Э����������̵�ַ�̶�Ϊ0x0000
#define E18_MS1_IPX_DEV_C_FIXED_SHORT_ADDR		0x0000

//�������
#define E18_MS1_IPX_GROUP_MIN					1		//Ĭ��
#define E18_MS1_IPX_GROUP_MAX					99

//�����ŵ�
#define E18_MS1_IPX_CHANNEL_MIN					11		//Ĭ��
#define E18_MS1_IPX_CHANNEL_MAX					26

//���͹���
#define E18_MS1_IPX_TXPOWER_N3_16_20_DBM		0x00
#define E18_MS1_IPX_TXPOWER_N1D5_17_22_DBM		0x01
#define E18_MS1_IPX_TXPOWER_0_19_24_DBM			0x02
#define E18_MS1_IPX_TXPOWER_2D5_20_26_DBM		0x03
#define E18_MS1_IPX_TXPOWER_4D5_20_27_DBM		0x04	//Ĭ��

//����ͨ�Ų�����
#define E18_MS1_IPX_BAUD_2400					0x00
#define E18_MS1_IPX_BAUD_4800					0x01
#define E18_MS1_IPX_BAUD_9600					0x02
#define E18_MS1_IPX_BAUD_14400					0x03
#define E18_MS1_IPX_BAUD_19200					0x04
#define E18_MS1_IPX_BAUD_38400					0x05
#define E18_MS1_IPX_BAUD_43000					0x06
#define E18_MS1_IPX_BAUD_57600					0x07
#define E18_MS1_IPX_BAUD_76800					0x08
#define E18_MS1_IPX_BAUD_115200					0x09	//Ĭ��
#define E18_MS1_IPX_BAUD_128000					0x0A
#define E18_MS1_IPX_BAUD_230400					0x0B
#define E18_MS1_IPX_BAUD_256000					0x0C
#define E18_MS1_IPX_BAUD_460800					0x0D
#define E18_MS1_IPX_BAUD_921600					0x0E
#define E18_MS1_IPX_BAUD_1000000				0x0F

//����״̬�ر�(�ն���Ч)
#define E18_MS1_IPX_SLEEP_TIME_CLOSE			0

//���������ʱ, �������豸����
#define E18_MS1_IPX_JOIN_CYCLE_NO_SLEEP			0
/*---------------------------------------------------------------------------*/

typedef struct {
 	uint8_t dev_type;					//�豸����
 	uint8_t nwk_state;					//����״̬
 	uint8_t pan_id[2];					//���� PAN_ID
 	uint8_t nwk_key[16];				//�����ܳ�
 	uint8_t local_short_addr[2];		//��������̵�ַ
 	uint8_t local_mac_addr[8];			//���� MAC ��ַ
 	uint8_t coor_short_addr[2];			//���ڵ�����̵�ַ
 	uint8_t coor_mac_addr[8];			//���ڵ� MAC ��ַ
 	uint8_t group;						//�������
 	uint8_t channel;					//ͨ���ŵ�
 	uint8_t txpower;					//���͹���
 	uint8_t baud;						//���ڲ�����
 	uint8_t sleep_time;					//����ʱ��(�ն˽ڵ���Ч)

 	uint8_t data_save_time;				//�ýڵ����ݱ���ʱ��(·������Э������Ч)
 	uint8_t join_cycle_join_cnt[2];		//�����������ڣ�����(�ն˽ڵ���Ч)
 	uint8_t dev_work_mode;				//�豸����ģʽ
 }e18_ms1_ipx_t;

uint8_t e18_ms1_ipx_init(void);
void e18_ms1_ipx_protocol_handle(uint8_t *pdata, uint16_t size);

#ifdef __cplusplus
}
#endif


#endif /* __E18_MS1_IPX_H__ */

