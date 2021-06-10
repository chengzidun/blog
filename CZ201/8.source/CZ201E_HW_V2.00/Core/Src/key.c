#include "key.h"

static KEY_t *gKeyTblPtr;		//��¼�����б��ַ
static u8_t gKeyCurStatStep;	//��¼��ǰ����״̬
static u8_t gKeyLastValue;		//��¼����̧��ǰ��ֵ
static u16_t gKeyTimeCtr;		//���ڳ����������ļ�ʱ
static boolean init_ok = _False;//�ȳ�ʼkey_init()������������ִ��

/**
 * @brief ���ذ����б�
 * @param pKeyTbl: �����б��ַ
 * @param callBack: ��������Ӧ�Ļص�������ά����
 * @NOTE ÿһά��˳��:�̰��ص�����,�����ص�����,̧��ص�����,�����ص�����.
 * @NOTE ����Ҫ�ĸ��ص�����������ڸ�λ��д0
 * @NOTE ��ȫ��ֹĳһ���͵Ļص�������Ӧ��key_config.h������Ӧ�ĺ�
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
 * @brief ����ɨ�踨������
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
 * @brief ��״̬ת�Ƶķ�ʽ������ɨ��
 * @NOTE  �ú���Ӧ���ڶ�ʱ���ж���
 */
void key_scan(void)
{
	u8_t keyVal;

	if(init_ok != _True) return;

	keyVal = key_scan_fn();
	
	switch(gKeyCurStatStep){
		case KEY_STATE_STEP_INIT:{//��ʼ����״̬
			//����ʱ�а����������л�������״̬
			if(KEY_NULL != keyVal) gKeyCurStatStep = KEY_STATE_STEP_WOBBLE;
		}break;
		
		case KEY_STATE_STEP_WOBBLE:{//����״̬
			gKeyCurStatStep = KEY_STATE_STEP_PRESS;//�л�����һ����״̬
		}break;
		
		case KEY_STATE_STEP_PRESS:{//����״̬
			if(KEY_NULL != keyVal){
			#if KEY_PRESS_CALLBACK_EN > 0
				u8_t i;
				//���ݰ���ֵ��λ��Ӧ���������mFlags��Ա��KEY_DOWNλ
				for(i = 0;i < KEY_MAX;i++){
					if(keyVal & (1 << i))SET_BITS(gKeyTblPtr[i].mFlags, KEY_DOWN);
				}
			#endif
				gKeyLastValue = keyVal;//�����水��ֵ�Ա�KEY_STATE_STEP_RELEASE״̬���¿��Դ���KEY_UPλ
				gKeyCurStatStep = KEY_STATE_STEP_LONG;//�л�����һ����״̬
			}else{
				gKeyCurStatStep = KEY_STATE_STEP_INIT;
			}
		}break;
		
		case KEY_STATE_STEP_LONG:{//��������״̬
			if(KEY_NULL != keyVal){
				if(++gKeyTimeCtr > KEY_LONG_PERIOD){//�жϳ�����ʱ�䵽
				#if KEY_LONG_CALLBACK_EN > 0
					u8_t i;
					//���ݰ���ֵ��λ���������mFlags��Ա��KEY_LONGλ
					for(i = 0;i < KEY_MAX;i++){
						if(keyVal & (1 << i))SET_BITS(gKeyTblPtr[i].mFlags, KEY_LONG);
					}
				#endif
					gKeyCurStatStep = KEY_STATE_STEP_RETRIG;//�л�����һ����״̬
					gKeyTimeCtr = 0;
				}
			}else{
				gKeyCurStatStep = KEY_STATE_STEP_RELEASE;
			}
		}break;
		
		case KEY_STATE_STEP_RETRIG:{//����״̬
			if(KEY_NULL != keyVal){
				if(++gKeyTimeCtr > KEY_RETRIG_PERIOD){//���������
				#if KEY_RETRIG_CALLBACK_EN > 0
					u8_t i;
					//���ݰ���ֵ��λ���������mFlags��Ա��KEY_RETRIGλ
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
		
		case KEY_STATE_STEP_RELEASE:{//�����ͷ�״̬
			u8_t i;
			for(i = 0;i < KEY_MAX;i++){
				if(gKeyLastValue & (1 << i)){//���ݰ���ֵ��ճ�KEY_UP������б�־λ, ����KEY_UPλ
					gKeyTblPtr[i].mFlags = 0;
					SET_BITS(gKeyTblPtr[i].mFlags, KEY_UP);
				}
			}
			gKeyLastValue = KEY_NULL;//��0
			gKeyCurStatStep = KEY_STATE_STEP_INIT;//�ص���ʼ״̬
			gKeyTimeCtr = 0;
		}break;
	}
}

/**
 * @brief ���������ı�־λ, �����򴥷���Ӧ�Ļص�����. 
 * @NOTE  �˺���Ӧ���ں�̨������, ����̨��������������ִ�нϺ�ʱ��Ӧ���˺������ڶ�ʱ���ж���
 */
void key_handler(void)
{
	u8_t i;
	u8_t flags;
	
	if(init_ok != _True) return;

	for(i = 0;i < KEY_MAX;i++){
		flags = gKeyTblPtr[i].mFlags;
		//��������ʱ�ص�
		if(flags & KEY_DOWN){
		#if KEY_PRESS_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mPressCallback) gKeyTblPtr[i].mPressCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_DOWN);//clear bit
		}
		//����ʱ�ص�
		if(flags & KEY_LONG){
		#if KEY_LONG_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mLongCallback) gKeyTblPtr[i].mLongCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_LONG);//clear bit
		}
		//����״̬ÿ��һ��ʱ���Զ��ظ�����
		if(flags & KEY_RETRIG){
		#if KEY_RETRIG_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mReTrigCallback) gKeyTblPtr[i].mReTrigCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_RETRIG);//clear bit
		}
		//����̧��ʱ�ص�
		if(flags & KEY_UP){
		#if KEY_UP_CALLBACK_EN > 0
			if(gKeyTblPtr[i].mUpCallback) gKeyTblPtr[i].mUpCallback();
		#endif
			CLR_BITS(gKeyTblPtr[i].mFlags, KEY_UP);//clear bit
		}
	}
}
