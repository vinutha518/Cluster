# ****************************************************************************
# PROJECT : VLIB
# FILE    : $Id: Makefile 4317 2014-11-10 15:03:50Z miodrag.stankovic $
# ============================================================================
# DESCRIPTION
# Makefile for dave hd static drawinglist application
# ============================================================================
#                             C O P Y R I G H T                                    
# ============================================================================
#                            Copyright (c) 2014
#                                   by 
#                        Renesas Electronics (Europe) GmbH. 
#                            Arcadiastrasse 10
#                           D-40472 Duesseldorf
#                                Germany
#                           All rights reserved.
# ============================================================================
# Purpose: only for testing
# 
# DISCLAIMER                                                                   
# This software is supplied by Renesas Electronics Corporation and is only     
# intended for use with Renesas products. No other uses are authorized. This   
# software is owned by Renesas Electronics Corporation and is protected under  
# all applicable laws, including copyright laws.                               
# THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
# THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
# LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
# AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
# TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
# ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
# FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
# ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
# BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
# Renesas reserves the right, without notice, to make changes to this software 
# and to discontinue the availability of this software. By using this software,
# you agree to the additional terms and conditions found by accessing the      
# following link:                                                              
# http://www.renesas.com/disclaimer *                                          
# Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     
# ****************************************************************************

##############################################################################
#              Application configuration (mandatory part)
##############################################################################
# This is the name of your application subdirectory and will be used for the project
# files as well. Its directory name wil be used for project and *.out file naming 

VLIB_ROOT := $(firstword $(shell pwd | sed -re '{s/[Vv]-?[Ll][Ii][Bb][0-9]?/& /;}'))

include $(VLIB_ROOT)/compiler/setup.mk

VLIB_APP      		:= d1m1_flashdash_demo.out
VLIB_COMPILER 		:= rh850_ghs
VLIB_DEVICE   		:= d1mx
VLIB_BOARD    		:= d1mx_mango

VLIB_RENESAS_MISRA_STRICT	:= no
VLIB_BUILD_LOG := on

VLIB_MIDDLEWARE := gfxutils
VLIB_CFLAGS       += -Odebug -G
VLIB_LDFLAGS      += -G

#VLIB_DEFINE += BSP_INIT_SFMA BSP_INIT_QOS_VRAM
VLIB_DEFINE += BSP_INIT_SFMA_RAW BSP_INIT_QOS_VRAM 
##############################################################################
#              Application configuration (optional part)
##############################################################################
#
#DRIVER_CONFIG set to default (use config_default.mk) if not changed here 
#Possible options:
#
#  rgl
#  max
#  default
#
ifeq ($(VLIB_DRIVER_CONFIG),)
VLIB_DRIVER_CONFIG = rgl_obj
endif
ifeq ($(VLIB_DRIVER_OPTIMIZE),yes)
VLIB_CFLAGS       += -Ospeed
endif
ifeq ($(VLIB_DRIVER_DEBUG),)
VLIB_DEBUG_BUILD   := yes
else
VLIB_DEBUG_BUILD   := no
endif



#Add here  bsp extensions 
VLIB_BSP_DRIVER	:= stdio
VLIB_BSP_DRIVER	+= hmi
VLIB_BSP_DRIVER	+= gfx

VLIB_INC   += $(VLIB_ROOT)/app/gfx_apps/d1m1_flashdash_demo/target/d1m2_mango_ghs/config

##############################################################################
# All further processing is intiated by the file below
# no need to bother about ;) 
#
# "parent" makefile
#
include $(VLIB_ROOT)/app/gfx_apps/d1m1_flashdash_demo/d1m1_flashdash_demo.mk
