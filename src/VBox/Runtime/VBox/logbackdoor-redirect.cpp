/* $Id$ */
/** @file
 * Virtual Box Runtime - Guest Backdoor Logging, release logging for assertions.
 */

/*
 * Copyright (C) 2006-2007 innotek GmbH
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <VBox/log.h>
#include <iprt/asm.h>
#include <iprt/string.h>
#ifdef IN_GUEST_R3
# include <VBox/VBoxGuest.h>
#endif


/*******************************************************************************
*   Public Functions                                                         *
*******************************************************************************/

/* All release logging goes to the backdoor logger anyway. */
RTDECL(PRTLOGGER) RTLogRelDefaultInstance(void)
{
    return NULL;
}

/* All logging goes to the backdoor logger anyway. */
RTDECL(PRTLOGGER) RTLogDefaultInstance(void)
{
    return NULL;
}

RTDECL(void) RTLogRelPrintf(const char *pszFormat, ...)
{
    va_list args;

    va_start(args, pszFormat);
    RTLogBackdoorPrintfV(pszFormat, args);
    va_end(args);
}

RTDECL(void) RTLogRelPrintfV(const char *pszFormat, va_list args)
{
    RTLogBackdoorPrintfV(pszFormat, args);
}

RTDECL(void) RTLogPrintf(const char *pszFormat, ...)
{
    va_list args;

    va_start(args, pszFormat);
    RTLogBackdoorPrintfV(pszFormat, args);
    va_end(args);
}

RTDECL(void) RTLogPrintfV(const char *pszFormat, va_list args)
{
    RTLogBackdoorPrintfV(pszFormat, args);
}

/* Do nothing for now. */
RTDECL(void) RTLogFlush(PRTLOGGER)
{
}
