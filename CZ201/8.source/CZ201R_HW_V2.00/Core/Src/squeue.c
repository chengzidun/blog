#include "squeue.h"


/**
 *@brief 初始化队列(为队列分配内存)
 *@param q: 队列指针
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
 *@brief 将元素插入队列尾部
 *@param q: 队列指针
 *@param e: 元素
 *@retval true: 插入成功, false: 插入失败
 */
boolean enqueue(sqqueue *q, ElemType e)
{
  //队列满
  if((q->m_rear + 1) % MAXQSIZE == q->m_front) return _False;

  q->m_base[q->m_rear] = e;
  q->m_rear = (q->m_rear + 1) % MAXQSIZE;
  return _True;
}

/**
 *@brief 将元素从队列中取出(会删除该元素)
 *@param q: 队列指针
 *@param eout: 元素赋值地址
 *@retval true: 成功, false: 失败
 */
boolean dequeue(sqqueue *q, ElemType *eout)
{
  //队列空
  if(q->m_front == q->m_rear) return _False;

  *eout = q->m_base[q->m_front];
  q->m_front = (q->m_front + 1) % MAXQSIZE;
  return _True;
}

/**
 *@brief 根据索引读取元素(此方法不会删除元素)
 *@param q: 队列指针
 *@param index: 元素索引
 *@param eout: 元素赋值地址
 *@retval true: 成功, false: 失败
 */
boolean get(sqqueue *q, u32_t index, ElemType *eout)
{
  //队列空
  if(q->m_front == q->m_rear) return _False;

  if(index >= length(q)) return _False;

  *eout = q->m_base[(q->m_front + index) % MAXQSIZE];
  return _True;
}

/**
 *@brief 获取队列中元素个数
 *@param q: 队列指针
 *@retval 元素个数
 */
u32_t length(sqqueue *q)
{
  return (q->m_rear - q->m_front + MAXQSIZE) % MAXQSIZE;
}

/**
 *@brief 清空队列
 *@param q: 队列指针
 *@retval none
 */
void clear_queue(sqqueue *q)
{
  memset(q->m_base, 0, MAXQSIZE * sizeof(ElemType));
  q->m_front = 0;
  q->m_rear = 0;
}

/**
 *@brief 从队列头开始查询是否匹配的子串
 *@param q: 队列指针
 *@param buf: 子串缓冲区地址
 *@param len: 子串长度
 *@retval -1: 子串长度大于队列中元素的个数,  0: 找不到匹配的子串,  1: 找到匹配的子串
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
