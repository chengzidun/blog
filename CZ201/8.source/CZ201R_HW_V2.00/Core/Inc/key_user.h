#ifndef __KEY_USER_H__
#define __KEY_USER_H__

#include "key.h"

#define KEY_LOCK_IN()		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)	//key_lock
#define KEY_LIGHT_IN()		HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)	//key_light

u8_t key_init(void);
void key_gpio_config(void);
void key_lock_press_cb(void);
void key_lock_long_cb(void);
void key_lock_release_cb(void);
void key_lock_retrig_cb(void);
void key_light_press_cb(void);
void key_light_long_cb(void);
void key_light_release_cb(void);
void key_light_retrig_cb(void);

#endif
