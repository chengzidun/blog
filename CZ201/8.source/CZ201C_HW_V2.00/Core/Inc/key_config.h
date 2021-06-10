#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__

#include "stm32f0xx_hal.h"
#include "global.h"

void key_gpio_config(void);

/**
 * 配置具体硬件
 */

//进行一些基本的初始化工作如GPIO的配置等
#define KEY_CONFIG()  	key_gpio_config()

/**
 * 必须保证逻辑: 按下返回1, 抬起返回0, 不需要的按键则填0
 */
#define GET_KEY0()		(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))	//key_lock_in

/**
 * 配置参数
 */

//不需要的回调函数类型可以关掉以节省RAM空间
#define KEY_PRESS_CALLBACK_EN			1		//使能触发按下时的回调函数
#define KEY_LONG_CALLBACK_EN			1		//使能触发长按的回调函数
#define KEY_UP_CALLBACK_EN				1 		//使能触发抬起的回调函数
#define KEY_RETRIG_CALLBACK_EN			1		//使能触发连发的回调函数
#define KEY_CALLBACK_NUM				4		//每个按键关心的回调函数数量
 
#define KEY_LONG_PERIOD					1000	//长按的时间=KEY_LONG_PERIOD * Ttmr (Ttmr为定时器产生一次中断的时间)
#define KEY_RETRIG_PERIOD				100		//连发的间隔时间=KEY_RETRIG_PERIOD * Ttmr
#define KEY_MAX							1		//支持的按键数量

#endif
