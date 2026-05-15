/* **********************************************************************************************************************
File                    :       swsc.h
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

*********************************************************************************************************************** */

#ifndef APP_GFX_APPS_W501_SRC_SWSC_SWSC_H_
#define APP_GFX_APPS_W501_SRC_SWSC_SWSC_H_

extern uint16_t SWSC_ADC_value; /* after adding this file in vlib folder need to delete this variable */

/* ********************************************************************************** */
/* Used for SWSC */
#define     ADC_COUNT_OPEN_KEY_ON    (2800)
#define		ADC_COUNT_UP_KEY_ON	     (1400)
#define		ADC_COUNT_UP_KEY_OFF     (1800)
#define		ADC_COUNT_DOWN_KEY_ON	 (1900)
#define		ADC_COUNT_DOWN_KEY_OFF	 (2300)
#define		ADC_COUNT_OK_KEY_ON	     (700)
#define		ADC_COUNT_OK_KEY_OFF     (1300)
#define		ADC_COUNT_MENU_KEY_ON    (2400)
#define		ADC_COUNT_MENU_KEY_OFF   (2700)
/* ********************************************************************************** */

extern void SWSC_Condition(void);
extern void SWSC_Mono_Condition(void);

extern uint16_t Counter_OK_KEY_LongPress_100usec;
extern uint16_t Counter_OK_KEY_100usec;
extern uint16_t Counter_UP_KEY_100usec;
extern uint16_t Counter_DOWN_KEY_100usec;
extern uint16_t Counter_MENU_KEY_100usec;
extern uint8_t OK_KEY_Flag;
extern uint8_t UP_KEY_Flag;
extern uint8_t DOWN_KEY_Flag;
extern uint8_t MENU_KEY_Flag;
extern uint8_t Up_key_ShortPress;//r
extern uint8_t Down_key_ShortPress;//r
extern uint8_t Menu_key_ShortPress;//r
extern uint8_t MODE_key_LongPress;
extern uint8_t MODE_key_ShortPress;
extern uint8_t SET_key_LongPress;
extern uint8_t SET_key_ShortPress;
extern uint8_t Ok_key_LongPress;
extern _Bool PAB_jump_driveinfo;

/*U350*/
extern uint32_t Counter_SET_KEY_LongPress_100usec;
extern uint32_t Counter_MODE_KEY_LongPress_100usec;
extern uint32_t Counter_SET_KEY_100usec;
extern uint32_t Counter_MODE_KEY_100usec;
extern uint8_t SET_KEY_Flag;
extern uint8_t MODE_KEY_Flag;
extern uint8_t MODE_key_press;
extern uint8_t SET_key_press;
extern _Bool SET_5seckey_press;
extern uint8_t OK_pressed;
//extern _Bool ok_modelong_press;
extern _Bool Set_Key_stuck;
extern _Bool Mode_Key_stuck;
extern _Bool Up_key_released;
extern _Bool Menu_key_released;
extern _Bool Down_key_released;


extern uint8_t dont_throw_alerts_BS6_alert_on_screen_present;
extern _Bool avoid_trip_reset;
extern _Bool TPMS_ENTER_PR;
#endif /* APP_GFX_APPS_W501_SRC_SWSC_SWSC_H_ */
