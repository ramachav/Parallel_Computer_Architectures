/*
 * File: pthreads/parmacs_sync.c
 * Description: implementation of synchronization macros of PARMACS
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

#include <parmacs_internals.h>


/*
 * Barriers
 */

void
_parmacs_barinit(parmacs_barrier_t *barrier)
{
  m5_mutexattr_t m_attr;
  m5_condattr_t  c_attr;
  
  /* get a free barrier entry */
  _parmacs_internal_lock(PARMACS_BARRIER_ID);
  if (_parmacs_data->n_barriers >= _parmacs_config->max_barriers) {
    _parmacs_internal_unlock(PARMACS_BARRIER_ID);
    _parmacs_panic("parmacs: too many barriers!");
  }
  *barrier = &_parmacs_data->barrier[_parmacs_data->n_barriers++];
  _parmacs_internal_unlock(PARMACS_BARRIER_ID);
  
  /* now, initialize the new barrier */
  m5_mutexattr_init(&m_attr);
  m5_condattr_init(&c_attr);
  m5_mutex_init(&((*barrier)->m), &m_attr);
  m5_cond_init(&((*barrier)->c), &c_attr);
  (*barrier)->n = 0;
}
 
void 
_parmacs_barrier(parmacs_barrier_t barrier, int procs)
{
  m5_mutex_lock(&(barrier->m));
  if (++(barrier->n) < procs) {
    m5_cond_wait(&(barrier->c), &(barrier->m));
  } else {
    barrier->n = 0;
    m5_cond_broadcast(&(barrier->c));
  }
  m5_mutex_unlock(&(barrier->m));
}

/*
 * Global subscripts
 */
 
void
_parmacs_gsinit(parmacs_global_t *global)
{
  m5_mutexattr_t m_attr;
  m5_condattr_t  c_attr;

  /* get a free global subscript entry */
  _parmacs_internal_lock(PARMACS_GLOBAL_ID);
  if (_parmacs_data->n_globals >= _parmacs_config->max_globals) {
    _parmacs_internal_unlock(PARMACS_GLOBAL_ID);
    _parmacs_panic("parmacs: too many global subscripts!");
  }
  *global = &_parmacs_data->global[_parmacs_data->n_globals++];
  _parmacs_internal_unlock(PARMACS_GLOBAL_ID);
  
  /* now, initialize the new global subscript */
  m5_mutexattr_init(&m_attr);
  m5_condattr_init(&c_attr);
  m5_mutex_init(&((*global)->m), &m_attr);
  m5_cond_init(&((*global)->c), &c_attr);
  (*global)->i = 0;
  (*global)->n = 0;
}

void
_parmacs_getsub(parmacs_global_t global, int *next, int last, int procs)
{
  m5_mutex_lock(&(global->m));
  if (global->i <= last) {
    *next = (global->i)++;
  } else {
    *next = -1;
    if (++(global->n) == procs) {
      global->i = 0;
      global->n = 0;
      m5_cond_broadcast(&(global->c));
    } else {
      m5_cond_wait(&(global->c), &(global->m));
    }
  }
  m5_mutex_unlock(&(global->m));
}

/*
 * Locks
 */

void 
_parmacs_lockinit(parmacs_lock_t *lock)
{
  m5_mutexattr_t m_attr;
  
  /* get a free lock entry */
  _parmacs_internal_lock(PARMACS_LOCK_ID);
  if (_parmacs_data->n_locks >= _parmacs_config->max_locks) {
    _parmacs_internal_unlock(PARMACS_LOCK_ID);
    _parmacs_panic("parmacs: too many locks!");
  }
  *lock = &_parmacs_data->lock[_parmacs_data->n_locks++];
  _parmacs_internal_unlock(PARMACS_LOCK_ID);
  
  /* now, initialize the new lock */
  m5_mutexattr_init(&m_attr);
  m5_mutex_init(&((*lock)->m), &m_attr);
}

void
_parmacs_lock(parmacs_lock_t lock)
{
  _parmacs_sem_lock(&(lock->m));
}

void
_parmacs_unlock(parmacs_lock_t lock)
{
  _parmacs_sem_unlock(&(lock->m));
}

void
_parmacs_alockinit(parmacs_lock_t *lock, int amount)
{
  m5_mutexattr_t m_attr;
  register int i;
  
  /* get a free lock entry */
  _parmacs_internal_lock(PARMACS_LOCK_ID);
  if ((_parmacs_data->n_locks + amount -1) >= _parmacs_config->max_locks) {
    _parmacs_internal_unlock(PARMACS_LOCK_ID);
    _parmacs_panic("parmacs: too many locks!");
  }
  *lock = &_parmacs_data->lock[_parmacs_data->n_locks];
  _parmacs_data->n_locks += amount;
  _parmacs_internal_unlock(PARMACS_LOCK_ID);
  
  /* now, initialize the new locks */
  m5_mutexattr_init(&m_attr);
  for (i=0; i < amount; i++) {
    m5_mutex_init(&((*lock)[i].m), &m_attr);    
  }
}

void
_parmacs_alock(parmacs_lock_t lock, int idx)
{
  _parmacs_sem_lock(&(lock[idx].m));
}

void
_parmacs_aulock(parmacs_lock_t lock, int idx)
{
  _parmacs_sem_unlock(&(lock[idx].m));
}

/*
 * Events
 */
 
void
_parmacs_pauseinit(parmacs_event_t *event, int amount)
{
  register int i;
  m5_condattr_t c_attr;
  m5_mutexattr_t m_attr;
  
  /* get the free event entries */
  _parmacs_internal_lock(PARMACS_EVENT_ID);
  if ((_parmacs_data->n_events + amount -1) >= _parmacs_config->max_events) {
    _parmacs_internal_unlock(PARMACS_EVENT_ID);
    _parmacs_panic("parmacs: too many events!");
  }
  *event = &_parmacs_data->event[_parmacs_data->n_events];
  _parmacs_data->n_events += amount;
  _parmacs_internal_unlock(PARMACS_EVENT_ID);
  
  /* initialize the event entries */
  m5_condattr_init(&c_attr);
  m5_mutexattr_init(&m_attr);
  for(i=0; i < amount; i++) {
    m5_mutex_init(&((*event)[i].m), &m_attr);
    m5_cond_init(&((*event)[i].c), &c_attr);
    m5_cond_init(&((*event)[i].s), &c_attr);
    (*event)[i].v = 0;
  }
}

void
_parmacs_clearpause(parmacs_event_t event, int idx)
{
  m5_mutex_lock(&(event[idx].m));
  event[idx].v = 0;
  m5_cond_broadcast(&(event[idx].c));
  m5_mutex_unlock(&(event[idx].m));
}

void
_parmacs_setpause(parmacs_event_t event, int idx)
{
  m5_mutex_lock(&(event[idx].m));
  event[idx].v = 1;
  m5_cond_broadcast(&(event[idx].s));
  m5_mutex_unlock(&(event[idx].m));
}

void
_parmacs_event(parmacs_event_t event, int idx)
{
  m5_mutex_lock(&(event[idx].m));
  while (event[idx].v == 1) {
    m5_cond_wait(&(event[idx].c), &(event[idx].m));
  }
  event[idx].v = 1;
  m5_cond_broadcast(&(event[idx].s));
  m5_mutex_unlock(&(event[idx].m));
}


void
_parmacs_waitpause(parmacs_event_t event, int idx)
{
  m5_mutex_lock(&(event[idx].m));
  if (event[idx].v == 0) {
    m5_cond_wait(&(event[idx].s), &(event[idx].m));
  }
  m5_mutex_unlock(&(event[idx].m));
}

void
_parmacs_pause(parmacs_event_t event, int idx)
{
  m5_mutex_lock(&(event[idx].m));
  while (event[idx].v == 0) {
    m5_cond_wait(&(event[idx].s), &(event[idx].m));
  }
  event[idx].v = 0;
  m5_cond_broadcast(&(event[idx].c));
  m5_mutex_unlock(&(event[idx].m));
}  
  
  
