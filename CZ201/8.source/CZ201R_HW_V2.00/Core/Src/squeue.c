#include "squeue.h"


/**
 *@brief ��ʼ������(Ϊ���з����ڴ�)
 *@param q: ����ָ��
 *@retval none
 */
void init_queue(sqqueue *q)
{
  static ElemType e[MAXQSIZE];
  memset(e, 0, MAXQSIZE * sizeof(ElemType));

  q->m_base = e;
  q->m_front = 0;
  q->m_rear = 0;
}

/**
 *@brief ��Ԫ�ز������β��
 *@param q: ����ָ��
 *@param e: Ԫ��
 *@retval true: ����ɹ�, false: ����ʧ��
 */
boolean enqueue(sqqueue *q, ElemType e)
{
  //������
  if((q->m_rear + 1) % MAXQSIZE == q->m_front) return _False;

  q->m_base[q->m_rear] = e;
  q->m_rear = (q->m_rear + 1) % MAXQSIZE;
  return _True;
}

/**
 *@brief ��Ԫ�شӶ�����ȡ��(��ɾ����Ԫ��)
 *@param q: ����ָ��
 *@param eout: Ԫ�ظ�ֵ��ַ
 *@retval true: �ɹ�, false: ʧ��
 */
boolean dequeue(sqqueue *q, ElemType *eout)
{
  //���п�
  if(q->m_front == q->m_rear) return _False;

  *eout = q->m_base[q->m_front];
  q->m_front = (q->m_front + 1) % MAXQSIZE;
  return _True;
}

/**
 *@brief ����������ȡԪ��(�˷�������ɾ��Ԫ��)
 *@param q: ����ָ��
 *@param index: Ԫ������
 *@param eout: Ԫ�ظ�ֵ��ַ
 *@retval true: �ɹ�, false: ʧ��
 */
boolean get(sqqueue *q, u32_t index, ElemType *eout)
{
  //���п�
  if(q->m_front == q->m_rear) return _False;

  if(index >= length(q)) return _False;

  *eout = q->m_base[(q->m_front + index) % MAXQSIZE];
  return _True;
}

/**
 *@brief ��ȡ������Ԫ�ظ���
 *@param q: ����ָ��
 *@retval Ԫ�ظ���
 */
u32_t length(sqqueue *q)
{
  return (q->m_rear - q->m_front + MAXQSIZE) % MAXQSIZE;
}

/**
 *@brief ��ն���
 *@param q: ����ָ��
 *@retval none
 */
void clear_queue(sqqueue *q)
{
  memset(q->m_base, 0, MAXQSIZE * sizeof(ElemType));
  q->m_front = 0;
  q->m_rear = 0;
}

/**
 *@brief �Ӷ���ͷ��ʼ��ѯ�Ƿ�ƥ����Ӵ�
 *@param q: ����ָ��
 *@param buf: �Ӵ���������ַ
 *@param len: �Ӵ�����
 *@retval -1: �Ӵ����ȴ��ڶ�����Ԫ�صĸ���,  0: �Ҳ���ƥ����Ӵ�,  1: �ҵ�ƥ����Ӵ�
 */
s8_t match(sqqueue *q, u8_t *buf, u16_t len){
    u16_t i;
    u8_t e;

    for(i = 0;i < len;i++){
        if(get(q, i, &e) != _True) return -1;
        if(buf[i] != e) return 0;
    }

    return 1;
}
