/*******************************************************************************
**   Private Macro Definitions                                                **
*******************************************************************************/

#define ON                  1U
#define OFF                 0U

#define FOURxFOUR_MIN       0x00U
#define FOURxFOUR_MAX       0x03U

#define TWO_WD              0U
#define FOUR_WDH            1U
#define FOUR_WDL            2U

#define TWO_WD_4WDL         3U
#define FOUR_WDL_2WD        4U
#define TWO_WD_4WDH         5U
#define FOUR_WDH_FOUR_WDL   6U
#define FOUR_WDL_4WDH       7U
#define FOUR_WDH_TWO_WD     8U
/*******************************************************************************
**  GLOBAL FUNCTION PROTOTYPES          PUBLIC FUNCTIONS AND VARIABLES		  **
*******************************************************************************/
/******************************************************************************
** Calibration data                                                           **
******************************************************************************/
extern uint8_t      		Previous_Alert_Condition_Transfer_Mode;

/*******************************************************************************
** Syntax           : void Main_Screen_Display_4WD( void )                     **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for actual condition check for    **
**                    Main screen  display.                                   **
**                    This function shall be called every 100ms               **
**                                                                            **
*******************************************************************************/
extern void Main_Screen_Display_4WD( void );

/*******************************************************************************
** Syntax           : void Alert_Condition_4WD_Meter( void )                  **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for actual condition check for    **
**                    Alert display.                                          **
**                    This function shall be called every 100ms               **
**                                                                            **
*******************************************************************************/
extern void Alert_Condition_4WD_Meter( void );

/*******************************************************************************
** Syntax           : void FourxFour_Alert_Main_Screen_Display_4WD_Config( void )         **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for actual condition check for    **
**                    Alert display.                                          **
**                    This function shall be called every 100ms               **
**                                                                            **
*******************************************************************************/
extern void FourxFour_Alert_Main_Screen_Display_4WD_Config( void );

/*******************************************************************************
** Syntax           : void FourxFour_Init (void)                              **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for Initialize                    **
**                                                                            **
*******************************************************************************/
extern void FourxFour_Init( void );

typedef struct {
    uint8_t     FourxFour_Main_Screen_Display_Mode;         /*FourxFour Main screen display value*/
    uint8_t     FourxFour_Alert_Condition_4WD;              /*FourxFour Alert screen display value*/
    _Bool       FourxFour_Main_Screen_Display_4WD_Valid;    /*When this flag is false, FourxFour Main Screen display should be inactivated*/
    _Bool       FourxFour_Alert_Condition_4WD_Valid;        /*When this flag is false, FourxFour Alert Screen display should be inactivated*/
    _Bool       Invalid_FourxFour_Msg;                      /*If value is out of range, this flag is set*/
} FourxFourY;

extern FourxFourY FxFY;
/* used for Alert */
extern uint8_t var_4x4_frameRate;
extern uint8_t var_4x4_2WD_visible;
extern uint8_t var_4x4_4WD_visible;
extern uint8_t var_4x4_4WD_animation;
/* Used for Main_Screen */
extern uint8_t VI_4WD_Framerate;
extern uint8_t VI_2WD_Visiblity;
extern uint8_t VI_4WD_Visiblity;