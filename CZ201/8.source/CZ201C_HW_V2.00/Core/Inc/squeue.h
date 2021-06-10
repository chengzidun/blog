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
 *@brief 初始化队列(为队列分配内存)
 *@param q: 队列指针
 *@retval none
 */
void init_queue(sqqueue *q);

/**
 *@brief 将元素插入队列尾部
 *@param q: 队列指针
 *@param e: 元素
 *@retval true: 插入成功, false: 插入失败
 */
boolean enqueue(sqqueue *q, ElemType e);

/**
 *@brief 将元素从队列中取出(会删除该元素)
 *@param q: 队列指针
 *@param eout: 元素赋值地址
 *@retval true: 成功, false: 失败
 */
boolean dequeue(sqqueue *q, ElemType *eout);

/**
 *@brief 根据索引读取元素(此方法不会删除元素)
 *@param q: 队列指针
 *@param index: 元素索引
 *@param eout: 元素赋值地址
 *@retval true: 成功, false: 失败
 */
boolean get(sqqueue *q, u32_t index, ElemType *eout);

/**
 *@brief 获取队列中元素个数
 *@param q: 队列指针
 *@retval 元素个数
 */
u32_t length(sqqueue *q);

/**
 *@brief 清空队列
 *@param q: 队列指针
 *@retval none
 */
void clear_queue(sqqueue *q);

/**
 *@brief 从队列头开始查询是否匹配的子串
 *@param q: 队列指针
 *@param buf: 子串缓冲区地址
 *@param len: 子串长度
 *@retval -1: 子串长度大于队列中元素的个数,  0: 找不到匹配的子串,  1: 找到匹配的子串
 */
s8_t match(sqqueue *q, u8_t *buf, u16_t len);

#endif
