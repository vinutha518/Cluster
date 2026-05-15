# ****************************************************************************
# PROJECT : TBTF
# FILE    : $Id: Makefile 11006 2016-11-07 16:32:44Z michael.golczewski $
# AUTHOR  : $Author: michael.golczewski $
# ============================================================================ 
# DESCRIPTION
# The Big Transformer 
# ============================================================================
#                             C O P Y R I G H T                                    
# ============================================================================
#                            Copyright (c) 2007
#                                   by 
#                        NEC Electronics (Europe) GmbH. 
#                            Arcadiastrasse 10
#                           D-40472 Duesseldorf
#                                Germany
#                           All rights reserved.
# ============================================================================
# Purpose: only for testing
# 
# Warranty Disclaimer
# 
# Because the Product(s) is licensed free of charge, there is no warranty 
# of any kind whatsoever and expressly disclaimed and excluded by NEC, 
# either expressed or implied, including but not limited to those for 
# non-infringement of intellectual property, merchantability and/or 
# fitness for the particular purpose. 
# NEC shall not have any obligation to maintain, service or provide bug 
# fixes for the supplied Product(s) and/or the Application.
# 
# Each User is solely responsible for determining the appropriateness of 
# using the Product(s) and assumes all risks associated with its exercise 
# of rights under this Agreement, including, but not limited to the risks 
# and costs of program errors, compliance with applicable laws, damage to 
# or loss of data, programs or equipment, and unavailability or 
# interruption of operations.
# 
# Limitation of Liability
# 
# In no event shall NEC be liable to the User for any incidental, 
# consequential, indirect, or punitive damage (including but not limited 
# to lost profits) regardless of whether such liability is based on breach 
# of contract, tort, strict liability, breach of warranties, failure of 
# essential purpose or otherwise and even if advised of the possibility of 
# such damages. NEC shall not be liable for any services or products 
# provided by third party vendors, developers or consultants identified or
# referred to the User by NEC in connection with the Product(s) and/or the 
# Application.
# ****************************************************************************


##############################################################################
#              Application configuration (mandatory part)
##############################################################################
# This is the name of your application subdirectory and will be used for the project
# files as well. Its directory name wil be used for project and *.out file naming 

VLIB_ROOT := $(firstword $(shell pwd | sed -re '{s/[Vv]-?[Ll][Ii][Bb][0-9]?/& /;}'))

include $(VLIB_ROOT)/compiler/setup.mk

VLIB_APP      	:= tbtf.out
VLIB_COMPILER 	:= x86_gnu
VLIB_DEVICE   	:= x86


#
# Application
#

include $(VLIB_ROOT)/utils/tbtf/tbtf.mk


#
# Compiler 
#

include $(VLIB_ROOT)/compiler/compiler.mk


