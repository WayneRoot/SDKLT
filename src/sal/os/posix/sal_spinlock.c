/*! \file sal_spinlock.c
 *
 * Spinlock API.
 */
/*
 * Copyright: (c) 2018 Broadcom. All Rights Reserved. "Broadcom" refers to 
 * Broadcom Limited and/or its subsidiaries.
 * 
 * Broadcom Switch Software License
 * 
 * This license governs the use of the accompanying Broadcom software. Your 
 * use of the software indicates your acceptance of the terms and conditions 
 * of this license. If you do not agree to the terms and conditions of this 
 * license, do not use the software.
 * 1. Definitions
 *    "Licensor" means any person or entity that distributes its Work.
 *    "Software" means the original work of authorship made available under 
 *    this license.
 *    "Work" means the Software and any additions to or derivative works of 
 *    the Software that are made available under this license.
 *    The terms "reproduce," "reproduction," "derivative works," and 
 *    "distribution" have the meaning as provided under U.S. copyright law.
 *    Works, including the Software, are "made available" under this license 
 *    by including in or with the Work either (a) a copyright notice 
 *    referencing the applicability of this license to the Work, or (b) a copy 
 *    of this license.
 * 2. Grant of Copyright License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    copyright license to reproduce, prepare derivative works of, publicly 
 *    display, publicly perform, sublicense and distribute its Work and any 
 *    resulting derivative works in any form.
 * 3. Grant of Patent License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    patent license to make, have made, use, offer to sell, sell, import, and 
 *    otherwise transfer its Work, in whole or in part. This patent license 
 *    applies only to the patent claims licensable by Licensor that would be 
 *    infringed by Licensor's Work (or portion thereof) individually and 
 *    excluding any combinations with any other materials or technology.
 *    If you institute patent litigation against any Licensor (including a 
 *    cross-claim or counterclaim in a lawsuit) to enforce any patents that 
 *    you allege are infringed by any Work, then your patent license from such 
 *    Licensor to the Work shall terminate as of the date such litigation is 
 *    filed.
 * 4. Redistribution
 *    You may reproduce or distribute the Work only if (a) you do so under 
 *    this License, (b) you include a complete copy of this License with your 
 *    distribution, and (c) you retain without modification any copyright, 
 *    patent, trademark, or attribution notices that are present in the Work.
 * 5. Derivative Works
 *    You may specify that additional or different terms apply to the use, 
 *    reproduction, and distribution of your derivative works of the Work 
 *    ("Your Terms") only if (a) Your Terms provide that the limitations of 
 *    Section 7 apply to your derivative works, and (b) you identify the 
 *    specific derivative works that are subject to Your Terms. 
 *    Notwithstanding Your Terms, this license (including the redistribution 
 *    requirements in Section 4) will continue to apply to the Work itself.
 * 6. Trademarks
 *    This license does not grant any rights to use any Licensor's or its 
 *    affiliates' names, logos, or trademarks, except as necessary to 
 *    reproduce the notices described in this license.
 * 7. Limitations
 *    Platform. The Work and any derivative works thereof may only be used, or 
 *    intended for use, with a Broadcom switch integrated circuit.
 *    No Reverse Engineering. You will not use the Work to disassemble, 
 *    reverse engineer, decompile, or attempt to ascertain the underlying 
 *    technology of a Broadcom switch integrated circuit.
 * 8. Termination
 *    If you violate any term of this license, then your rights under this 
 *    license (including the license grants of Sections 2 and 3) will 
 *    terminate immediately.
 * 9. Disclaimer of Warranty
 *    THE WORK IS PROVIDED "AS IS" WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 *    KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WARRANTIES OR CONDITIONS OF 
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE OR 
 *    NON-INFRINGEMENT. YOU BEAR THE RISK OF UNDERTAKING ANY ACTIVITIES UNDER 
 *    THIS LICENSE. SOME STATES' CONSUMER LAWS DO NOT ALLOW EXCLUSION OF AN 
 *    IMPLIED WARRANTY, SO THIS DISCLAIMER MAY NOT APPLY TO YOU.
 * 10. Limitation of Liability
 *    EXCEPT AS PROHIBITED BY APPLICABLE LAW, IN NO EVENT AND UNDER NO LEGAL 
 *    THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE 
 *    SHALL ANY LICENSOR BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY DIRECT, 
 *    INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
 *    OR RELATED TO THIS LICENSE, THE USE OR INABILITY TO USE THE WORK 
 *    (INCLUDING BUT NOT LIMITED TO LOSS OF GOODWILL, BUSINESS INTERRUPTION, 
 *    LOST PROFITS OR DATA, COMPUTER FAILURE OR MALFUNCTION, OR ANY OTHER 
 *    COMMERCIAL DAMAGES OR LOSSES), EVEN IF THE LICENSOR HAS BEEN ADVISED OF 
 *    THE POSSIBILITY OF SUCH DAMAGES.
 */

#include <sys/select.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

#include <sal/sal_time.h>
#include <sal/sal_mutex.h>
#include <sal/sal_spinlock.h>

/* Default configuration */
#ifndef SAL_SPINLOCK_USE_POSIX_NATIVE
#  if defined(__linux__) && defined(__USE_XOPEN2K) && defined(__GLIBC__)
#    define SAL_SPINLOCK_USE_POSIX_NATIVE      1
#  endif
#endif
#ifndef SAL_SPINLOCK_USE_POSIX_NATIVE
#define SAL_SPINLOCK_USE_POSIX_NATIVE          0
#endif

/* Optionally dump configuration */
#include <sal/sal_internal.h>
#if SAL_INT_DUMP_CONFIG
#pragma message(SAL_INT_VAR_VALUE(SAL_SPINLOCK_USE_POSIX_NATIVE))
#endif


/* Local abstract type built on the POSIX spinlock */
#if (SAL_SPINLOCK_USE_POSIX_NATIVE)
typedef struct spinlock_ctrl_s {
    pthread_spinlock_t spinlock;
    char *desc;
} *spinlock_ctrl_t;
#endif


sal_spinlock_t
sal_spinlock_create(char *desc)
{
#if SAL_SPINLOCK_USE_POSIX_NATIVE
    spinlock_ctrl_t sl = malloc(sizeof(*sl));
    int rv;

    if (sl != NULL) {
        rv = pthread_spin_init(&(sl->spinlock), PTHREAD_PROCESS_SHARED);
        if (rv < 0) {
            free(sl);
            return (sal_spinlock_t)NULL;
        }
        sl->desc = desc;
    }
    return (sal_spinlock_t)sl;
#else
    return (sal_spinlock_t)sal_mutex_create(desc);
#endif
}

int
sal_spinlock_destroy(sal_spinlock_t lock)
{
#if SAL_SPINLOCK_USE_POSIX_NATIVE
    spinlock_ctrl_t sl = (spinlock_ctrl_t)lock;
    int rv;

    assert(sl);

    rv = pthread_spin_destroy(&sl->spinlock);
    free(sl);

    return rv;
#else
    sal_mutex_destroy((sal_mutex_t)lock);

    return 0;
#endif
}

int
sal_spinlock_lock(sal_spinlock_t lock)
{
#if (SAL_SPINLOCK_USE_POSIX_NATIVE)
    spinlock_ctrl_t sl = (spinlock_ctrl_t)lock;
    struct timeval tv;

    assert(sl);

    /*
     * On most systems pthread_spin_lock() will not block preemption,
     * which could lead to deadlock. To avoid this, we use
     * pthread_spin_trylock() instead.
     */
    while (pthread_spin_trylock(&sl->spinlock)) {
        tv.tv_sec = 0;
        tv.tv_usec = SECOND_USEC / sysconf(_SC_CLK_TCK);
        select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tv);
    }
    return 0;
#else
    return sal_mutex_take((sal_mutex_t)lock, SAL_MUTEX_FOREVER);
#endif
}

int
sal_spinlock_unlock(sal_spinlock_t lock)
{
#if (SAL_SPINLOCK_USE_POSIX_NATIVE)
    spinlock_ctrl_t sl = (spinlock_ctrl_t)lock;

    assert(sl);

    return pthread_spin_unlock(&sl->spinlock);
#else
    return sal_mutex_give((sal_mutex_t)lock);
#endif
}
