
#include "dr7f701461.dvf.h"

#define SFMA_CLK 		PORT_ISOP21_0
#define SFMA_CS 		PORT_ISOP21_1
#define SFMA_DATA0	 	PORT_ISOP21_2
#define SFMA_DATA1	 	PORT_ISOP21_3
#define SFMA_DATA2	 	PORT_ISOP21_4
#define SFMA_DATA3		PORT_ISOP21_5
#define SFMA_DATA4	 	PORT_ISOP21_6
#define SFMA_DATA5		PORT_ISOP21_7
#define SFMA_DATA6		PORT_ISOP21_8
#define SFMA_DATA7	 	PORT_ISOP21_9


//#define TURN_LEFT 		PORT_ISOP17_5
#define BT_ASSO 		PORT_ISOP17_5

#define TURN_RIGHT 		PORT_ISOP17_1
#define HIGH_BEAM	 	PORT_ISOP16_10
#define SERV_ICON	 	PORT_ISOP17_2
#define LOW_BATT	 	PORT_ISOP17_4
#define MAL_FN		 	PORT_ISOP16_11
#define NEUTRAL_IND	 	PORT_ISOP17_6
#define ABS_ON_OFF		PORT_ISOP17_7

//#define BT_ASSO		 	PORT_ISOP17_0
#define TURN_LEFT		 	PORT_ISOP17_0

#define E_OIL_TEMP	 	PORT_ISOP17_3
#define E_COOL_TEMP	 	PORT_ISOP17_9
#define E_OIL_PRESS	 	PORT_ISOP17_10

#define ON	1
#define OFF	0

#define TURN_LEFT_TOGGLE 		PORT_ISOPNOT17_0
#define TURN_RIGHT_TOGGLE 		PORT_ISOPNOT17_1


#define TURN_RIGHT_SENSE	PORT_ISOPPR16_6

void my_tell_tale_init(void);
void my_tell_tale_update(void);
void my_update_flasher(void);

