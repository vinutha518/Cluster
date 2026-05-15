# ****************************************************************************
# PROJECT : VLIB D1Lx
# FILE    : $Id: config_default.mk 3935 2014-09-23 08:52:32Z golczewskim $
# ============================================================================ 
# DESCRIPTION
# Umbrella Vlib default drivers 
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

#
# If no local config has been set, use the device default
#

ifeq ($(VLIB_SRC_MACROS),)
ifeq ($(VLIB_OBJ_MACROS),)
VLIB_DRIVER_CONFIG = rgl_src

VLIB_SRC_MACROS := 
VLIB_SRC_MACROS += dbg

VLIB_SRC_MACROS += ad_converter/adc7010  
VLIB_SRC_MACROS += audio/sg
VLIB_SRC_MACROS += audio/ssif             
VLIB_SRC_MACROS += crc/dcra              
VLIB_SRC_MACROS += csi/csig              
VLIB_SRC_MACROS += csi/csih              
VLIB_SRC_MACROS += csi/csisw             
VLIB_SRC_MACROS += dma/dma               
VLIB_SRC_MACROS += dma/g3dma             
VLIB_SRC_MACROS += dotmatrix/lcddogm     
VLIB_SRC_MACROS += ecm                   
VLIB_SRC_MACROS += flashc/sfma           
VLIB_SRC_MACROS += gfxbus           
VLIB_SRC_MACROS += gfxbus/xbus           
VLIB_SRC_MACROS += gpio                  
VLIB_SRC_MACROS += i2c/riic              
#VLIB_SRC_MACROS += memc/vram             
VLIB_SRC_MACROS += pwm/pcmp              
VLIB_SRC_MACROS += pwm/pwm7010           
VLIB_SRC_MACROS += tick                  
VLIB_SRC_MACROS += timer/awot            
VLIB_SRC_MACROS += timer/ostm            
VLIB_SRC_MACROS += timer/rtca            
VLIB_SRC_MACROS += timer/taub            
VLIB_SRC_MACROS += timer/tauj            
VLIB_SRC_MACROS += timer/wdta            
VLIB_SRC_MACROS += uart/rlin3            
#VLIB_SRC_MACROS += vo/ddb                
#VLIB_SRC_MACROS += vo/discom             
#VLIB_SRC_MACROS += vo/spea               
#VLIB_SRC_MACROS += vo/tcon               
#VLIB_SRC_MACROS += vdce               
#VLIB_SRC_MACROS += wm

endif
endif
