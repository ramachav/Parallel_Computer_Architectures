/*
 * File: pthreads/parmacs.h
 * Description: headers for pthreads based parmacs implementation.
 * Comments:
 * Copyright: Department of Computer Architecture - Polytechnic University of
 *   Catalonia (UPC)
 * Created: 1999/07 ernest@ac.upc.es (E.Artiaga)
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

#ifndef _PTHREADS_PARMACS_H_
#define _PTHREADS_PARMACS_H_

typedef parmacs_barrier *parmacs_barrier_t;
typedef parmacs_global  *parmacs_global_t;
typedef parmacs_event   *parmacs_event_t;
typedef parmacs_lock    *parmacs_lock_t;

void _parmacs_barinit(parmacs_barrier_t *);
void _parmacs_barrier(parmacs_barrier_t, int);

void _parmacs_gsinit(parmacs_global_t *);
void _parmacs_getsub(parmacs_global_t, int *, int, int);

void _parmacs_lockinit(parmacs_lock_t *);
void _parmacs_lock(parmacs_lock_t);
void _parmacs_unlock(parmacs_lock_t);
void _parmacs_alockinit(parmacs_lock_t *, int);
void _parmacs_alock(parmacs_lock_t, int);
void _parmacs_aulock(parmacs_lock_t, int);

void _parmacs_pauseinit(parmacs_event_t *, int);
void _parmacs_clearpause(parmacs_event_t, int);
void _parmacs_setpause(parmacs_event_t, int);
void _parmacs_event(parmacs_event_t, int);
void _parmacs_waitpause(parmacs_event_t, int);
void _parmacs_pause(parmacs_event_t, int);

void _parmacs_create(void (*)());
void _parmacs_wait_for_end(int);

void _parmacs_main_initenv(unsigned long int);
void _parmacs_main_end(void);

void *_parmacs_memory_alloc(int);
void _parmacs_memory_free(void *);

parmacs_pid _parmacs_get_pid(void);

parmacs_timestamp _parmacs_clock(void);

#endif /* _PTHREADS_PARMACS_H_ */
