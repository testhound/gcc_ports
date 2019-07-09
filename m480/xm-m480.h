/* $Id: xm-m480.h,v 1.1 2002/10/28 21:14:49 yazlovy Exp $
 *============================================================================
 *
 * RESTRICTED RIGHTS LEGEND
 * 
 * Use, duplication, or disclosure is subject to restrictions.
 * 
 * Unpublished Work Copyright (C) 1998 Maker Communications Inc.
 * All Rights Reserved.
 * 
 * This computer program is the property of Maker Communications and contains
 * its confidential trade secrets.  Use, examination, copying, transfer and
 * disclosure to others, in whole or in part, are prohibited except with the
 * express prior written consent of Maker Communications.
 * 
 *============================================================================
 * 
 * AUTHOR: Wilson Snyder
 * DESCRIPTION: MXT4400 Compiler
 * 
 *============================================================================
 * This file supports "bare" operating system for the elsy compiler.
 *============================================================================
 */

/* #defines that need visibility everywhere.  */
#define FALSE 0
#define TRUE 1

/* This describes the machine the compiler is hosted on.  */
#define HOST_BITS_PER_CHAR 8
#define HOST_BITS_PER_SHORT 16
#define HOST_BITS_PER_INT 32
#define HOST_BITS_PER_LONG 32
#define HOST_BITS_PER_LONGLONG 32

/* target machine dependencies.
   tm.h is a symbolic link to the actual target specific file.   */
#include "tm.h"

/* Arguments to use with `exit'.  */
#define SUCCESS_EXIT_CODE 0
#define FATAL_EXIT_CODE 33
