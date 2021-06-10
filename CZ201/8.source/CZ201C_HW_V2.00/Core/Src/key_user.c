#include "key_user.h"
#include "stdio.h"
#include "usart.h"
#include "led.h"
#include "lock.h"

static KEY_t gKeyTbl[KEY_MAX];	//你需要的按键最多个数

static const KeyEventCallback gCallback[KEY_MAX][KEY_CALLBACK_NUM] =
{
	//第一维是按键最多个数；第二维是每个按键的回调函数
	//共四个回调函数，依次顺序为短按、长按、抬起和连发函数
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
