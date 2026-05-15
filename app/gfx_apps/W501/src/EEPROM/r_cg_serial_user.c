/*
 * r_cg_serial_user.c
 *
 *  Created on: Jan 28, 2019
 *      Author: MSWELT12
 */
/* **********************************************************************************************************************
File            :       r_cg_serial_user.c
Author          :
Model           :       W501
Revision Author :
Reviewed By     :
Company         :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune
/********************************************************************************************************************* */

/* *********Includes************************************************************************************************** */


#include "r_typedefs.h"
//#include "dr7f701441.dvf.h"
//#include "dr7f701461.dvf.h"
#include "my_can.h"
#include "r_gpio_api.h"
#include "r_cg_timer.h"
#include "UDSS.h"
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "r_cg_timer.h"
#include "memmory_map.h"
#include "r_cg_serial_user.h"
#include "r_tick_api.h"
#include "odometer.h"

/* *********Variables Defination*************************************************************************************** */
void delay(void)  ;
void eeprom_write(uint16_t Address, uint8_t Write_Buffer);
extern void delay(void);
_Bool EEPROM_IN_USE=0;
extern uint64_t Counter_IC_CAN_tx;
extern uint64_t Counter_10ms;
/* **********************************************************************************************************************
 * Function Name: EEPROM_init
 * Description  : This function initialize EEPROM.
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void EEPROM_init(void)
{
	uint8_t value = CLEAR;
	uint16_t cnt;

	// RESET_WDT; watchdog W501     								 /*  feed the dog   */
	for(cnt = EEPROM_START_LOCATION; cnt <= EEPROM_MAX_LOCATION; cnt++)
	{
		//  RESET_WDT;      watchdog W501                        			 /*  feed the dog   */
		eeprom_write(cnt, value);
		delay();delay();
	}
}

/* **********************************************************************************************************************
 * Function Name: eeprom_write
 * Description  : This function to write to EEPROM
 * Arguments    : Address - The address at which we have to read value
                 write_buffer - value to save in EEPROM
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void eeprom_write(uint16_t Address, uint8_t Write_Buffer)
{
	EEPROM_IN_USE = 1;

	eeprom_writebt(Address, Write_Buffer);
#if 0
	uint8_t hi,lo;
	uint8_t err,timeout = 0xFF;
	EEPROM_IN_USE = 1;
	lo = (uint8_t)Address;
	hi = (uint8_t)(Address >> 8);

	I2C_Release_Bus();
	do
	{
		E_Delay_i2c();                       /* provide sufficient cummunication delay */
		IIC_Start_Condition();                   /* Send eprom Start Transfer */
		E_Delay_i2c();                       /* provide sufficient cummunication delay */
		err = IIC_Master_Send(EEPROM_ADDRESS);   /* Send device Address on eprom to activate the device */

	}while((--timeout) && (err));

	IIC_Master_Send(hi);                          /* Send memoryaddress to the device */
	IIC_Master_Send(lo);                          /* Send memoryaddress to the device */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */

	IIC_Master_Send(Write_Buffer);                /* write the value to the device */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */

	E_Delay_i2c();                       /* provide sufficient cummunication delay */
	IIC_Stop_Condition();                         /* Stop the eprom */
	delay_time(6000);
	//R_TICK_WaitMS( 0,  10u);
#endif
	EEPROM_IN_USE = 0;

}

/* **********************************************************************************************************************
 * Function Name: eeprom_DTC_write
 * Description  : This function to write to EEPROM
 * Arguments    : Address - The address at which we have to read value
                 write_buffer - value to save in EEPROM
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void eeprom_DTC_write(uint16_t Address, uint8_t Write_Buffer)
{

	uint8_t hi,lo;
	uint8_t err,timeout = 0xFF;
	EEPROM_IN_USE = 1;
	lo = (uint8_t)Address;
	hi = (uint8_t)(Address >> 8);

	I2C_Release_Bus();
	do
	{
		E_Delay_i2c();                       /* provide sufficient cummunication delay */
		IIC_Start_Condition();                   /* Send eprom Start Transfer */
		E_Delay_i2c();                       /* provide sufficient cummunication delay */
		err = IIC_Master_Send(EEPROM_ADDRESS);   /* Send device Address on eprom to activate the device */

	}while((--timeout) && (err));

	IIC_Master_Send(hi);                          /* Send memoryaddress to the device */
	IIC_Master_Send(lo);                          /* Send memoryaddress to the device */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */

	IIC_Master_Send(Write_Buffer);                /* write the value to the device */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */

	E_Delay_i2c();                       /* provide sufficient cummunication delay */
	IIC_Stop_Condition();                         /* Stop the eprom */
	delay_time(6000);
	//R_TICK_WaitMS( 0,  10u);
	EEPROM_IN_USE = 0;
}

/* **********************************************************************************************************************
 * Function Name: eeprom_write
 * Description  : This function to write to EEPROM
 * Arguments    : Address - The address at which we have to read value
                 write_buffer - value to save in EEPROM
 * Return Value : None
 ********************************************************************************************************************** */
void eeprom_writebt(uint16_t Address, uint8_t Write_Buffer)
{
	__DI();
	uint8_t hi,lo;
	uint8_t err,timeout = 0xFF;
	EEPROM_IN_USE = 1;
	lo = (uint8_t)Address;
	hi = (uint8_t)(Address >> 8);
	Counter_IC_CAN_tx+=20;
	Counter_10ms+=20;
	I2C_Init();
	I2C_Release_Bus();
	do
	{
		delay_time(20);                         /* provide sufficient cummunication delay */
		IIC_Start_Condition();                   /* Send eprom Start Transfer */
		delay_time(20);                         /* provide sufficient cummunication delay */
		err = IIC_Master_Send(EEPROM_ADDRESS);   /* Send device Address on eprom to activate the device */

	}while((--timeout) && (err));

	IIC_Master_Send(hi);                          /* Send memoryaddress to the device */
	IIC_Master_Send(lo);                          /* Send memoryaddress to the device */
	delay_time(20);                          /* provide sufficient cummunication delay */

	IIC_Master_Send(Write_Buffer);                /* write the value to the device */
	delay_time(40);                          /* provide sufficient cummunication delay */
	IIC_Stop_Condition();                         /* Stop the eprom */
	delay_time(60000);
	__EI();
	EEPROM_IN_USE = 0;
}
/* **********************************************************************************************************************
 * Function Name: eeprom_read
 * Description  : This function read EEPROM
 * Arguments    : Address - The address at which we have to read value
 * Return Value : read_buffer - value from EEPROM
 ********************************************************************************************************************** */
PUBLIC uint8_t eeprom_read(uint16_t Address)
{
	__DI();
	uint8_t data_in, hi, lo;
	uint8_t err,timeout = 0xFF;
	EEPROM_IN_USE = 1;

	//RESET_WDT; watchdog W501
	lo = (uint8_t)Address;
	hi = (uint8_t)(Address >> 8);

	I2C_Init();
	I2C_Release_Bus();
	do
	{
		delay_time(20);
		delay_time(20);

		IIC_Start_Condition();                      /* Send eprom Start Transfer */
		delay_time(20);
		delay_time(20);

		err = IIC_Master_Send(EEPROM_ADDRESS);            /* Send identifier eeprom address - Write */
	}while((--timeout) && (err));

	IIC_Master_Send(hi);
	IIC_Master_Send(lo);
	delay_time(20);                 /* provide sufficient cummunication delay */
	delay_time(20);



		I2C_Release_Bus();                          /* Send eprom Stop Transfer */
		IIC_Start_Condition();                      /* Send eprom Start Transfer */
		delay_time(20);                       /* provide sufficient cummunication delay */
		delay_time(20);

		IIC_Master_Send(EEPROM_ADDRESS_READ);       /* Send identifier eeprom address - Read */
	delay_time(20);                        /* provide sufficient cummunication delay */
	delay_time(20);

	data_in = IIC_Master_Receive();             /* Read the data */
	delay_time(20);                        /* provide sufficient cummunication delay */
	delay_time(20);


	IIC_Stop_Condition();
	//delay_time(6000);
	__EI();
	EEPROM_IN_USE = 0;
	return data_in;

}

/* **********************************************************************************************************************
 * Function Name: eeprom_read
 * Description  : This function read EEPROM
 * Arguments    : Address - The address at which we have to read value
 * Return Value : read_buffer - value from EEPROM
 ********************************************************************************************************************** */
PUBLIC uint8_t eeprom_DTC_read(uint16_t Address)
{

	uint8_t data_in, hi, lo;
	EEPROM_IN_USE = 1;
__DI();
	//RESET_WDT; watchdog W501
	lo = (uint8_t)Address;
	hi = (uint8_t)(Address >> 8);

	I2C_Release_Bus();
	E_Delay_i2c();
	E_Delay_i2c();

	IIC_Start_Condition();                      /* Send eprom Start Transfer */
	E_Delay_i2c();
	E_Delay_i2c();

	IIC_Master_Send(EEPROM_ADDRESS);            /* Send identifier eeprom address - Write */
	IIC_Master_Send(hi);
	IIC_Master_Send(lo);
	E_Delay_i2c();                       /* provide sufficient cummunication delay */
	E_Delay_i2c();

	I2C_Release_Bus();                          /* Send eprom Stop Transfer */
	IIC_Start_Condition();                      /* Send eprom Start Transfer */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */
	E_Delay_i2c();

	IIC_Master_Send(EEPROM_ADDRESS_READ);       /* Send identifier eeprom address - Read */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */
	E_Delay_i2c();

	data_in = IIC_Master_Receive();             /* Read the data */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */
	E_Delay_i2c();

	IIC_Stop_Condition();

	__EI();
	EEPROM_IN_USE = 0;
	return data_in;


}

/* **********************************************************************************************************************
 * Function Name: eeprom_memwrite_32bytes
 * Description  : This function write array in eeprom .
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void eeprom_memwrite_32bytes(unsigned int memorylocation, unsigned char *Address,unsigned char Number_Of_bytes)
{
	unsigned char hi, lo, i = CLEAR;
	unsigned char err,timeout = 0xFF;
	EEPROM_IN_USE = 1;

	for(i = 0; i < Number_Of_bytes; i++)
	{

		eeprom_writebt((memorylocation+i), *(Address + i));
		E_Delay_i2c();                       /* provide sufficient cummunication delay */
	}



#if 0
	lo = (unsigned char)memorylocation;
	hi = (unsigned char)(memorylocation >> 8);

	I2C_Release_Bus();
	do
	{
		E_Delay_i2c();                       /* provide sufficient cummunication delay */
		IIC_Start_Condition();                  /* Send eprom Start Transfer */
		E_Delay_i2c();                       /* provide sufficient cummunication delay */

		err = IIC_Master_Send(EEPROM_ADDRESS);  /* Send device Address on eprom to activate the device */
	}while((--timeout) && (err));

	IIC_Master_Send(hi);                         /* Send memoryaddress to the device */
	IIC_Master_Send(lo);                         /* Send memoryaddress to the device */
	E_Delay_i2c();                       /* provide sufficient cummunication delay */

	for(i = 0; i < Number_Of_bytes; i++)
	{
		IIC_Master_Send(*(Address + i));           /* write the value to the device */
		/* Address++; */

		E_Delay_i2c();                       /* provide sufficient cummunication delay */
	}

	IIC_Stop_Condition();                        /* Stop the eprom   */
	//R_TICK_WaitMS( 0,  10u);
	delay_time(60000);
#endif
	EEPROM_IN_USE = 0;
}

/* **********************************************************************************************************************
 * Function Name: eeprom_memwrite_32bytes
 * Description  : This function write array in eeprom .
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void eeprom_memread_32bytes(unsigned int memorylocation, unsigned char *Address,unsigned char Number_Of_bytes)
{
	EEPROM_IN_USE = 1;
	unsigned char i;
	for(i = 0; i < Number_Of_bytes; i++)
	{
		*(Address + i) = eeprom_read(memorylocation + i);           /* Read the date */
		//  RESET_WDT; watchdog timer W501
	}
	EEPROM_IN_USE = 0;
}

/* **********************************************************************************************************************
 * Function Name: New_eeprom_memwrite_32bytes
 * Description  : This function write array in eeprom .
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void New_eeprom_memwrite_32bytes(unsigned int memorylocation, unsigned char *Address,unsigned char Number_Of_bytes)
{
	EEPROM_IN_USE = 1;
	unsigned char i;
	for(i = 0; i < Number_Of_bytes; i++)
	{
		//*(Address + i) = eeprom_read(memorylocation + i);           /* Read the date */
		eeprom_write((memorylocation+i),(*(Address + i)));
		//  RESET_WDT; watchdog timer W501
	}
	EEPROM_IN_USE = 0;
}

/* **********************************************************************************************************************
 * Function Name: delay
 * Description  : This function generates the delay.
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void delay(void)                                     /*  5 ms  */
{
	uint8_t  l, j;
	for(l = 0; l <= 0xFEU; l++)
	{
		for(j = 0; j <= 0x1FU; j++){}
		//   RESET_WDT;    watchdog timer W501       /*  feed the dog   */
	}
}

void eeprom_write_AFE_data(unsigned int memorylocation,uint32_t value)
/**************************************************************************************
 **
 **  Parameter(s):      AFE Values write
 **
 **  Return value:       -
 **
 **  Var dependencies:
 **
 **  Description:
 **
 **
 **
 **************************************************************************************/
{
	Split32 afe_tmp;

	afe_tmp.u32 = value;
	eeprom_writeEeprom(memorylocation, 4, afe_tmp.u8);
}


uint32_t eeprom_read_AFE_data(unsigned int memorylocation)
/**************************************************************************************
 **
 **  Parameter(s):       -
 **
 **  Return value:       stored afe data value
 **
 **  Var dependencies:    (RW)
 **
 **  Description:        Reads out the last stored afe data value out of the EEPROM
 **                      and returns it.
 **
 **************************************************************************************/
{
	uint32_t     afe_read_value = 0;
	Split32     AFE_read;

	eeprom_readEeprom(memorylocation, 4, AFE_read.u8);
	afe_read_value        = AFE_read.u32;
	return afe_read_value;
}


void eeprom_write_IVN_FC_data(unsigned int memorylocation,uint32_t value)
/**************************************************************************************
 **
 **  Parameter(s):      AFE Values write
 **
 **  Return value:       -
 **
 **  Var dependencies:
 **
 **  Description:
 **
 **
 **
 **************************************************************************************/
{
	Split32 afe_tmp;

	afe_tmp.u32 = value;
	eeprom_writeEeprom(memorylocation, 4, afe_tmp.u8);
}


uint32_t eeprom_read_IVN_FC_data(unsigned int memorylocation)
/**************************************************************************************
 **
 **  Parameter(s):       -
 **
 **  Return value:       stored afe data value
 **
 **  Var dependencies:    (RW)
 **
 **  Description:        Reads out the last stored afe data value out of the EEPROM
 **                      and returns it.
 **
 **************************************************************************************/
{
	uint32_t     afe_read_value = 0;
	Split32     AFE_read;

	eeprom_readEeprom(memorylocation, 4, AFE_read.u8);
	afe_read_value        = AFE_read.u32;
	return afe_read_value;
}
