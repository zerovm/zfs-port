/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Copyright 2006 Ricardo Correia.
 * Use is subject to license terms.
 */

#ifndef _SOL_THREAD_H
#define _SOL_THREAD_H

#include <pthread.h>
#include <assert.h>

/*
 * Compatibility thread stuff needed for Solaris -> Linux port
 */

typedef pthread_t thread_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;
typedef pthread_rwlock_t rwlock_t;

#define USYNC_THREAD 0

#ifdef ZVM_ENABLE

#define thr_self()               pthread_self()
#define thr_equal(a,b)           pthread_equal(a,b)
#define thr_join(t,d,s)          pthread_join(t,s)
#define thr_exit(r)              pthread_exit(r)
#define _mutex_init(l,f,a)       pthread_mutex_init(l,NULL)
#define _mutex_destroy(l)        pthread_mutex_destroy(l)
#define mutex_lock(l)            pthread_mutex_lock(l)
#define mutex_trylock(l)         pthread_mutex_trylock(l)
#define mutex_unlock(l)          pthread_mutex_unlock(l)
#define rwlock_init(l,f,a)       pthread_rwlock_init(l,NULL)
#define rwlock_destroy(l)        pthread_rwlock_destroy(l)
#define rw_rdlock(l)             pthread_rwlock_rdlock(l)
#define rw_wrlock(l)             pthread_rwlock_wrlock(l)
#define rw_tryrdlock(l)          pthread_rwlock_tryrdlock(l)
#define rw_trywrlock(l)          pthread_rwlock_trywrlock(l)
#define rw_unlock(l)             pthread_rwlock_unlock(l)
#define cond_init(l,f,a)         pthread_cond_init(l,NULL)
#define cond_destroy(l)          pthread_cond_destroy(l)
#define cond_wait(l,m)           pthread_cond_wait(l,m)
#define cond_signal(l)           pthread_cond_signal(l)
#define cond_broadcast(l)        pthread_cond_broadcast(l)

#else

#define thr_self()               1
#define thr_equal(a,b)           0
#define thr_join(t,d,s)          0
#define thr_exit(r)              0
#define _mutex_init(l,f,a)       0
#define _mutex_destroy(l)        0
#define mutex_lock(l)            0
#define mutex_trylock(l)         0
#define mutex_unlock(l)          0
#define rwlock_init(l,f,a)       0
#define rwlock_destroy(l)        0
#define rw_rdlock(l)             0
#define rw_wrlock(l)             0
#define rw_tryrdlock(l)          0
#define rw_trywrlock(l)          0
#define rw_unlock(l)             0
#define cond_init(l,f,a)         0
#define cond_destroy(l)          0
#define cond_wait(l,m)           0
#define cond_signal(l)           0
#define cond_broadcast(l)        0

#endif //ZVM_ENABLE

#define zfsfuse_thr_main()       (0)

#define THR_BOUND     0x00000001  /* = PTHREAD_SCOPE_SYSTEM */
#define THR_NEW_LWP   0x00000002
#define THR_DETACHED  0x00000040  /* = PTHREAD_CREATE_DETACHED */
#define THR_SUSPENDED 0x00000080
#define THR_DAEMON    0x00000100

static inline int thr_create(void *stack_base, size_t stack_size, void *(*start_func) (void*), void *arg, long flags, thread_t *new_thread_ID) {
#ifdef __native_client__
    start_func(arg);
    return 0;
#else
	assert(stack_base == NULL);
	assert(stack_size == 0);
	assert((flags & ~THR_BOUND & ~THR_DETACHED) == 0);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if(flags & THR_DETACHED)
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	/* This function ignores the THR_BOUND flag, since NPTL doesn't seem to support PTHREAD_SCOPE_PROCESS */

	int ret = pthread_create(new_thread_ID, &attr, start_func, arg);

	pthread_attr_destroy(&attr);
	return ret;
#endif
}

#endif
