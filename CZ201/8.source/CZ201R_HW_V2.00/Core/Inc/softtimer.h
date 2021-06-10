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
 *@brief �����ʱ��ģ���ʼ��
 */
void timer_init(void);

/**
 *@brief ��Ӷ�ʱ������������
 *@param timer: ��ʱ���ṹ��ָ��
 *@param tick: ��ʱʱ��(��ʱʱ��=tick*Ӳ����ʱ����ʱ����)
 *@param cb: ��ʱ���ص�����
 *@param repeat: ��ʱ���Ƿ��Զ���װ��ʱ
 *@retval none
 */
void timer_arm(softtimer *timer, u32_t tick, ontick_cb cb, boolean repeat);

/**
 *@brief ɾ����ʱ��
 *@param timer: ��ʱ���ṹ��ָ��
 *@retval none
 */
void timer_disarm(softtimer *timer);

/**
 *@brief ������ʱ����ʼ��ʱ
 *@param timer: ��ʱ���ṹ��ָ��
 *@retval none
 */
void timer_start(softtimer *timer);

/**
 *@brief ֹͣ��ʱ��
 *@param timer: ��ʱ���ṹ��ָ��
 *@retval none
 */
void timer_stop(softtimer *timer);

/**
 *@brief ִ�м�һ����, �������Ӳ����ʱ���ж���
 *@param none
 *@retval none
 */
void timer_exec(void);

/**
 *@brief �ڱ������������ڱ�����ʱ�������ѯ��ʱ���Ƿ���, ���������ڶ�ʱ���Ļص�����
 *@param none
 *@retval none
 */
void timer_service(void);

#endif
