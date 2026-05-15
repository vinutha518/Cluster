#include "r_typedefs.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"

#include "state_machine.h"
#include "scenes.h"
#include "main_scene.h"
#include "bench.h"

#include "r_tick_api.h"
#include "my_tell_tale.h"
#include "my_can.h"

typedef enum { 
    LOC_STATE_INIT,
    LOC_STATE_IDLE,
    LOC_STATE_APE,
    LOC_STATE_CONV,
    LOC_STATE_JPEG,
    LOC_STATE_MAIN
} loc_State_t;

static loc_State_t loc_State;

void FlashDash_Run(void)
{
	static uint32_t myCurrentTime = 0;
	static uint32_t myStartTime = 0;
	static uint32_t myElaspsedTime = 0;
	static uint32_t myTellTaleTime = 0;
    loc_State = LOC_STATE_INIT;
	
	//Init_MainScene(); /* Init all buffers (2 in this case) */
    //Init_MainScene(); /* Execute this twice */
	myStartTime = R_TICK_GetTimeMS(0);
	myTellTaleTime = R_TICK_GetTimeMS(0);
	
	FlashDash_OpeningScene();
	
	Init_Background();
	
    
    for (;;)
    {
		if((R_TICK_GetTimeMS(0) - myTellTaleTime) > 1500)
		{
		  my_tell_tale_update();
		  myTellTaleTime = R_TICK_GetTimeMS(0);
		}
		my_update_flasher();
		
		if(rx_flag)
		 {
			 rx_flag = 0;
			 my_can0_rx();
			
			// if(my_immo_val == 0x46)
			// {
				// my_lock = 1;			
			// }
			
		 }
		 Refresh_MainScene();
	}
}

