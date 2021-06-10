#ifndef __KEY_USER_H__
#define __KEY_USER_H__

#include "key.h"

#define KEY_LOCK_IN()		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)	//key_lock_in

u8_t key_init(void);
void key_gpio_config(void);
void key_lock_press_cb(void);
void key_lock_long_cb(void);
void key_lock_release_cb(void);
void key_lock_retrig_cb(void);

#endif
