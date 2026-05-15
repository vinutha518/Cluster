/*
 * r_cg_serial.c
 *
 *  Created on: Jan 28, 2019
 *      Author: MSWELT12
 */
/* **********************************************************************************************************************
File            :       r_cg_serial.c
Author          :
Model           :       Mahindra W501
Revision Author :
Reviewed By     :
Company         :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune
 * *********Includes************************************************************************************************** */

#include "dbgmacro.h"
#include "errmgr.h"
#include "dr7f701461.dvf.h"
#include "my_can.h"
#include "r_gpio_api.h"
#include "r_cg_timer.h"
#include "r_tick_api.h"
//#include "r_cg_serial.h"
#include "UDSS.h"
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "r_tick_api.h"
#include "r_cg_timer.h"
#include "r_gpio_api.h"
#include "r_cdi_api.h"
#include "r_config_riic.h"
#include "r_riic_regs.h"
#include "r_riic_api.h"
#include "r_riic_main.h"
#include "r_riic_sys.h"
#include "r_dev_api.h"
#include  "Memmory_Map.h"
//#include <string.h>
#include "r_typedefs.h"
#include "r_bsp_stdio_api.h"
#include <r_bsp_api.h>
#include "r_dev_api.h"
#include "r_riic_api.h"
#include "r_gpio_api.h"
#include "my_can.h"

#include "r_taub_api.h"

#define I2C_Time  10U        /* 80 */

#define NOF_BITS  8U

void delay_time(uint16_t time_end);
void IIC_Start_Condition(void);
void IIC_Stop_Condition (void);
uint8_t IIC_Master_Send(uint8_t output_data);
uint8_t IIC_Master_Receive(void);
void I2C_Release_Bus(void);
void Delay ( unsigned short   DelatT);
void E_Delay_i2c(void);


static const r_dev_PinConfig_t ICC_PinConfigSDA_in[] =
		/*   Port                 Pin              Func  Dir               Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
		{3, 0, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* IGN_SENSE */
		{0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}
};
static const r_dev_PinConfig_t ICC_PinConfigSDA_out[] =
		/*   Port                 Pin              Func  Dir               Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
		{3, 0, 0u, R_DEV_PIN_OUT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* IGN_SENSE */
		{0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}
};

/* *********Variables Defination************************************************************************ */

/* **********************************************************************************************************************
 * Function Name: I2C Init
 * Description  : This function Inializes I2C.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************** */
PUBLIC void I2C_Init(void)
{
	PORT_ISOPM3_0= 0x0 ;				/* P3.0 GPIO output */
	PORT_ISOPM3_1 = 0x0;


	R_DEV_PinInit (ICC_PinConfigSDA_out);

	//PORT_ISOP3_0= 1;
	//PORT_ISOP3_1= 1;


	R_GPIO_WritePin(3, 0, 1);
	E_Delay_i2c();                            /* provide sufficient delay */

	R_GPIO_WritePin(3, 1, 1);
	E_Delay_i2c();                            /* provide sufficient delay */
}

/* **********************************************************************************************************************
 * Function Name: I2C_Release_Bus
 * Description  : This function releases I2C bus.
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void I2C_Release_Bus(void)
{

	R_GPIO_WritePin(3, 0, 1);
	E_Delay_i2c();                            /* provide sufficient delay */

	R_GPIO_WritePin(3, 1, 1);
	E_Delay_i2c();                            /* provide sufficient delay */

}

/* **********************************************************************************************************************
 * Function Name: Start IIC_Start_Condition
 * Description  : This function generates I2C Start condition.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************** */
PUBLIC void IIC_Start_Condition(void)
{

	R_GPIO_WritePin(3, 1, 1);
	E_Delay_i2c();                           /* provide sufficient delay */

	R_GPIO_WritePin(3, 0, 1);

	E_Delay_i2c();                           /* provide sufficient delay */

	R_GPIO_WritePin(3, 0, 0);

	E_Delay_i2c();                           /* provide sufficient delay */
	R_GPIO_WritePin(3, 1, 0);

	E_Delay_i2c();                           /* provide sufficient delay */

}
/* **********************************************************************************************************************
 * Function Name: IIC_Stop_Condition
 * Description  : This function generates I2C stop condition.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************** */
PUBLIC void IIC_Stop_Condition(void)
{

	R_GPIO_WritePin(3, 1, 0);

	E_Delay_i2c();                           /* provide sufficient delay */

	R_GPIO_WritePin(3, 0, 0);				/*Set data line low*/

	E_Delay_i2c();                           /* provide sufficient delay */

	/* Set clock line high*/
	R_GPIO_WritePin(3, 1, 1);

	E_Delay_i2c();                           /* provide sufficient delay */

	R_GPIO_WritePin(3, 0, 1);				 /* Set data line high (STOP SIGNAL)*/

	E_Delay_i2c();                           /* provide sufficient delay */


}

/* **********************************************************************************************************************
 * Function Name: IIC_Master_Send
 * Description  : This function starts transferring data for IIC11 in master mode.
 * Arguments    : output_data
 * Return Value : Ack_Error
 ********************************************************************************************************************** */
PUBLIC uint8_t IIC_Master_Send(uint8_t output_data)
{
	uint8_t index;
	uint8_t ACK_Error;
	uint8_t read_pin_status;

	for(index = 0; index < NOF_BITS; index++)      // Send 8 bits to the eeprom
	{
		/* Output the data bit to the eeprom*/
		PORT_ISOP3_0= ((output_data & 0x80) ? SET : CLEAR);
		output_data  <<= 1;             /* Shift the byte by one bit */
		E_Delay_i2c();                           /* provide sufficient delay */

		R_GPIO_WritePin(3, 1, 1);

		E_Delay_i2c();                           /* provide sufficient delay */

		R_GPIO_WritePin(3, 1, 0);

		E_Delay_i2c();                           /* provide sufficient delay */

	}
	R_GPIO_WritePin(3, 0, 1);

	E_Delay_i2c();                           /* provide sufficient delay */

	R_DEV_PinInit (ICC_PinConfigSDA_in);

	R_GPIO_WritePin(3, 1, 1);

	E_Delay_i2c();                           /* provide sufficient delay */

	if(R_GPIO_PinRead(3,0) == SET)
	{
		ACK_Error = SET;
	}
	else
	{
		ACK_Error = CLEAR;
	}

	R_DEV_PinInit (ICC_PinConfigSDA_out);

	R_GPIO_WritePin(3, 1, 0);

	E_Delay_i2c();                           /* provide sufficient delay */

	return (ACK_Error);
}

/* **********************************************************************************************************************
 * Function Name: IIC_Master_Receive
 * Description  : This function starts receiving data for IIC11 in master mode.
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC uint8_t IIC_Master_Receive(void)
{
	uint8_t index, input_data;
	//SDA_PM = SET;
	//PORT_ISOPM3_0=0x1;

	R_DEV_PinInit (ICC_PinConfigSDA_in);
	input_data = 0x00;
	for(index = 0; index < NOF_BITS; index++)      /* Send 8 bits to the eeprom */
	{
		input_data <<= 1;               /* Shift the byte by one bit */

		R_GPIO_WritePin(3, 1, 0);

		E_Delay_i2c();                           /* provide sufficient delay */

		R_GPIO_WritePin(3, 1, 1);

		E_Delay_i2c();                           /* provide sufficient delay */

		input_data |=R_GPIO_PinRead(3,0);

	}

	R_GPIO_WritePin(3, 1, 0);

	E_Delay_i2c();                           /* provide sufficient delay */

	R_DEV_PinInit (ICC_PinConfigSDA_out);

	R_GPIO_WritePin(3, 0, 0);

	R_GPIO_WritePin(3, 0, 1);

	E_Delay_i2c();                           /* provide sufficient delay */


	R_GPIO_WritePin(3, 1, 1);
	E_Delay_i2c();                           /* provide sufficient delay */

	R_GPIO_WritePin(3, 1, 0);
	/*NACK*/

	return input_data;
}

/* **********************************************************************************************************************
 * Function Name: delay_time
 * Description  : This function generates the required delay.
 * Arguments    : time_end
 * Return Value : None
 ********************************************************************************************************************** */

PUBLIC void delay_time(uint16_t time_end)
{
	uint16_t index,Temp_var=0;
	for (index = 0; index < time_end; index++)
	{
		Temp_var++;
	}
}

void E_Delay_i2c(void)
/**************************************************************************************
 *	FUNCTION NAME	:	E_Delay_i2c
 *	RETURN TYPE 	:	void
 *	ARGUMENTS 	    :	none
 *	DESCRIPTION 	:	this function is used to provide the small delay required
 *                      for I2C bus communication. internally used by other funcs.
 *
 **************************************************************************************/
{
#if 1
	U8 ix,a=0;                               /* local loop index */
	for(ix=0;ix<40;ix++)
	{                 /* revolve five times and feed the dog */

		a++;
	}
#endif
//	R_TICK_WaitUS(0u, 10u);   // R_TICK_WaitUS(0u, 3u);

	return;
}
void Delay ( unsigned short  DelatT)
/**************************************************************************************
 **
 **  Parameter(s):     delay to be generated
 **
 **  Return value:     None
 **
 **  Var dependencies: None
 **
 **************************************************************************************/
{
	U8 b=0;
	for( unsigned short  i=0; i<DelatT; i++)
	{
		b++;
		/*__asm("nop"); //nop's may be added for timing adjustment*/
	}
}
