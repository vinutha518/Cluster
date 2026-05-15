/*
 * ADC.c
 *
 *  Created on: 22-Jan-2019
 *      Author: MSWELT23
 */

#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_adc_api.h"
#include "r_cg_adc.h"
#include "r_adc_sys.h"    /* for enable & disable ADC */
#include "int_enabled.h"  /* for enable ADC interrupt */

#define NOF_ADC_CHANNELS	3U
#define NOF_ADC_SAMPLES		10U

void ADC_Start_Conversion(void);
void ADC_Start(void);
void ADC_Stop(void);
void ADC_Wait_Conversion(void);
void ADC_Read(void);

//r_adc_Config_t my_adca_config;

uint16_t ADC_Array[3] = {0};

enum
{
	FUEL_IN,
	BATT_IN,
	SWSC_IN,
};
uint16_t Fuel_ADC_value    = 0;
uint16_t SWSC_ADC_value    = 0;
uint16_t Batt_ADC_value    = 0;
uint16_t Fuel_EXVrefADC_value = 0;
uint16_t New_fuel_read_ad  = 0;


uint8_t ADC_Counter_u1g = 0;
uint8_t AD_Done_u1g     = 0; /* This variable is need to be become 1 in "my_adc_isr()" */
uint8_t channel = 0;

/* **********************************************************************************************************************
* Function Name: ADC_Start_Conversion
* Description  : This function starts analog to digital conversion.
* Arguments    : None
* Return Value : None
********************************************************************************************************************** */
void ADC_Start_Conversion(void)  /* this function need to call in schedular */
{
	
    uint16_t timeout = 0xFFFF;
    //NOP();NOP();NOP();NOP();NOP();

    switch(ADC_Counter_u1g)
    {

      case 0:
    	  my_adca_config.VCh[0].phyChannel = 0x10;  /* Physical channel 0 */
    	  AD_Done_u1g = 0;
          ADC_Start();                     /* start the ADC conversion */
          ADC_Wait_Conversion();
          channel = 0;
          //ADC_Read();               /* read the value and store in respective variable */
      break;

      case 1:
    	  my_adca_config.VCh[1].phyChannel = 0x12;  /* Physical channel 2 */
          AD_Done_u1g = 0;
          ADC_Start();                  /* start the ADC conversion */
          ADC_Wait_Conversion();
          channel = 1;
          //ADC_Read();         /* read the value and store in respective variable */
      break;

      case 2:
    	  my_adca_config.VCh[2].phyChannel = 0x14;  /* Physical channel 4 */
          AD_Done_u1g = 0;
          ADC_Start();                    /* start the ADC conversion */
          ADC_Wait_Conversion();
          channel = 2;
          //ADC_Read();          /* read the value and store in respective variable */
      break;


      case 3:
    	  //ADC_Read();
    	  Fuel_ADC_value    = ADC_Array[FUEL_IN];
    	  New_fuel_read_ad  = Fuel_ADC_value;
    	  Batt_ADC_value    = ADC_Array[BATT_IN];
    	  SWSC_ADC_value    = ADC_Array[SWSC_IN];
    	  break;

      default:
           ADC_Counter_u1g = 0;
      break;


    }

    ADC_Counter_u1g++;               /* this counter help the number to channel to exceute for
                                     ADC ocnversion from switching the vref voltage.*/

                                     /* Now only fuel conversion is taken, if you want battery ADC
                                     value then change the value from 2 to 4 */

    if(ADC_Counter_u1g > 3) /* temporarily 3 used... 3 replaced by=> 4 */
    {
       ADC_Counter_u1g = 0; /* Three ADC values (Temp,Fuel,Batt) and initialization before reading this 3 -
                            totally requires 6 steps */
    }

}

/* **********************************************************************************************************************
* Function Name: ADC_Start
* Description  : This function starts analog to digital conversion.
* Arguments    : None
* Return Value : None
********************************************************************************************************************** */

void ADC_Start(void)
{
	/* enable interrupt flag  */
#ifdef USE_ADCA
    #ifndef INTADCE0TSN_ENABLE
        #define INTADCE0TSN_ENABLE (1u)
    #endif
    #ifndef INTADCE0I1_ENABLE
        #define INTADCE0I1_ENABLE (1u)
    #endif
    #ifndef INTADCE0I2_ENABLE
        #define INTADCE0I2_ENABLE (1u)
    #endif
    #ifndef INTADCE0I3_ENABLE
        #define INTADCE0I3_ENABLE (1u)
    #endif

#endif
	/* enable ADC */
	R_SYS_ADC_EnableInt(0,R_ADC_INT_CG0);
	R_SYS_ADC_EnableInt(0,R_ADC_INT_CG1);
	R_SYS_ADC_EnableInt(0,R_ADC_INT_CG2);
}

/* **********************************************************************************************************************
* Function Name: ADC_Stop
* Description  : This function stops analog to digital conversion.
* Arguments    : None
* Return Value : None
********************************************************************************************************************** */

void ADC_Stop(void)
{

	/*  disable interrupt flag  */
#ifdef USE_ADCA
    #ifndef INTADCE0TSN_ENABLE
        #define INTADCE0TSN_ENABLE (0u)
    #endif
    #ifndef INTADCE0I1_ENABLE
        #define INTADCE0I1_ENABLE (0u)
    #endif
    #ifndef INTADCE0I2_ENABLE
        #define INTADCE0I2_ENABLE (0u)
    #endif
    #ifndef INTADCE0I3_ENABLE
        #define INTADCE0I3_ENABLE (0u)
    #endif

#endif
	/*  disable ADC */
	R_SYS_ADC_DisableInt(0,R_ADC_INT_CG0);
	R_SYS_ADC_DisableInt(0,R_ADC_INT_CG1);
	R_SYS_ADC_DisableInt(0,R_ADC_INT_CG2);
}


void ADC_Wait_Conversion(void)
{
    uint16_t timeout;

    timeout = 0xFFFF;

    while(!AD_Done_u1g && timeout--)   /* IF the AD module fails somehow the program will keep looping*/
    {
                                           /* in this while loop, so for protecting we are using a timeout */
    }
}
