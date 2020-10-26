/*
 * File: pthreads/parmacs_types.h
 * Description: miscellaneous type definitions
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

#ifndef _PTHREADS_PARMACS_TYPES_H_
#define _PTHREADS_PARMACS_TYPES_H_

#include <m5.h>

#ifdef PARMACS_INTERNALS
/* Parmacs configuration */
typedef struct parmacs_configuration {
  char              *config_file;
  int                spin_count;
  int                max_barriers;
  int                max_globals;
  int                max_events;
  int                max_locks;
  int                page_size;
  int                stack_size;
  unsigned long int  shared_memory;
  int                memory_alignment;
  int                max_processes;
  int                kernel_limit;
  int                timestamp;
} parmacs_configuration;

typedef parmacs_configuration *parmacs_configuration_t;

#endif /* PARMACS_INTERNALS */

/* Barrier structure */
typedef struct parmacs_barrier {
  m5_mutex_t m;       /* mutual exclusion */
  m5_cond_t  c;       /* condition variable */
  volatile int    n;       /* number of threads waiting */
} parmacs_barrier;

/* Global subscript structure */
typedef struct parmacs_global {
  m5_mutex_t m;       /* mutual exclusion */
  m5_cond_t  c;       /* condition variable */
  volatile int    i;       /* next subscript */
  volatile int    n;       /* number of processes waiting */
} parmacs_global;

/* Pause/event structure */
typedef struct parmacs_event {
  m5_mutex_t m;       /* mutual exclusion */
  m5_cond_t  s;       /* condition variable - set event */
  m5_cond_t  c;       /* condition variable - clear event */
  volatile int    v;       /* event value */
} parmacs_event;

/* lock structure */
typedef struct parmacs_lock {
  m5_mutex_t m;     /* mutex */
} parmacs_lock;

/* Time stamps */
typedef unsigned long int parmacs_timestamp;

/* Processes */
typedef pthread_t parmacs_pid;

#ifdef PARMACS_INTERNALS
typedef struct parmacs_process {
  struct parmacs_process *link;     /* Queues */
  volatile int            ref_count;/* Reference counter */
  m5_mutex_t         m;        /* Mutual exclusion */
  m5_cond_t          c;        /* condition variable - wait for children */
  volatile int            children; /* Number of child processes */
  volatile int            finished; /* Number of finished children */
  struct parmacs_process *parent;   /* Parent process */
  void         (*routine)();        /* code to execute */
} parmacs_process;

typedef parmacs_process *parmacs_process_t;

/* Parmacs control structure */
#define PARMACS_MAIN_ID    0
#define PARMACS_CONFIG_ID  1
#define PARMACS_MEMORY_ID  2
#define PARMACS_BARRIER_ID 3
#define PARMACS_GLOBAL_ID  4
#define PARMACS_EVENT_ID   5
#define PARMACS_LOCK_ID    6
#define PARMACS_PROCESS_ID 7
#define PARMACS_MAX_ID     8
typedef struct parmacs_data {
  parmacs_timestamp start_time;  /* Starting time */
  parmacs_timestamp end_time;    /* Ending time */
  pthread_key_t     private;     /* Private data area per pthread */
  parmacs_barrier  *barrier;     /* Barrier structures */
  int               n_barriers;
  parmacs_global   *global;      /* Global subscript structures */
  int               n_globals;
  parmacs_event    *event;       /* Event structures */
  int               n_events;
  parmacs_lock     *lock;        /* Lock structures */
  int               n_locks;
  parmacs_process  *process;     /* Parmacs processes */
  parmacs_process_t free_process;/* Free processes queue */
  m5_mutex_t  *mutex;       /* Semaphores for mutual exclusions */
} parmacs_data;

typedef parmacs_data *parmacs_data_t;

#endif /* PARMACS_INTERNALS */

#endif /* _PTHREADS_PARMACS_TYPES_H_ */
