#include "m5.h"

int m5_mutex_init(m5_mutex_t *mutex, const m5_mutexattr_t *attr)
{
    *mutex = 0;
    return 0;
}

int m5_mutexattr_init(m5_mutexattr_t *attr)
{
    /* no-op */
    return 0;
}


int m5_cond_init(m5_cond_t *cond, const m5_condattr_t *attr)
{
    *cond = 0;
    return 0;
}

int m5_condattr_init(m5_condattr_t *attr)
{
    /* no-op */
    return 0;
}


int m5_cond_wait(m5_cond_t *cond, m5_mutex_t *mutex)
{
    /* do syscall to atomically unlock mutex and wait */
    m5_cond_wait_impl(cond);
    /* syscall does not re-acquire mutex: need to do it here */
    m5_mutex_lock(mutex);
    return 0;
}
