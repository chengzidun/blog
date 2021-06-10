#include "softtimer.h"

static struct list g_head;

/**
 *@brief �����ʱ��ģ���ʼ��
 */
void timer_init(void){
	list_init(&g_head);
}

/**
 *@brief ��Ӷ�ʱ������������
 *@param timer: ��ʱ���ṹ��ָ��
 *@param tick: ��ʱʱ��(��ʱʱ��=tick*Ӳ����ʱ����ʱ����)
 *@param cb: ��ʱ���ص�����
 *@param repeat: ��ʱ���Ƿ��Զ���װ��ʱ
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
 *@brief ɾ����ʱ��
 *@param timer: ��ʱ���ṹ��ָ��
 *@retval none
 */
void timer_disarm(softtimer *timer){
	timer->m_flags = 0x0000;	
	list_remove(&g_head, &(timer->m_node));
}

/**
 *@brief ������ʱ����ʼ��ʱ
 *@param timer: ��ʱ���ṹ��ָ��
 *@retval none
 */
void timer_start(softtimer *timer){
	TIMER_SET_BITS(timer->m_flags, Timer_Flag_Mask_Enable);
}

/**
 *@brief ֹͣ��ʱ��
 *@param timer: ��ʱ���ṹ��ָ��
 *@retval none
 */
void timer_stop(softtimer *timer){
	TIMER_RESET_BITS(timer->m_flags, Timer_Flag_Mask_Enable);
	timer->m_tick = timer->m_backup;
}

/**
 *@brief ִ�м�һ����, �������Ӳ����ʱ���ж���
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
 *@brief �ڱ������������ڱ�����ʱ�������ѯ��ʱ���Ƿ���, ���������ڶ�ʱ���Ļص�����
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
