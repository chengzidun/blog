#ifndef __LIST_H__
#define __LIST_H__

#include "global.h"

struct node{
	struct node *m_prev;
	struct node *m_next;
};

struct list{
	struct node m_node;
};


#define LIST_OFFSET(type, member)  ((u32_t)&(((type*)0)->member))
/**
	*@brief 获取最外层结构体变量的指针
	*@param type: 最外层结构体类型
	*@param member: 最外层结构体的truct node类型的成员的名称
	*@param mptr: 指向member的指针
	*/
#define LIST_ENTRY(type, member, mptr) (type*)((u32_t)mptr - LIST_OFFSET(type, member))

void list_init(struct list *lst);
void list_insert_front(struct list *head, struct node *newnode);
void list_insert_tail(struct list *head, struct node *newnode);
boolean list_remove(struct list *head, struct node *rmnode);
boolean list_empty(struct list *head);
u32_t list_length(struct list *head);

#endif
