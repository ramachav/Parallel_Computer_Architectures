/*
 * File: pthreads/parmacs_decl.h
 * Description: miscellaneous variable declarations and function prototypes
 * Comments: 
 * Copyright: Department of Computer Architecture - Polytechnic University of
 *   Catalonia (UPC)
 * Created: 1999/06 ernest@ac.upc.es (E.Artiaga)
 * Contact: gsomk@ac.upc.es
 * Last update:
 * History:
 */
/*
 * Disclaimer and copying policy:
 *
 * The Polytechnic University of Catalonia (UPC) and the authors disclaim
 * all warranties with regard to this software, including all implied
 * warranties of merchantability and fitness.
 * In no event shall the Polytechnic University of Catalonia (UPC) or the
 * authors be liable for any special, indirect or consequential damages or
 * any damages whatsoever resulting from loss of use, data or profits,
 * whether in an action of contract, negligence or other tortius action,
 * arising out of or in connection with the use or performance of this
 * software.
 *
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice and disclaimer, and also this permission notice, appear
 * in all copies.
 *
 */

#ifndef _PTHREADS_PARMACS_DECL_H_
#define _PTHREADS_PARMACS_DECL_H_

#include <m5.h>

/* Global parmacs control data */
#ifdef PARMACS_INTERNALS
extern parmacs_data_t _parmacs_data;
extern parmacs_configuration_t _parmacs_config;

/*
 * Enable compatability with old pthreads interface (POSIX_1003_4a)
 */
#ifdef POSIX_1003_4a

#define _posix_create(x,y,z,t) pthread_create(x,*(y),z,t)
#define _posix_detach(x) pthread_detach(&(x))
#define _posix_self() pthread_self()
#define _posix_attr_init(x) pthread_attr_create(x)
#define _posix_key_create(x,y) pthread_keycreate(x,y)
#define _posix_getspecific(x,y) pthread_getspecific(x,y)
#define _posix_setspecific(x,y) pthread_setspecific(x,y)
#define _posix_setconcurrency(x)

#else /* POSIX_1003_4a */

#define _posix_create(x,y,z,t) pthread_create(x,y,z,t)
#define _posix_detach(x) pthread_detach(x)
#define _posix_self() pthread_self()
#define _posix_attr_init(x) pthread_attr_init(x)
#define _posix_key_create(x,y) pthread_key_create(x,y)
#define _posix_getspecific(x,y) (*(y) = pthread_getspecific(x))
#define _posix_setspecific(x,y) pthread_setspecific(x,y)
#define _posix_setconcurrency(x) pthread_setconcurrency(x)

#endif /* POSIX_1003_4a */

/* inline functions */

/* 
 * Mutual exclusion - internal use only
 */

#ifdef PARMACS_SPIN_LOCK
#ifdef __GNUC__
static __inline__ void _parmacs_sem_lock(m5_mutex_t *);
static __inline__ void _parmacs_sem_lock(m5_mutex_t *mutex)
{
  register int countdown;
  register int spin;
  
  countdown = spin = _parmacs_config->spin_count;
  while(!_poxix_mutex_trylock(mutex)) {
    if (countdown >= 0) {
      if (!countdown) {
	_parmacs_yield();
	countdown = spin;
      } else {
	countdown --;
      }
    }
  }
}

#else /* __GNUC__ */

#define _parmacs_sem_lock(__mutex__)                                \
do {                                                                \
  register int __countdown__;                                       \
  register int __spin__;                                            \
                                                                    \
  __countdown__ = __spin__ = _parmacs_config->spin_count;           \
  while(!m5_mutex_trylock(__mutex__)) {                         \
    if (__countdown__ >= 0) {                                       \
      if (!__countdown__) {                                         \
        _parmacs_yield();                                           \
        __countdown__ = __spin__;                                   \
      } else {                                                      \
        __countdown__ --;                                           \
      }                                                             \
    }                                                               \
  }                                                                 \
} while (0)

#endif /* _GNUC_ */

#else /* PARMACS_SPIN_LOCK */
#define _parmacs_sem_lock(x) m5_mutex_lock(x)
#endif /* PARMACS_SPIN_LOCK */

#define _parmacs_sem_unlock(x) m5_mutex_unlock(x)

#define _parmacs_internal_lock(__mutex__)     \
  _parmacs_sem_lock(&(_parmacs_data->mutex[__mutex__]))

#define _parmacs_internal_unlock(__mutex__)   \
  _parmacs_sem_unlock(&(_parmacs_data->mutex[__mutex__]))

#endif /* PARMACS_INTERNALS */

/*
 * Function prototypes
 */

void *_parmacs_local_alloc(int);
void  _parmacs_local_free(void *);

void  _parmacs_panic(char *);
void  _parmacs_exit(void);
void  _parmacs_yield(void);

#endif /* _PTHREADS_PARMACS_DECL_H_ */
