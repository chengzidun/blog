#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

#include "list.h"

struct _softtimer;

typedef void (*ontick_cb) (struct _softtimer *timer);

typedef struct _softtimer{
	u32_t m_tick;
	u32_t m_backup;
	u16_t m_flags;
	ontick_cb m_ontick;
	struct node m_node;
}softtimer;

typedef enum {
	Timer_Flag_Mask_Enable = 0x0001,
	Timer_Flag_Mask_Repeat = 0x0002,
	Timer_Flag_Mask_Expired = 0x0004
}mask_bits;

#define TIMER_SET_BITS(flags, mask)	 flags |= mask
#define TIMER_RESET_BITS(flags, mask) flags &= (~mask)
#define TIMER_BITS(flags, mask) (flags & mask)

/**
 *@brief 软件定时器模块初始化
 */
void timer_init(void);

/**
 *@brief 添加定时器并进行配置
 *@param timer: 定时器结构体指针
 *@param tick: 定时时间(定时时间=tick*硬件定时器定时周期)
 *@param cb: 定时到回调函数
 *@param repeat: 定时器是否自动重装计时
 *@retval none
 */
void timer_arm(softtimer *timer, u32_t tick, ontick_cb cb, boolean repeat);

/**
 *@brief 删除定时器
 *@param timer: 定时器结构体指针
 *@retval none
 */
void timer_disarm(softtimer *timer);

/**
 *@brief 启动定时器开始计时
 *@param timer: 定时器结构体指针
 *@retval none
 */
void timer_start(softtimer *timer);

/**
 *@brief 停止定时器
 *@param timer: 定时器结构体指针
 *@retval none
 */
void timer_stop(softtimer *timer);

/**
 *@brief 执行减一操作, 必须放在硬件定时器中断中
 *@param none
 *@retval none
 */
void timer_exec(void);

/**
 *@brief 在背景函数中周期遍历定时器链表查询定时器是否到期, 并触发到期定时器的回调函数
 *@param none
 *@retval none
 */
void timer_service(void);

#endif
