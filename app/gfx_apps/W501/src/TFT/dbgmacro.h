/*
***************************************************************************************
***************************************************************************************
***
***     File: dbgmacro.h                                                              
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

   \Subject    Debug print macros

   \Reference  1612/1-06095 SRE Common Software Components

   \Abstract
   
   This header file contains wrapper macros for debug printouts on can.


   \Version Id:
   
   "$Id: dbgmacro.h 372 2013-12-16 08:46:46Z STONERIDGE\MLR $"


@{
***************************************************************************************
*/




/*
**====================================================================================
** Double inclusion protection
**====================================================================================
*/

#ifndef DBGMACRO_H
#define DBGMACRO_H


/*
**====================================================================================
** Imported definitions
**====================================================================================
*/

#include "r_typedefs.h"
//#include "moduleid.h"

/*
** The following functions should not be called
** directly but only via the debug print macros below!
*/
extern void dbgprint_debugPrint(U8 file_id, U16 line);
extern void dbgprint_debugPrintData(U8 file_id, U16 line, U8 size, const void *data);


/*
**====================================================================================
** Debug macros
**====================================================================================
*/

/*lint -esym(750,FILE_ID) -esym(750,DEBUG_ON) */
#if defined(_lint) && defined(DEBUG_PRINT_ENABLED)
   #undef DEBUG_ON /* lint !e960 Misra Advisory Rule 92, use of 'undef' is discouraged */
   #define DEBUG_ON  0
   #undef DEBUG_PRINT_ENABLED /* lint !e960 Misra Advisory Rule 92, use of 'undef' is discouraged */
   #define DEBUG_PRINT_ENABLED   0
#endif

#if DEBUG_PRINT_ENABLED
   /*
   ** Unmaskable debug prints
   */
   #define DEBUG_PRINT_UNMASKED(message) \
         dbgprint_debugPrint((U8)FILE_ID, (U16)__LINE__);
         
   #define DEBUG_PRINT_DATA_UNMASKED(message, data) \
         dbgprint_debugPrintData((U8)FILE_ID, (U16)__LINE__, sizeof(*data), data);

   
   /*
   ** Level 1 maskable debug prints
   */
   #if DEBUG_ON
      #define DEBUG_PRINT(message) \
            dbgprint_debugPrint((U8)FILE_ID, (U16)__LINE__);
      #define DEBUG_PRINT_DATA(message, data) \
            dbgprint_debugPrintData((U8)FILE_ID, (U16)__LINE__, sizeof(*data), data)
   #else
      #define DEBUG_PRINT(message)
      #define DEBUG_PRINT_DATA(message, data)
   #endif
   
   /*
   ** Level 2 maskable debug prints
   */
   #if DEBUG_ON > 1
      #define DEBUG_PRINT2(message) \
            dbgprint_debugPrint((U8)FILE_ID, (U16)__LINE__);
      #define DEBUG_PRINT_DATA2(message, data) \
            dbgprint_debugPrintData((U8)FILE_ID, (U16)__LINE__, sizeof(*data), data)
   #else
      #define DEBUG_PRINT2(message)
      #define DEBUG_PRINT_DATA2(message, data)
   #endif
   
   /*
   ** Level 3 maskable debug prints
   */
   #if DEBUG_ON > 2
      #define DEBUG_PRINT3(message) \
            dbgprint_debugPrint((U8)FILE_ID, (U16)__LINE__);
      #define DEBUG_PRINT_DATA3(message, data) \
            dbgprint_debugPrintData((U8)FILE_ID, (U16)__LINE__, sizeof(*data), data)
   #else
      #define DEBUG_PRINT3(message)
      #define DEBUG_PRINT_DATA3(message, data)
   #endif
#else
   #define DEBUG_PRINT_UNMASKED(message)
   #define DEBUG_PRINT_DATA_UNMASKED(message, data)
   #define DEBUG_PRINT(message)
   #define DEBUG_PRINT_DATA(message, data)
   #define DEBUG_PRINT2(message)
   #define DEBUG_PRINT_DATA2(message, data)
   #define DEBUG_PRINT3(message)
   #define DEBUG_PRINT_DATA3(message, data)
#endif

#endif


/************************************************************************************/
/** @}
************************************ End of file ************************************/
