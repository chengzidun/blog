#ifndef __SQUEUE_H__
#define __SQUEUE_H__
#include "global.h"
#include "string.h"

#define MAXQSIZE   64

typedef  u8_t ElemType;

typedef struct _sqqueue{
  ElemType *m_base;
  u16_t m_front;
  u16_t m_rear;
}sqqueue;

/**
 *@brief ��ʼ������(Ϊ���з����ڴ�)
 *@param q: ����ָ��
 *@retval none
 */
void init_queue(sqqueue *q);

/**
 *@brief ��Ԫ�ز������β��
 *@param q: ����ָ��
 *@param e: Ԫ��
 *@retval true: ����ɹ�, false: ����ʧ��
 */
boolean enqueue(sqqueue *q, ElemType e);

/**
 *@brief ��Ԫ�شӶ�����ȡ��(��ɾ����Ԫ��)
 *@param q: ����ָ��
 *@param eout: Ԫ�ظ�ֵ��ַ
 *@retval true: �ɹ�, false: ʧ��
 */
boolean dequeue(sqqueue *q, ElemType *eout);

/**
 *@brief ����������ȡԪ��(�˷�������ɾ��Ԫ��)
 *@param q: ����ָ��
 *@param index: Ԫ������
 *@param eout: Ԫ�ظ�ֵ��ַ
 *@retval true: �ɹ�, false: ʧ��
 */
boolean get(sqqueue *q, u32_t index, ElemType *eout);

/**
 *@brief ��ȡ������Ԫ�ظ���
 *@param q: ����ָ��
 *@retval Ԫ�ظ���
 */
u32_t length(sqqueue *q);

/**
 *@brief ��ն���
 *@param q: ����ָ��
 *@retval none
 */
void clear_queue(sqqueue *q);

/**
 *@brief �Ӷ���ͷ��ʼ��ѯ�Ƿ�ƥ����Ӵ�
 *@param q: ����ָ��
 *@param buf: �Ӵ���������ַ
 *@param len: �Ӵ�����
 *@retval -1: �Ӵ����ȴ��ڶ�����Ԫ�صĸ���,  0: �Ҳ���ƥ����Ӵ�,  1: �ҵ�ƥ����Ӵ�
 */
s8_t match(sqqueue *q, u8_t *buf, u16_t len);

#endif
