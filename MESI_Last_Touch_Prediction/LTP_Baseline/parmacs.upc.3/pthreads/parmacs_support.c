/*
 * File: pthreads/parmacs_support.c
 * Description: implementation of support code for pthreads based PARMACS
 *   macros. Also define global var definitions.
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
 * Data structures
 */
 
parmacs_data_t _parmacs_data = (parmacs_data_t)0;
parmacs_configuration _parmacs_configuration = {
  (char *)0,
  PARMACS_SPIN_COUNT,
  PARMACS_MAX_BARRIER,
  PARMACS_MAX_GLOBAL,
  PARMACS_MAX_EVENT,
  PARMACS_MAX_LOCK,
  PARMACS_PAGE_SIZE,
  PARMACS_STACK_SIZE,
  PARMACS_SHARED_MEMORY,
  PARMACS_MEM_ALIGNMENT,
  PARMACS_MAX_PROCESS,
  PARMACS_KERNEL_LIMIT,
  PARMACS_TIMESTAMP
};
parmacs_configuration_t _parmacs_config = &_parmacs_configuration;

static void _parmacs_configure(unsigned long int);
static void *_parmacs_process_wrapper(void *);

/*
 * Initialize environment
 * Comments:
 *   This routine is supposed to be called from the main routine, at the
 *   very beginning of the execution.
 */
void
_parmacs_main_initenv(unsigned long int shm_size)
{
  register void *address;
  register int i;
  register parmacs_process_t myself;
  m5_mutexattr_t m_attr;
  m5_condattr_t c_attr;
  
  /* Load configuration */
  _parmacs_configure(shm_size);
  m5_mutexattr_init(&m_attr);
  m5_condattr_init(&c_attr);
  
  /* Set concurrency level */
#ifndef PARMACS_NO_SETCONCURRENCY
  _posix_setconcurrency(_parmacs_config->kernel_limit);
#endif /* PARMACS_NO_SETCONCURRENCY */
   
  /* Allocate memory for parmacs data */
  /* MAIN DATA */
  address = malloc(sizeof(parmacs_data));
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate memory for parmacs data!");
  }
  _parmacs_data = (parmacs_data_t)address;
  
  /* Initialize thread private areas */
  _posix_key_create(&(_parmacs_data->private), NULL);
  
  /* Initialize mutex locks */
  address = malloc(sizeof(m5_mutex_t)*PARMACS_MAX_ID);
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate semaphores for mutual exclusion!");
  }
  _parmacs_data->mutex = address;
  for (i=0; i < PARMACS_MAX_ID; i++) {
    m5_mutex_init(&(_parmacs_data->mutex[i]), &m_attr);
  }
    
  /* BARRIERS */
  address = malloc(_parmacs_config->max_barriers*sizeof(parmacs_barrier));
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate memory for barriers!");
  }
  _parmacs_data->barrier = address;
  _parmacs_data->n_barriers = 0;
  
  /* GLOBAL SUBSCRIPTS */
  address = malloc(_parmacs_config->max_globals*sizeof(parmacs_global));
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate memory for global subscripts!");
  }
  _parmacs_data->global = address;
  _parmacs_data->n_globals = 0;
  
  /* EVENTS */
  address = malloc(_parmacs_config->max_events*sizeof(parmacs_event));
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate memory for events!");
  }
  _parmacs_data->event = address;
  _parmacs_data->n_events = 0;
  
  /* LOCKS */
  address = malloc(_parmacs_config->max_locks*sizeof(parmacs_lock));
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate memory for locks!");
  }
  _parmacs_data->lock = address;
  _parmacs_data->n_locks = 0;
  
  /* PROCESSES */
  address = malloc(_parmacs_config->max_processes*sizeof(parmacs_process));
  if (!address) {
    _parmacs_panic("parmacs: cannot allocate memory for processes!");
  }
  _parmacs_data->process = address;
  for(i=1; i < (_parmacs_config->max_processes-1); i++) {
    _parmacs_data->process[i].link = &(_parmacs_data->process[i+1]);
    _parmacs_data->process[i].ref_count = 0;
  }
  _parmacs_data->process[_parmacs_config->max_processes-1].link =
      (parmacs_process_t)0;
  _parmacs_data->process[_parmacs_config->max_processes-1].ref_count = 0;
  _parmacs_data->free_process = &(_parmacs_data->process[1]);
  myself = &(_parmacs_data->process[0]);
  myself->ref_count = 1;
  myself->children  = 0;
  myself->finished  = 0;
  myself->parent    = (parmacs_process_t)0;
  m5_mutex_init(&(myself->m), &m_attr);
  m5_cond_init(&(myself->c), &c_attr);
  _posix_setspecific(_parmacs_data->private,(void *)myself);
  
  /*
   * Time stamp:
   */
  if (_parmacs_config->timestamp) {
    struct tms now;
    _parmacs_data->start_time = times(&now);
  }
}

void
_parmacs_main_end()
{
  /*
   * Time stamp:
   */
  if (_parmacs_config->timestamp) {
    struct tms now;
    _parmacs_data->end_time = times(&now);
    fprintf(stderr, "\nPARMACS TIME: start: %lu, end: %lu, elapsed: %lu.\n",
            _parmacs_data->start_time,
            _parmacs_data->end_time,
            _parmacs_data->end_time - _parmacs_data->start_time);
  }
  _parmacs_exit();
}

static void
_parmacs_configure(unsigned long int shm_size)
{
  extern char **environ;
  char **my_environment;
  char * var_name;

  _parmacs_configuration.shared_memory = shm_size;
  
  /* Read configuration from environment */
  
  for(my_environment = environ; *my_environment; my_environment++) {
    var_name = strtok(strdup(*my_environment), "=");
    if (!strcmp(var_name, "PARMACS_CONFIG_FILE")) {
      _parmacs_configuration.config_file = strdup(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_SPIN_COUNT")) {
      _parmacs_configuration.spin_count = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_MAX_BARRIER")) {
      _parmacs_configuration.max_barriers = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_MAX_GLOBAL")) {
      _parmacs_configuration.max_globals = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_MAX_EVENT")) {
      _parmacs_configuration.max_events = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_MAX_LOCK")) {
      _parmacs_configuration.max_locks = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_PAGE_SIZE")) {
      _parmacs_configuration.page_size = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_STACK_SIZE")) {
      _parmacs_configuration.stack_size = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_SHARED_MEMORY")) {
      char *endptr;
      _parmacs_configuration.shared_memory = 
        strtoul(strtok(NULL, "\0"), &endptr, 10);
    } else if (!strcmp(var_name, "PARMACS_MEM_ALIGNMENT")) {
      _parmacs_configuration.memory_alignment = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_MAX_PROCESS")) {
      _parmacs_configuration.max_processes = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_KERNEL_LIMIT")) {
      _parmacs_configuration.kernel_limit = atoi(strtok(NULL, "\0"));
    } else if (!strcmp(var_name, "PARMACS_TIMESTAMP")) {
      _parmacs_configuration.timestamp = atoi(strtok(NULL, "\0"));
    }
    free(var_name);  
  }
  
  /* if there is a configuration file, use it */
  if (_parmacs_configuration.config_file) {
    char buffer[512];
    FILE *fd;

    if (!(fd = fopen(_parmacs_configuration.config_file, "r"))) {
      _parmacs_panic("parmacs: cannot open configuration file!");
    }    
    while (fgets(buffer, 512, fd)) {
      var_name = strtok(buffer, " =\t\n\0");
      if (!strcmp(var_name, "PARMACS_SPIN_COUNT")) {
        _parmacs_configuration.spin_count = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_MAX_BARRIER")) {
        _parmacs_configuration.max_barriers = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_MAX_GLOBAL")) {
        _parmacs_configuration.max_globals = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_MAX_EVENT")) {
        _parmacs_configuration.max_events = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_MAX_LOCK")) {
        _parmacs_configuration.max_locks = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_PAGE_SIZE")) {
        _parmacs_configuration.page_size = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_STACK_SIZE")) {
        _parmacs_configuration.stack_size = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_SHARED_MEMORY")) {
        char *endptr;
        _parmacs_configuration.shared_memory = 
          strtoul(strtok(NULL, " =\t\n\0"), &endptr, 10);
      } else if (!strcmp(var_name, "PARMACS_MEM_ALIGNMENT")) {
        _parmacs_configuration.memory_alignment=atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_MAX_PROCESS")) {
        _parmacs_configuration.max_processes = atoi(strtok(NULL, "\0"));
      } else if (!strcmp(var_name, "PARMACS_KERNEL_LIMIT")) {
        _parmacs_configuration.kernel_limit = atoi(strtok(NULL, " =\t\n\0"));
      } else if (!strcmp(var_name, "PARMACS_TIMESTAMP")) {
        _parmacs_configuration.timestamp = atoi(strtok(NULL, " =\t\n\0"));
      }
    }
    fclose(fd);
  }
}

/*
 * Memory management
 * Comments:
 *   Memory is shared among pthreads. So, simple malloc is enough
 */
 
void *
_parmacs_memory_alloc(int size)
{
  register void *address;
  register unsigned long int u_ptr;
  register int adjust;
  
  /* malloc is reentrant ? */
#ifdef PARMACS_MALLOC_NOT_REENTRANT
  _parmacs_internal_lock(PARMACS_MEMORY_ID);
#endif
  address = (void *)malloc(size+_parmacs_config->memory_alignment);
#ifdef PARMACS_MALLOC_NOT_REENTRANT
  _parmacs_internal_unlock(PARMACS_MEMORY_ID);
#endif
  u_ptr = (unsigned long int)address;
  if ((adjust = u_ptr & (_parmacs_config->memory_alignment-1))) {
    u_ptr += _parmacs_config->memory_alignment - adjust;
    address = (void *)u_ptr;
  }
  return (void *)address;
}

void
_parmacs_memory_free(void *address)
{
  /* not implemented */
}

void *
_parmacs_local_alloc(int size)
{
  register void *address;
  /* malloc is reentrant? it should... */
#ifdef PARMACS_MALLOC_NOT_REENTRANT
  _parmacs_internal_lock(PARMACS_MEMORY_ID);
#endif
  address = (void *)malloc(size);
#ifdef PARMACS_MALLOC_NOT_REENTRANT
  _parmacs_internal_unlock(PARMACS_MEMORY_ID);
#endif
  return address;
}

void
_parmacs_local_free(void *address)
{
  free(address);
}

/*
 * Process creation and joining
 */
 
static void *
_parmacs_process_wrapper(void *pr_data)
{
  register parmacs_process_t process = (parmacs_process_t)pr_data;
  register parmacs_process_t parent;
  
  /* self-identification */
  _posix_setspecific(_parmacs_data->private, pr_data);
  parent = process->parent;
  
  /* execute code */
  process->routine();
  
  /* Notify parent and clean up */
  _parmacs_internal_lock(PARMACS_PROCESS_ID);
  m5_mutex_lock(&(parent->m));
  parent->finished ++;
  parent->ref_count --;
  if (!parent->ref_count) {
    parent->link = _parmacs_data->free_process;
    _parmacs_data->free_process = parent;
  }
  m5_cond_signal(&(parent->c));
  m5_mutex_unlock(&(parent->m));
  m5_mutex_lock(&(process->m));
  process->ref_count --;
  if (!process->ref_count) {
    process->link = _parmacs_data->free_process;
    _parmacs_data->free_process = process;
  }
  m5_mutex_unlock(&(process->m));
  _parmacs_internal_unlock(PARMACS_PROCESS_ID);
  m5_thread_exit();
}
 
void
_parmacs_create(void (*routine)())
{
  register int i;
  parmacs_process_t myself, child;
  m5_mutexattr_t m_attr;
  m5_condattr_t c_attr;
  pthread_attr_t p_attr;
  pthread_t new;

  /* self identify */
  _posix_getspecific(_parmacs_data->private, (void **)&myself);

  /* Allocate an entry for the new process */
  _parmacs_internal_lock(PARMACS_PROCESS_ID);
  if (!_parmacs_data->free_process) {
    _parmacs_internal_unlock(PARMACS_PROCESS_ID);
    _parmacs_panic("parmacs: too many processes!\n");
  }
  child = _parmacs_data->free_process;
  _parmacs_data->free_process = child->link;
  child->ref_count = 1;
  _parmacs_internal_unlock(PARMACS_PROCESS_ID);

  /* Initialize */
  m5_condattr_init(&c_attr);
  m5_mutexattr_init(&m_attr);
  m5_cond_init(&(child->c), &c_attr);
  m5_mutex_init(&(child->m), &m_attr);
  child->children = 0;
  child->finished = 0;
  child->parent   = myself;
  child->routine  = routine;
  child->link     = (parmacs_process_t)0;
  
  /* increase parent's reference count */
  m5_mutex_lock(&(myself->m));
  myself->ref_count ++;
  m5_mutex_unlock(&(myself->m));
  
  /* Actually fork */
  _posix_attr_init(&p_attr);
  if (_posix_create(&new, &p_attr, _parmacs_process_wrapper, (void *)child)!=0){
    _parmacs_panic("parmacs: cannot create new process!");
  }
  _posix_detach(new);
}

void
_parmacs_wait_for_end(int amount)
{
  parmacs_process_t myself;
  
  _posix_getspecific(_parmacs_data->private, (void **)&myself);
  m5_mutex_lock(&(myself->m));
  while(amount) {
    if (myself->finished) {
      amount --;
      myself->finished --;
      myself->children --;
    } else {
      m5_cond_wait(&(myself->c), &(myself->m));
    } 
  }
  m5_mutex_unlock(&(myself->m));
}

parmacs_pid
_parmacs_get_pid()
{
  return ((parmacs_pid)_posix_self());
}

/*
 * Scheduling, clean up and exit routines
 */
 
void 
_parmacs_panic(char *msg)
{
  if (msg) fprintf(stderr, "%s\n", msg);
  exit(-1);
}

void 
_parmacs_exit()
{
  exit(0);
}

void 
_parmacs_yield()
{
  sched_yield();
}

/*
 * Clock
 */
parmacs_timestamp
_parmacs_clock()
{
  struct tms now;
  return (parmacs_timestamp)times(&now);
}

