#include "softtimer.h"

static struct list g_head;

/**
 *@brief 软件定时器模块初始化
 */
void timer_init(void){
	list_init(&g_head);
}

/**
 *@brief 添加定时器并进行配置
 *@param timer: 定时器结构体指针
 *@param tick: 定时时间(定时时间=tick*硬件定时器定时周期)
 *@param cb: 定时到回调函数
 *@param repeat: 定时器是否自动重装计时
 *@retval none
 */
void timer_arm(softtimer *timer, u32_t tick, ontick_cb cb, boolean repeat){
	timer->m_tick = tick;
	timer->m_backup = tick;	
	timer->m_ontick = cb;
	
	timer->m_flags = 0x0000;
	if(repeat) TIMER_SET_BITS(timer->m_flags, Timer_Flag_Mask_Repeat);
	
	list_insert_tail(&g_head, &(timer->m_node));
}

/**
 *@brief 删除定时器
 *@param timer: 定时器结构体指针
 *@retval none
 */
void timer_disarm(softtimer *timer){
	timer->m_flags = 0x0000;	
	list_remove(&g_head, &(timer->m_node));
}

/**
 *@brief 启动定时器开始计时
 *@param timer: 定时器结构体指针
 *@retval none
 */
void timer_start(softtimer *timer){
	TIMER_SET_BITS(timer->m_flags, Timer_Flag_Mask_Enable);
}

/**
 *@brief 停止定时器
 *@param timer: 定时器结构体指针
 *@retval none
 */
void timer_stop(softtimer *timer){
	TIMER_RESET_BITS(timer->m_flags, Timer_Flag_Mask_Enable);
	timer->m_tick = timer->m_backup;
}

/**
 *@brief 执行减一操作, 必须放在硬件定时器中断中
 *@param none
 *@retval none
 */
void timer_exec(void){
	softtimer *ptimer;
	struct node *pnode = g_head.m_node.m_next;
	while(pnode != &(g_head.m_node)){
		ptimer = LIST_ENTRY(softtimer, m_node, pnode);
		if(TIMER_BITS(ptimer->m_flags, Timer_Flag_Mask_Enable)){
			if(ptimer->m_tick > 0){
				ptimer->m_tick--;
			}
			if(ptimer->m_tick == 0){
				TIMER_SET_BITS(ptimer->m_flags, Timer_Flag_Mask_Expired);
			}
		}
		pnode = pnode->m_next;
	}
}

/**
 *@brief 在背景函数中周期遍历定时器链表查询定时器是否到期, 并触发到期定时器的回调函数
 *@param none
 *@retval none
 */
void timer_service(void){
	softtimer *ptimer;
	struct node *pnode = g_head.m_node.m_next;
	while(pnode != &(g_head.m_node)){
		ptimer = LIST_ENTRY(softtimer, m_node, pnode);
		if(TIMER_BITS(ptimer->m_flags, Timer_Flag_Mask_Expired)){
			timer_stop(ptimer);
			TIMER_RESET_BITS(ptimer->m_flags, Timer_Flag_Mask_Expired);
			ptimer->m_ontick(ptimer);
			
			if(TIMER_BITS(ptimer->m_flags, Timer_Flag_Mask_Repeat)){
				ptimer->m_tick = ptimer->m_backup;
				timer_start(ptimer);
			}
		}
		pnode = pnode->m_next;
	}
}
