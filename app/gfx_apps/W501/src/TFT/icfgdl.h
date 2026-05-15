/*
***************************************************************************************
***************************************************************************************
***
***     File: icfgdl.h  
***
***     Project: SRE Common Software Component
***
***     Copyright (C) Stoneridge Electronics AB, 1990 - $Date:: 2013#$
***
***     This file is a part of the Stoneridge Common Software Component Repository
***     and may only be used in accordance with the terms and conditions stipulated
***     in the agreement contract under which it has been supplied.
***
***************************************************************************************
**************************************************************************************/
/** \file

   \ingroup    SRE_COMMON_SOFTWARE_COMPONENT

   \Subject    Private configuration of the data layer

   \Reference  1231/001-05368 SRE Common Platform Architecture

   \Abstract
   
   This header file contains the Private configurations of the data layer.


   \Version Id:
   
   "$Id: icfgdl.h 372 2013-12-16 08:46:46Z STONERIDGE\MLR $"


@{
***************************************************************************************
*/




/*
**====================================================================================
** Double inclusion protection
**====================================================================================
*/

#ifdef DL_PRIVATE_CONFIGURATION


/*
**====================================================================================
** Imported definitions
**====================================================================================
*/

#include "typedefs.h"
#include "protect.h"
#include "errmgr.h"
#include "moduleid.h"


/*
**====================================================================================
** Critical section protection macros used within data layer
**====================================================================================
*/

#define DL_CRITICAL_SECTION_BEGIN()    protect_enterCriticalSection()
#define DL_CRITICAL_SECTION_END()      protect_exitCriticalSection()


/*
**====================================================================================
** Assert macros used within the data layer
**====================================================================================
*/

#define DL_ASSERT(test) /*lint -e506 Constant value Boolean */ \
      ((test) ? ((void)0) : errmgr_userError(errmgr_RESET, (U16)DL_C, (U16)__LINE__))

#define DL_ASSURE(test) /*line -e506 Constant value Boolean */       \
      if (!(test)) {                                                 \
         errmgr_userError(errmgr_RESET, (U16)DL_C, (U16)__LINE__);   \
      } else


#endif


/************************************************************************************/
/** @}
************************************ End of file ************************************/
