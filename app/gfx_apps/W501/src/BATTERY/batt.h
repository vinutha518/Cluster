/* **********************************************************************************************************************
File                    :       batt.h
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */


extern void Battery_Condition(void);

extern unsigned char LVSD_flag;
extern unsigned char HVSD_flag;
extern uint8_t LVSD_flag_1;
extern uint8_t HVSD_flag_1;
/*For Network Monitoring*/
extern _Bool LVSD_CAN;
extern _Bool HVSD_CAN;
