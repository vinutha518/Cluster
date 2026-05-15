/*
****************************************************************************
PROJECT : ISM Test APP
FILE    : $Id: carmodel.c 4594 2015-01-08 17:13:58Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Car Model
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2015
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

/*******************************************************************************
  Title: carmodel.c

  Implementation of car model functions.
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "carmodel.h"


/***********************************************************
  Section: Local Defines
*/


/***********************************************************
  Define: LOC_ACCELERATION_FACTOR

  Speed difference that is caused by one speed update tick. (in km/h)
*/

#define LOC_ACCELERATION_FACTOR    0.95


/***********************************************************
  Define: LOC_CM_SPEED_MAX

  Maximum speed.
*/

#define LOC_CM_SPEED_MAX           240


/***********************************************************
  Define: LOC_CM_SPEED_MIN

  Minimum speed.
*/

#define LOC_CM_SPEED_MIN             10


/***********************************************************
  Define: LOC_CM_SPEED_MID

  Medium speed.
*/

#define LOC_CM_SPEED_MID             50


/***********************************************************
  Define: LOC_CM_RPM_MAX

  Maximum rpm.
*/

#define LOC_CM_RPM_MAX             8000


/***********************************************************
  Define: LOC_CM_RPM_MIN

  Minimum rpm.
*/

#define LOC_CM_RPM_MIN             100


/***********************************************************
  Define: LOC_CM_GEARDOWN_RPM

  Shift down RPM.
*/

#define LOC_CM_GEARDOWN_RPM        2700

/***********************************************************
  Define: LOC_CM_GEARUP_RPM

  Shift down RPM.
*/

#define LOC_CM_GEARUP_RPM          5500


/***********************************************************
  Define: LOC_CM_GEAR_MAX

  Max gear.
*/

#define LOC_CM_GEAR_MAX            5


/***********************************************************
  Define: LOC_CM_GEAR_MIN

  Min gear.
*/

#define LOC_CM_GEAR_MIN            1


/***********************************************************
  Section: Local Variables
*/

/***********************************************************
  Variable: CarData
*/
CarData_t CarData;


/***********************************************************
  Variable: locGearTrans

  Translation factor speed to rpm.
*/

static const uint16_t locGearTrans[] = {
    200, 200, 150, 100, 50, 25 
};


/***********************************************************
  Section: Local Functions
*/


/***********************************************************
  Function: locUpdateTurnSignals
  
  Update turn signal values.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateTurnSignals(void) {
    static uint32_t cc = 0;

    if (cc == 1) {
        CarData.Left  = 1;
        CarData.Right = 0;
    } else if (cc == 500) {
        CarData.Left  = 0;
        CarData.Right = 0;
    } else if (cc == 1500) {
        CarData.Left  = 0;
        CarData.Right = 1;
    } else if (cc == 2000) {
        CarData.Left  = 0;
        CarData.Right = 0;
    } else if (cc == 2500) {
        CarData.Left  = 1;
        CarData.Right = 1;
    } else if (cc > 3000) {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateSpeed
  
  Update speed value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateSpeed(void) {
    static int cc = 0;

    if (((CarData.Speed <= LOC_CM_SPEED_MIN)
    	    || ((CarData.Speed >= LOC_CM_SPEED_MID) && (CarData.Speed < LOC_CM_SPEED_MID + LOC_ACCELERATION_FACTOR))
    	    || (CarData.Speed >= LOC_CM_SPEED_MAX))
        && (cc < 100)) { 
        cc++;
        if (cc < 100) {
            return;
        }
        cc = 0;
    }
    CarData.Speed += (LOC_ACCELERATION_FACTOR * (float)CarData.Direction);
    if (CarData.Speed > LOC_CM_SPEED_MAX) {
        CarData.Speed = LOC_CM_SPEED_MAX;
        CarData.Direction = -1;
    } else if (CarData.Speed <= LOC_CM_SPEED_MIN) {
        CarData.Speed = LOC_CM_SPEED_MIN;
        CarData.Direction = 1;
    } 
}


/***********************************************************
  Function: locUpdateRpm
  
  Update RPM & Gear value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateRpm(void) {
    do {
        CarData.Rpm = CarData.Speed * locGearTrans[CarData.Gear];
        if ((CarData.Rpm > LOC_CM_GEARUP_RPM) && (CarData.Direction > 0)) {
            if (CarData.Gear < LOC_CM_GEAR_MAX) {
                CarData.Gear++;
                CarData.Rpm = CarData.Speed * locGearTrans[CarData.Gear];
            }
        } else if ((CarData.Rpm < LOC_CM_GEARDOWN_RPM) && (CarData.Direction < 0)){
            if (CarData.Gear > LOC_CM_GEAR_MIN) {
                CarData.Gear--;
                CarData.Rpm = CarData.Speed * locGearTrans[CarData.Gear];
            }
        }
    } while ((CarData.Rpm > LOC_CM_RPM_MAX) || (CarData.Rpm < LOC_CM_RPM_MIN));

}


/***********************************************************
  Function: locUpdateLight
  
  Update low & high light
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateLight(void) {
    static int cc = 0; 

    if (cc < 100) {
        CarData.HighLight = 0;
        CarData.LowLight  = 0;
    } else if (cc < 200) {
        CarData.HighLight = 0;
        CarData.LowLight  = 1;
    } else if (cc < 250) {
        CarData.HighLight = 1;
        CarData.LowLight  = 1;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateFan
  
  Update fan
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateFan(void) {
    static int cc = 0; 

    if (cc < 200) {
        CarData.Fan = 0;
    } else if (cc < 300) {
        CarData.Fan = 1;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateOil
  
  Update oil
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateOil(void) {
    static int cc = 0; 

    if (cc < 250) {
        CarData.Oil = 0;
    } else if (cc < 325) {
        CarData.Oil = 1;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateBattery
  
  Update battery
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateBattery(void) {
    static int cc = 0; 

    if (cc < 125) {
        CarData.Battery = 1;
    } else if (cc < 225) {
        CarData.Battery = 0;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateAirbag
  
  Update airbag
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateAirbag(void) {
    static int cc = 0; 

    if (cc < 115) {
        CarData.Airbag = 1;
    } else if (cc < 1005) {
        CarData.Airbag = 0;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateBelt
  
  Update belt
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateBelt(void) {
    static int cc = 0; 

    if (cc < 415) {
        CarData.Belt = 1;
    } else if (cc < 705) {
        CarData.Belt = 0;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateBreak
  
  Update break
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateBreak(void) {
    static int cc = 0; 

    if (cc < 215) {
        CarData.Break = 1;
    } else if (cc < 305) {
        CarData.Break = 0;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateWindshield
  
  Update windshield
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateWindshield(void) {
    static int cc = 0; 

    if (cc < 95) {
        CarData.Windshield = 1;
    } else if (cc < 285) {
        CarData.Windshield = 0;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateWiping
  
  Update wiping
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateWiping(void) {
    static int cc = 0; 

    if (cc < 625) {
        CarData.Wiping = 1;
    } else if (cc < 1305) {
        CarData.Wiping = 0;
    } else {
        cc = 0;
    }
    cc++;
}


/***********************************************************
  Function: locUpdateCoolingLiquid
  
  Update cooling liquid  value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateCoolingLiquid(void) {
    static int inc = 1;

    CarData.CoolingTemp += inc;
    if (CarData.CoolingTemp > 100) {
        inc = -1;
    } else if (CarData.CoolingTemp < 20) {
        inc = 1;
    }
}




/***********************************************************
  Function: locUpdateFuel
  
  Update fuel value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateFuel(void) {
    static int cc = 0; 

    cc++;
    if ((cc % 2) == 0) {
        if (CarData.FuelLevel == 0) {
            CarData.FuelLevel = 100;
        } 
        CarData.FuelLevel--;
    }
}


/***********************************************************
  Function: locUpdateGearSelect
  
  Update gear selection.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateGearSelect(void) {
   static int cc = 0; 

    cc++;
    if ((cc % 50) == 0) {
        CarData.GearSelect = CarData.GearSelect << 1;
        if (CarData.GearSelect > 16) {
            CarData.GearSelect = 1;
        }
    }
}


/***********************************************************
  Section: Global Functions

  See: <app_cl_carmodel.h>
*/


/***********************************************************
  Function: CarModel_Init
*/

void CarModel_Init(void)
{
    CarData.Speed      = 30;
    CarData.Rpm        = 25;
    CarData.Gear       = 1;
    CarData.GearSelect = 1;
    CarData.Direction  = 1;
}


/***********************************************************
  Function: CarModel_DeInit
*/

void CarModel_DeInit(void)
{
}


/***********************************************************
  Function: CarModel_Update
  
  Car model update
  
  updates the car model data.
  
  Parameters:
  none
  
  Returns:
  void
*/

void CarModel_Update(void)
{
        locUpdateSpeed();
        locUpdateRpm();
        locUpdateTurnSignals();
        locUpdateLight();
        locUpdateFan();
        locUpdateOil();
        locUpdateBattery();
        locUpdateAirbag();
        locUpdateBelt();
        locUpdateBreak();
        locUpdateWindshield();
        locUpdateWiping();
        locUpdateCoolingLiquid();
        locUpdateFuel();
        locUpdateGearSelect();
}


/***********************************************************
  Function: CarModelSpeedGet
*/

double CarModelSpeedGet(void) {
    return CarData.Speed;
}

void CarModelSpeedSet(double Speed) {
    CarData.Speed = Speed;
}


/***********************************************************
  Function: CarModelRpmGet
*/

uint16_t CarModelRpmGet(void) {
    return CarData.Rpm;
}

void CarModelRpmSet(uint16_t Rpm) {
    CarData.Rpm = Rpm;
}


/***********************************************************
  Function: CarModelLeftGet
*/

uint8_t CarModelLeftGet(void) {
    return CarData.Left;
}

/***********************************************************
  Function: CarModelRightGet
*/

uint8_t CarModelRightGet(void) {
    return CarData.Right;
}


/***********************************************************
  Function: CarModelHighLightGet
*/

uint8_t CarModelHighLightGet(void) {
    return CarData.HighLight;
}


/***********************************************************
  Function: CarModelLowLightGet
*/

uint8_t CarModelLowLightGet(void) {
    return CarData.LowLight;
}


/***********************************************************
  Function: CarModelFanGet
*/

uint8_t CarModelFanGet(void) {
    return CarData.Fan;
}


/***********************************************************
  Function: CarModelOilGet
*/

uint8_t CarModelOilGet(void) {
    return CarData.Oil;
}


/***********************************************************
  Function: CarModelBatteryGet
*/

uint8_t CarModelBatteryGet(void) {
    return CarData.Battery;
}


/***********************************************************
  Function: CarModelAirbagGet
*/

uint8_t CarModelAirbagGet(void) {
    return CarData.Airbag;
}


/***********************************************************
  Function: CarModelBeltGet
*/

uint8_t CarModelBeltGet(void) {
    return CarData.Belt;
}


/***********************************************************
  Function: CarModelBreakGet
*/

uint8_t CarModelBreakGet(void) {
    return CarData.Break;
}


/***********************************************************
  Function: CarModelWindshieldGet
*/

uint8_t CarModelWindshieldGet(void) {
    return CarData.Windshield;
}


/***********************************************************
  Function: CarModelWipingGet
*/

uint8_t CarModelWipingGet(void) {
    return CarData.Wiping;
}


/***********************************************************
  Function: CarModelFuelEmptyGet
*/

uint8_t CarModelFuelEmptyGet(void) {
    uint8_t x;

    if (CarData.FuelLevel > 10) {
        x = 0;
    } else {
        x = 1;
    }
    return x;
}


/***********************************************************
  Function: CarModelCoolingHotGet
*/

uint8_t CarModelCoolingHotGet(void) {
    uint8_t x;

    if (CarData.CoolingTemp < 90) {
        x = 0;
    } else {
        x = 1;
    }
    return x;
}


/***********************************************************
  Function: CarModelFuelLevelGet
*/

uint8_t CarModelFuelLevelGet(void) {
    return CarData.FuelLevel;
}


/***********************************************************
  Function: CarModelCoolingTempGet
*/

int8_t CarModelCoolingTempGet(void) {
    return CarData.CoolingTemp;
}

/***********************************************************
  Function: CarModelGearPGet
*/

uint8_t CarModelGearPGet(void) {
    return (CarData.GearSelect & 1);
}


/***********************************************************
  Function: CarModelGearRGet
*/

uint8_t CarModelGearRGet(void) {
    return (CarData.GearSelect & 2);
}


/***********************************************************
  Function: CarModelGearNGet
*/

uint8_t CarModelGearNGet(void)  {
    return (CarData.GearSelect & 4);
}


/***********************************************************
  Function: CarModelGearDGet
*/

uint8_t CarModelGearDGet(void)  {
    return (CarData.GearSelect & 8);
}


/***********************************************************
  Function: CarModelGearSGet
*/

uint8_t CarModelGearSGet(void) {
    return (CarData.GearSelect & 16);
}

