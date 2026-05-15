/*
****************************************************************************
PROJECT : PWM driver
FILE    : $Id: r_pwm7010_regs.h 4016 2014-10-01 11:20:52Z golczewskim $
============================================================================
DESCRIPTION
Driver for PWM macro
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2012
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

#ifndef  PWM_REGS_H_
#define  PWM_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: PWM7010 Register Offsets

  Only the PWM driver shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: Register Offsets
  
  The Register Offsets of PWM7010

   **  PWM Clock Divider Register PWBA  **
   
  R_PWBA_BRS(m)      -  BRS Register  m                       000H  + m * 4H        
  R_PWBA_TE          -  Timer Enable Status Register          010H            
  R_PWBA_TS          -  Start Trigger Register                014H      
  R_PWBA_TT          -  Stop Trigger Register                 018H  
  R_PWBA_EMU         -  emulation Register                    01cH              
 
   **  PWM Generator Register PWGA  ** 
        
  R_PWGA_RSF         -  Simultaneous Rewrite Status Register  010H            
  R_PWGA_CNT         -  Counter Register                      014H              
  R_PWGA_CSBR        -  Raising Edge Buffer Register          024H              
  R_PWGA_CRBR        -  Falling Edge Buffer Register          028H          
  R_PWGA_CTBR        -  Trigger Buffer Register               02cH              
  R_PWGA_TE          -  Timer(PWBA) Enable Status Register    030H
  R_PWGA_CSDR        -  Raising Edge Register                 000H          
  R_PWGA_CRDR        -  Falling Edge Register                 004H          
  R_PWGA_CTDR        -  Trigger Register                      008H              
  R_PWGA_RDT         -  Simultaneous Rewrite Tirgger Register 00cH            
  R_PWGA_CTL         -  Control Register                      020H              
  R_PWGA_TS          -  Start Trigger Register                034H
  R_PWGA_TT          -  Stop Trigger Register                 038H              
  R_PWGA_SLPWGA(m)   -  Simultaneous Trigger Register 0-2     R_PWM_SLPWGA0 + m * 4H
                        (for Multiple Channels)

   **  PWM-Diag Control Function register PWSA  **    
           
  R_PWSA_QUE(m)    -  Encoded Trigger Number Register         020H  + m * 4H   
  R_PWSA_STR       -  Trigger Queue Status Register           004H   
  R_PWSA_CTL       -  Control Register                        000H
  R_PWSA_STC       -  Trigger Queue Status Clear Register     008H
  R_PWSA_PVCR(m)   -  PVCR Register                           040H  + m * 2H    

*/
  
#define R_PWBA_BRS(m)           (0x000 + (##m## * 4))
#define R_PWBA_TE               (0x010)
#define R_PWBA_TS               (0x014)
#define R_PWBA_TT               (0x018)
#define R_PWBA_EMU              (0x01c)



#define R_PWGA_RSF              (0x010)
#define R_PWGA_CNT              (0x014)
#define R_PWGA_CSBR             (0x024)
#define R_PWGA_CRBR             (0x028)
#define R_PWGA_CTBR             (0x02c)
#define R_PWGA_TE               (0x030)

#define R_PWGA_CSDR             (0x000)
#define R_PWGA_CRDR             (0x004)
#define R_PWGA_CTDR             (0x008)
#define R_PWGA_RDT              (0x00c)
#define R_PWGA_CTL              (0x020)
#define R_PWGA_TS               (0x034)
#define R_PWGA_TT               (0x038)
/* SLPWGA base address is device dependent  
#define R_PWGA_SLPWGA(m)        (0xffbc1000 + (##m## * 4))*/
#define R_PWGA_SLPWGA(m)        (R_PWM_SLPWGA0 + (##m## * 4))


#define R_PWSA_QUE(m)           (0x020 + (##m## * 4))
#define R_PWSA_STR              (0x004)

#define R_PWSA_CTL              (0x000)
#define R_PWSA_STC              (0x008)
#define R_PWSA_PVCR(m)          (0x040 + (##m## * 4))





#ifdef __cplusplus
}
#endif

#endif /* _PWM_REGS_H_  */
