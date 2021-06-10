#ifndef __KEY_H__
#define __KEY_H__

#include "key_config.h"

//步进状态值
#define KEY_STATE_STEP_INIT			0x01
#define KEY_STATE_STEP_WOBBLE		0x02
#define KEY_STATE_STEP_PRESS		0x03
#define KEY_STATE_STEP_LONG			0x04
#define KEY_STATE_STEP_RETRIG		0x05
#define KEY_STATE_STEP_RELEASE		0x06

//按键状态值
#define KEY_DOWN					0x80
#define KEY_UP						0x40
#define KEY_LONG					0x20
#define KEY_RETRIG					0x10

#define SET_BITS(flag, mask)		(flag |= mask)
#define CLR_BITS(flag, mask)		(flag &= ~mask)

#define KEY_NULL					0x00	//空键值

// #define KEY_VALUE_K0	0x01
// #define KEY_VALUE_K1	0x02
// #define KEY_VALUE_K2	0x04
// #define KEY_VALUE_K3	0x08


typedef void (*KeyEventCallback) ();

typedef struct _key{
#if KEY_PRESS_CALLBACK_EN > 0
	KeyEventCallback mPressCallback;	//短按回调函数句柄
#endif
	
#if KEY_LONG_CALLBACK_EN > 0
	KeyEventCallback mLongCallback;		//长按回调函数句柄
#endif
	
#if KEY_RETRIG_CALLBACK_EN > 0
	KeyEventCallback mReTrigCallback;	//连发回调函数句柄
#endif
	
#if KEY_UP_CALLBACK_EN > 0
	KeyEventCallback mUpCallback;		//抬起回调函数句柄
#endif
	
	u8_t mFlags;						//KEY_DOWN, KEY_UP,KEY_LONG, KEY_RETRIG
}KEY_t;

void load_key_tbl(KEY_t *pKeyTbl, const KeyEventCallback callBack[][KEY_CALLBACK_NUM]);
void key_scan(void);
void key_handler(void);

#endif
