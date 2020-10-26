/*
 * File: pthreads/parmacs_config.h
 * Description: default values for parmacs configuration
 * Comments: The meaning of some of these values are platform specific.
 *   Please read the corresponding documentation.
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
 
#ifndef _PTHREADS_PARMACS_CONFIG_H_
#define _PTHREADS_PARMACS_CONFIG_H_

#ifndef PARMACS_SPIN_COUNT
#define PARMACS_SPIN_COUNT 1
#endif

#ifndef PARMACS_MAX_BARRIER
#define PARMACS_MAX_BARRIER 128
#endif

#ifndef PARMACS_MAX_GLOBAL
#define PARMACS_MAX_GLOBAL 128
#endif

#ifndef PARMACS_MAX_EVENT
#define PARMACS_MAX_EVENT 128
#endif

#ifndef PARMACS_MAX_LOCK
#define PARMACS_MAX_LOCK 4096
#endif

#ifndef PARMACS_PAGE_SIZE
#define PARMACS_PAGE_SIZE 4096
#endif

#ifndef PARMACS_STACK_SIZE
#define PARMACS_STACK_SIZE 8
#endif

#ifndef PARMACS_SHARED_MEMORY
#define PARMACS_SHARED_MEMORY 16777216
#endif

#ifndef PARMACS_MEM_ALIGNMENT
#define PARMACS_MEM_ALIGNMENT 4
#endif

#ifndef PARMACS_MAX_PROCESS
#define PARMACS_MAX_PROCESS 128
#endif

#ifndef PARMACS_KERNEL_LIMIT
#define PARMACS_KERNEL_LIMIT 0
#endif

#ifndef PARMACS_TIMESTAMP
#define PARMACS_TIMESTAMP 0
#endif

#endif /* _PTHREADS_CONFIG_H_ */
