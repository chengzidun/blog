#include "key.h"

static KEY_t *gKeyTblPtr;		//记录按键列表地址
static u8_t gKeyCurStatStep;	//记录当前步进状态
static u8_t gKeyLastValue;		//记录按键抬起前的值
static u16_t gKeyTimeCtr;		//用于长按，连发的计时
static boolean init_ok = _False;//先初始key_init()其它函数才能执行

/**
 * @brief 加载按键列表
 * @param pKeyTbl: 按键列表地址
 * @param callBack: 各按键对应的回调函数二维数组
 * @NOTE 每一维的顺序:短按回调函数,长按回调函数,抬起回调函数,连发回调函数.
 * @NOTE 不需要哪个回调函数则必须在该位置写0
 * @NOTE 完全禁止某一类型的回调函数则应在key_config.h配置相应的宏
 *			(KEY_PRESS_CALLBACK_EN, KEY_LONG_CALLBACK_EN, KEY_UP_CALLBACK_EN, KEY_RETRIG_CALLBACK_EN)
 */
void load_key_tbl(KEY_t *pKeyTbl, const KeyEventCallback callBack[][KEY_CALLBACK_NUM])
{
	u8_t i;
	u8_t cb;
	
	gKeyTblPtr = pKeyTbl;
	gKeyCurStatStep = KEY_STATE_STEP_INIT;
	gKeyLastValue = KEY_NULL;
	gKeyTimeCtr = 0;
	
	KEY_CONFIG();
	
	for(i = 0;i < KEY_MAX;i++){
		cb = 0;
		
#if KEY_PRESS_CALLBACK_EN > 0
		gKeyTblPtr[i].mPressCallback = callBack[i][cb++];
#endif

#if KEY_LONG_CALLBACK_EN > 0
		gKeyTblPtr[i].mLongCallback = callBack[i][cb++];
#endif

#if KEY_UP_CALLBACK_EN > 0
		gKeyTblPtr[i].mUpCallback = callBack[i][cb++];
#endif
		
#if KEY_RETRIG_CALLBACK_EN > 0
		gKeyTblPtr[i].mReTrigCallback = callBack[i][cb];
#endif

		gKeyTblPtr[i].mFlags = 0;
	}

	init_ok = _True;
}

/**
 * @brief 按键扫描辅助函数
 */
static u8_t key_scan_fn()
{
	u8_t keyVal = 0;
	
#if KEY_MAX > 0
	keyVal |= (u8_t)(GET_KEY0());
#endif
	
#if KEY_MAX > 1
	keyVal |= (u8_t)((GET_KEY1()) << 1);
#endif
	
#if KEY_MAX > 2
	keyVal |= (u8_t)((GET_KEY2()) << 2);
#endif
	
#if KEY_MAX > 3
	keyVal |= (u8_t)((GET_KEY3()) << 3);
#endif
	
	return keyVal;
}

/**
 * @brief 以状态转移的方式处理按键扫描
 * @NOTE  该函数应放在定时器中断中
 */
void key_scan(void)
{
	u8_t keyVal;

	if(init_ok != _True) return;

	keyVal = key_scan_fn();
	
	switch(gKeyCurStatStep){
		case KEY_STATE_STEP_INIT:{//初始步进状态
			//若此时有按键按下则切换到消抖状态
			if(KEY_NULL != keyVal) gKeyCurStatStep = KEY_STATE_STEP_WOBBLE;
		}break;
		
		case KEY_STATE_STEP_WOBBLE:{//消抖状态
			gKeyCurStatStep = KEY_STATE_STEP_PRESS;//切换到下一步进状态
		}break;
		
		case KEY_STATE_STEP_PRESS:{//按下状态
			if(KEY_NULL != keyVal){
			#if KEY_PRESS_CALLBACK_EN > 0
				u8_t i;
				//根据按键值置位相应按键对象的mFlags成员的KEY_DOWN位
				for(i = 0;i < KEY_MAX;i++){
					if(keyVal & (1 << i))SET_BITS(gKeyTblPtr[i].mFlags, KEY_DOWN);
				}
			#endif
				gKeyLastValue = keyVal;//并保存按键值以便KEY_STATE_STEP_RELEASE状态步下可以处理KEY_UP位
				gKeyCurStatStep = KEY_STATE_STEP_LONG;//切换到下一步进状态
			}else{
				gKeyCurStatStep = KEY_STATE_STEP_INIT;
			}
		}break;
		
		case KEY_STATE_STEP_LONG:{//长按步进状态
			if(KEY_NULL != keyVal){
				if(++gKeyTimeCtr > KEY_LONG_PERIOD){//判断长按的时间到
				#if KEY_LONG_CALLBACK_EN > 0
					u8_t i;
					//根据按键值置位按键对象的mFlags成员的KEY_LONG位
					for(i = 0;i < KEY_MAX;i++){
						if(keyVal & (1 << i))SET_BITS(gKeyTblPtr[i].mFlags, KEY_LONG);
					}
				#endif
					gKeyCurStatStep = KEY_STATE_STEP_RETRIG;//切换到下一步进状态
					gKeyTimeCtr = 0;
				}
			}else{
				gKeyCurStatStep = KEY_STATE_STEP_RELEASE;
			}
		}break;
		
		case KEY_STATE_STEP_RETRIG:{//连发状态
			if(KEY_NULL != keyVal){
				if(++gKeyTimeCtr > KEY_RETRIG_PERIOD){//连发间隔到
				#if KEY_RETRIG_CALLBACK_EN > 0
					u8_t i;
					//根据按键值置位按键对象的mFlags成员的KEY_RETRIG位
					for(i = 0;i < KEY_MAX;i++){
						if(keyVal & (1 << i))SET_BITS(gKeyTblPtr[i].mFlags, KEY_RETRIG);
					}
				#endif
					gKeyTimeCtr = 0;
				}
			}else{
				gKeyCurStatStep = KEY_STATE_STEP_RELEASE;
			}
		}break;
		
		case KEY_STATE_STEP_RELEASE:{//按键释放状态
			u8_t i;
			for(i = 0;i < KEY_MAX;i++){
				if(gKeyLastValue & (1 << i)){//根据按键值清空除KEY_UP外的所有标志位, 并置KEY_UP位
					gKeyTblPtr[i].mFlags = 0;
					SET_BITS(gKeyTblPtr[i].mFlags, KEY_UP);
				}
			}
			gKeyLastValue = KEY_NULL;//清0
			gKeyCurStatStep = KEY_STATE_STEP_INIT;//回到初始状态
			gKeyTimeCtr = 0;
		}break;
	}
}

/**
 * @brief 检查各按键的标志位, 满足则触发相应的回调函数. 
 * @NOTE  此函数应放在后台程序中, 若后台程序中其他代码执行较耗时则应将此函数放在定时器中断中
 */
void key_handler(void)
{
	u8_t i;
	u8_t flags;
	
	if(init_ok != _True) return;

	for(i = 0;i < KEY_MAX;i++){
		flags = gKeyTblPtr[i].mFlags;
		//按键按下时回调
		if(flags & KEY_DOWN){
		#if KEY_PRESS_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mPressCallback) gKeyTblPtr[i].mPressCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_DOWN);//clear bit
		}
		//长按时回调
		if(flags & KEY_LONG){
		#if KEY_LONG_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mLongCallback) gKeyTblPtr[i].mLongCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_LONG);//clear bit
		}
		//长按状态每隔一段时间自动重复触发
		if(flags & KEY_RETRIG){
		#if KEY_RETRIG_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mReTrigCallback) gKeyTblPtr[i].mReTrigCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_RETRIG);//clear bit
		}
		//按键抬起时回调
		if(flags & KEY_UP){
		#if KEY_UP_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mUpCallback) gKeyTblPtr[i].mUpCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_UP);//clear bit
		}
	}
}
