#include "VIN_MISMATCH.h"
#include "r_typedefs.h"
#include "UDSS.h"
//#include "r_cg_macrodriver.h"
//#include "r_cg_userdefine.h"
//#include "iodefine.h"
#include "my_can.h"
#include "UDSS_APP.h"


_Bool Wrong_Index = 0;
_Bool SYS_Fun_Flag =0;
_Bool Limp_home_mode =0;
_Bool Successful_Transmisson = 0;

uint8_t VIN_Index0_flag = 0;
uint8_t VIN_Index1_flag = 0;
uint8_t VIN_Index2_flag = 0;
uint8_t Vin_counter_ulg=0;

uint8_t  VIN_Data[17] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q'};
uint8_t  VIN_INDEX; 

uint8_t Data0[8];
uint8_t Cnt_on_valid_data = 0;
uint8_t VIN_Msg_Counter = 0;
uint8_t VIN_Data_Counter = 0;
uint8_t Failure_Count = 0;

uint8_t VIN_Mismatch(uint8_t *a);
extern DTC DTC_IVN_MISMATCH_D305_Buff;
void VIN_flg_check(void);
void Vin_Clear_Variables(void);
/***********************************************************************************************************************
* Function Name: VIN_Mismatch
* Description  : This function is cehck the VIN_Mismatch.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t VIN_Mismatch(uint8_t *VIN_Data_Received)//2 sec from ign on and after every 500ms
{	
   
	VIN_INDEX = VIN_Data_Received[7];	
	Cnt_on_valid_data = 0;
	switch (VIN_INDEX)
	{
		case 0:
				for( VIN_Msg_Counter=0;VIN_Msg_Counter<7;VIN_Msg_Counter++)
				{
					VIN_Data_Counter = VIN_Msg_Counter;
					 
				 	if(VIN_Data_Received[VIN_Msg_Counter] == VIN_arr[VIN_Data_Counter])
					{
						Cnt_on_valid_data++;
			 		}
					else
					{
					}
					if(Cnt_on_valid_data==7)
					{
						VIN_Index0_flag =1;
					}
				}
				break;
				
		case 1:
				for(VIN_Msg_Counter=0;VIN_Msg_Counter<7;VIN_Msg_Counter++)
			 	{		
					VIN_Data_Counter = VIN_Msg_Counter+7;
					 
					if(VIN_Data_Received[VIN_Msg_Counter] == VIN_arr[VIN_Data_Counter])
					{
						Cnt_on_valid_data++;
			 		}
					else
					{
					}
					if(Cnt_on_valid_data==7)
					{
						VIN_Index1_flag =1;
					}
				}
				break;
		
		case 2:
				for(VIN_Msg_Counter=0;VIN_Msg_Counter<3;VIN_Msg_Counter++)
				{
					VIN_Data_Counter = VIN_Msg_Counter+14;
					 
					if(VIN_Data_Received[VIN_Msg_Counter] == VIN_arr[VIN_Data_Counter])
					{
						Cnt_on_valid_data++;
			 		}
					else
					{
					}
					if(Cnt_on_valid_data==3)
					{
						VIN_Index2_flag =1;
					}
				}
				break;	
		
		default: 
				Wrong_Index = 1;	
				break;
	}
}

/***********************************************************************************************************************
* Function Name: VIN_flg_check
* Description  : This function is  the VIN_Mismatch Flag set.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void VIN_flg_check(void)
{    	
	Vin_counter_ulg++;			//Check no of attempt VIN_Mismatch called.
	if(Vin_counter_ulg%3 == 0)
	{
		if((VIN_Index0_flag==1) && (VIN_Index1_flag==1) && (VIN_Index2_flag==1))
		{
			Vin_counter_ulg = 10;
			SYS_Fun_Flag = 1;	//check flag
			Failure_Count = 0;
			VIN_Index0_flag = 0;
			VIN_Index1_flag = 0;
			VIN_Index2_flag = 0;
			if(DTC_IVN_MISMATCH_D305_Buff.BYTEs[3] == 0x09)
			{
				DTC_IVN_MISMATCH_D305_Buff.BYTEs[3] = 0x08;
			}
		}
		else
		{
			Failure_Count++;
		}				
		if(Failure_Count>=3)
		{
			DTC_IVN_MISMATCH_D305_Buff.BYTEs[3] = 0x09;
			Limp_home_mode = 1; //will set DTC
		}
	}
}

/***********************************************************************************************************************
* Function Name: Vin_Clear_Variables
* Description  : This function is  VIN_Mismatch Flag and variables clear.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

// This function has to be called before Ignition Off

void Vin_Clear_Variables(void)
{
	SYS_Fun_Flag = 0;
	VIN_Index0_flag = 0;
	VIN_Index1_flag = 0;
	VIN_Index2_flag = 0;
	Failure_Count = 0;
	Vin_counter_ulg = 0;
	Limp_home_mode = 0; 
}
