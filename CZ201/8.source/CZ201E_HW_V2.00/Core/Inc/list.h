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
	*@brief ��ȡ�����ṹ�������ָ��
	*@param type: �����ṹ������
	*@param member: �����ṹ���truct node���͵ĳ�Ա������
	*@param mptr: ָ��member��ָ��
	*/
#define LIST_ENTRY(type, member, mptr) (type*)((u32_t)mptr - LIST_OFFSET(type, member))

void list_init(struct list *lst);
void list_insert_front(struct list *head, struct node *newnode);
void list_insert_tail(struct list *head, struct node *newnode);
boolean list_remove(struct list *head, struct node *rmnode);
boolean list_empty(struct list *head);
u32_t list_length(struct list *head);

#endif
