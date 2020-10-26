#ifndef _M5_H
#define _M5_H

typedef unsigned long m5_mutex_t;
typedef unsigned long m5_cond_t;
typedef unsigned long m5_mutexattr_t;	/* not used */
typedef unsigned long m5_condattr_t;	/* not used */

extern int m5_mutex_init(m5_mutex_t*, const m5_mutexattr_t*);
extern int m5_mutexattr_init(m5_mutexattr_t*);
extern int m5_mutex_lock(m5_mutex_t*);
extern int m5_mutex_trylock(m5_mutex_t*);
extern int m5_mutex_unlock(m5_mutex_t*);

extern int m5_cond_init(m5_cond_t*, const m5_condattr_t*);
extern int m5_condattr_init(m5_condattr_t*);
extern int m5_cond_signal(m5_cond_t*);
extern int m5_cond_broadcast(m5_cond_t*);
extern int m5_cond_wait(m5_cond_t*, m5_mutex_t*);

extern int m5_thread_exit();

#endif
