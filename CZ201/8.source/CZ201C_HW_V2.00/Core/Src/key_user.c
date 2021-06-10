#include "key_user.h"
#include "stdio.h"
#include "usart.h"
#include "led.h"
#include "lock.h"

static KEY_t gKeyTbl[KEY_MAX];	//����Ҫ�İ���������

static const KeyEventCallback gCallback[KEY_MAX][KEY_CALLBACK_NUM] =
{
	//��һά�ǰ������������ڶ�ά��ÿ�������Ļص�����
	//���ĸ��ص�����������˳��Ϊ�̰���������̧�����������
	{ key_lock_press_cb, key_lock_long_cb, key_lock_release_cb, key_lock_retrig_cb },
};

void key_gpio_config(void)
{

}

u8_t key_init(void)
{
	load_key_tbl(gKeyTbl, gCallback);

	return 0;
}

//key_lock
__weak void key_lock_press_cb(void)
{
	lock_unlocking(500);
}

__weak void key_lock_long_cb(void)
{

}

__weak void key_lock_release_cb(void)
{

}

__weak void key_lock_retrig_cb(void)
{

}
