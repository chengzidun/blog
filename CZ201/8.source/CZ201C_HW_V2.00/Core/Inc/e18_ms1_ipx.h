/*
 * e18_ms1_ipx.h
 *
 *  Created on: 2019年1月8日
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
//设备类型
#define E18_MS1_IPX_DEV_TYPE_C					0x00
#define E18_MS1_IPX_DEV_TYPE_R					0x01
#define E18_MS1_IPX_DEV_TYPE_D					0x02

//网络状态
#define E18_MS1_IPX_NWK_STATE_NO				0x00
#define E18_MS1_IPX_NWK_STATE_OK				0x01

//随机网络, 0x0000~0xFFFE固定网络
#define E18_MS1_IPX_PAN_ID_RANDOM				0xFFFF

//协调器的网络短地址固定为0x0000
#define E18_MS1_IPX_DEV_C_FIXED_SHORT_ADDR		0x0000

//网络组号
#define E18_MS1_IPX_GROUP_MIN					1		//默认
#define E18_MS1_IPX_GROUP_MAX					99

//网络信道
#define E18_MS1_IPX_CHANNEL_MIN					11		//默认
#define E18_MS1_IPX_CHANNEL_MAX					26

//发送功率
#define E18_MS1_IPX_TXPOWER_N3_16_20_DBM		0x00
#define E18_MS1_IPX_TXPOWER_N1D5_17_22_DBM		0x01
#define E18_MS1_IPX_TXPOWER_0_19_24_DBM			0x02
#define E18_MS1_IPX_TXPOWER_2D5_20_26_DBM		0x03
#define E18_MS1_IPX_TXPOWER_4D5_20_27_DBM		0x04	//默认

//串口通信波特率
#define E18_MS1_IPX_BAUD_2400					0x00
#define E18_MS1_IPX_BAUD_4800					0x01
#define E18_MS1_IPX_BAUD_9600					0x02
#define E18_MS1_IPX_BAUD_14400					0x03
#define E18_MS1_IPX_BAUD_19200					0x04
#define E18_MS1_IPX_BAUD_38400					0x05
#define E18_MS1_IPX_BAUD_43000					0x06
#define E18_MS1_IPX_BAUD_57600					0x07
#define E18_MS1_IPX_BAUD_76800					0x08
#define E18_MS1_IPX_BAUD_115200					0x09	//默认
#define E18_MS1_IPX_BAUD_128000					0x0A
#define E18_MS1_IPX_BAUD_230400					0x0B
#define E18_MS1_IPX_BAUD_256000					0x0C
#define E18_MS1_IPX_BAUD_460800					0x0D
#define E18_MS1_IPX_BAUD_921600					0x0E
#define E18_MS1_IPX_BAUD_1000000				0x0F

//休眠状态关闭(终端有效)
#define E18_MS1_IPX_SLEEP_TIME_CLOSE			0

//无网络加入时, 不允许设备休眠
#define E18_MS1_IPX_JOIN_CYCLE_NO_SLEEP			0
/*---------------------------------------------------------------------------*/

typedef struct {
 	uint8_t dev_type;					//设备类型
 	uint8_t nwk_state;					//网络状态
 	uint8_t pan_id[2];					//网络 PAN_ID
 	uint8_t nwk_key[16];				//网络密匙
 	uint8_t local_short_addr[2];		//本地网络短地址
 	uint8_t local_mac_addr[8];			//本地 MAC 地址
 	uint8_t coor_short_addr[2];			//父节点网络短地址
 	uint8_t coor_mac_addr[8];			//父节点 MAC 地址
 	uint8_t group;						//网络组号
 	uint8_t channel;					//通信信道
 	uint8_t txpower;					//发送功率
 	uint8_t baud;						//串口波特率
 	uint8_t sleep_time;					//休眠时间(终端节点有效)

 	uint8_t data_save_time;				//该节点数据保存时间(路由器和协调器有效)
 	uint8_t join_cycle_join_cnt[2];		//入网休眠周期，次数(终端节点有效)
 	uint8_t dev_work_mode;				//设备工作模式
 }e18_ms1_ipx_t;

uint8_t e18_ms1_ipx_init(void);
void e18_ms1_ipx_protocol_handle(uint8_t *pdata, uint16_t size);

#ifdef __cplusplus
}
#endif


#endif /* __E18_MS1_IPX_H__ */

