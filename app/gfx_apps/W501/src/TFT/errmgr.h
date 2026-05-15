/*
***************************************************************************************
***************************************************************************************
***
***     File: errmgr.h                                                                
***
***     Project: YMER                                                                 
***
***     Copyright (C) Stoneridge Electronics AB, 1990 - $Date:: 2013#$
***
***************************************************************************************
**************************************************************************************/
/** \file

   \ingroup    SVC

   \Subject    Interface to module errmgr.c

   \Reference  YMER SW ARCHITECTURE DESIGN

   \Abstract
   
   This header file contains the Public definitions and declarations of module
   errmgr.c.


   \Version Id:
   
   "$Id: errmgr.h 251 2013-10-24 13:03:48Z STONERIDGE\MLR $"


@{
***************************************************************************************
*/




/*
**====================================================================================
** Double inclusion protection
**====================================================================================
*/

#ifndef ERRMGR_H
#define ERRMGR_H


/*
**====================================================================================
** Imported definitions
**====================================================================================
*/

#include "r_typedefs.h"


/*
**====================================================================================
** Public type definitions
**====================================================================================
*/

typedef enum {
   errmgr_RESET,
   errmgr_LOG
} errmgr_SeverityLevel;


/*
**====================================================================================
** Public constant definitions for external access
**====================================================================================
*/

/**
*** The errmgr_ASSURE() error detection macro shall
*** be used in the same way as an if-statement.
**/
#define errmgr_ASSURE(level, test)                             \
   if (!(test)) {                                              \
      errmgr_userError(level, (U16)FILE_ID, (U16)__LINE__);    \
   } else

#define errmgr_ASSERT(level, test)                             \
   do {                                                        \
      if (!(test)) {                                           \
         errmgr_userError(level, (U8)FILE_ID, (U16)__LINE__);  \
      }                                                        \
   } while (0)


#define errmgr_CODE_ERROR(level)                               \
   errmgr_userError(level, (U16)FILE_ID, (U16)__LINE__)

/* Extra parameters not implemented: */
#define errmgr_ASSERT_P1(level, param1, test)  errmgr_ASSERT(level, test)
#define errmgr_ASSURE_P1(level, param1, test)  errmgr_ASSURE(level, test)
#define errmgr_CODE_ERROR_P1(level, param1)    errmgr_CODE_ERROR(level)

/*
**====================================================================================
** Function prototype declarations for external access
**====================================================================================
*/


/*************************************************************************************/
extern
void
errmgr_doInit(
      void);

/** Read out error information and clear it.
**************************************************************************************/


/*************************************************************************************/
extern
void
errmgr_userError(
      errmgr_SeverityLevel level,      /**< Severity level: RESET or LOG */
      U16 file_id,                     /**< Id of file */
      U16 line);                       /**< Code line in file */

/** User error management.
**************************************************************************************/


/*************************************************************************************/
extern
void
errmgr_illegalIrq(
      U32 exception);                  /**< Exception code */

/** Illegal irq management.
**************************************************************************************/


#endif


/************************************************************************************/
/** @}
************************************ End of file ************************************/
