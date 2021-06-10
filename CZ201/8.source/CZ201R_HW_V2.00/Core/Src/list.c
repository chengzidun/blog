#include "list.h"

void list_init(struct list *head){
	head->m_node.m_prev = &(head->m_node);
	head->m_node.m_next = &(head->m_node);
}

static void _list_insert(struct node *newnode, struct node *prev, struct node *next){
	next->m_prev = newnode;
	newnode->m_next = next;
	prev->m_next = newnode;
	newnode->m_prev = prev;
}

static void _list_remove(struct node *prev, struct node *next){
	next->m_prev = prev;
	prev->m_next = next;
}

void list_insert_front(struct list *head, struct node *newnode){
	_list_insert(newnode, &(head->m_node), head->m_node.m_next);
}

void list_insert_tail(struct list *head, struct node *newnode){
	_list_insert(newnode, head->m_node.m_prev, &(head->m_node));
}

boolean list_remove(struct list *head, struct node *rmnode){
	
	struct node *n = head->m_node.m_next;
	
	while((n != &(head->m_node))){
		if(n == rmnode){
			_list_remove(rmnode->m_prev, rmnode->m_next);
			return _True;
		}
		n = n->m_next;
	}
	
	return _False;
}

boolean list_empty(struct list *head){
	return (boolean)(head->m_node.m_prev == head->m_node.m_next);
}

u32_t list_length(struct list *head){
	u32_t i = 0;
	struct node *p = head->m_node.m_next;
	while((p != &(head->m_node))){
		i++;
		p = p->m_next;
	}
	
	return i;
}
