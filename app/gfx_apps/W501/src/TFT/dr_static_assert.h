/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                              dr_static_assert.h                                 ***
***                                                                                 ***
***                                 Project Drome                                   ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup IMPORTS

   \Subject Static assert macro definitions.

   \Abstract  
       This header file contains definitions of static assert macros.

   \Version history record: 

   $Id: dr_static_assert.h 10 2018-12-03 11:36:39Z stoneridge $

   \{ 
***************************************************************************************
*/

#ifndef DR_STATIC_ASSERT_H
#define DR_STATIC_ASSERT_H


#define DR_STATIC_ASSERT_token( info_token, line_number)       \
   DR_STATIC_ASSERT_ ## line_number ## _ ## info_token

#define DR_STATIC_ASSERT_typedef( condition, info_token, line) \
   typedef unsigned char DR_STATIC_ASSERT_token( info_token, line)[ condition ? 1 : -1 ]

#define DR_STATIC_ASSERT( condition, info_token) \
   DR_STATIC_ASSERT_typedef( (condition), info_token, __LINE__)

#endif // DR_STATIC_ASSERT_H

/** \} 
*********************************** End of file **************************************/
