/* **********************************************************************************************************************
File                    :       gearIndication.h
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

*********************************************************************************************************************** */
extern _Bool Reversegear_ongoing;
extern _Bool Reverse_gear_active;
extern _Bool reverse_played;
extern void Current_Gear_and_Gear_Shift_Indication(void);
extern uint8_t IO_Control_Gear_indication(void);
extern uint8_t IO_Control_Gear_shift_indication(void);
