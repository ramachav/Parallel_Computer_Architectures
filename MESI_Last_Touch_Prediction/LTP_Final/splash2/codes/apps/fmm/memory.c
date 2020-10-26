#line 185 "/home/min/a/ramachav/ECE-66600-Homeworks-Projects/Homework_2/gem5_working_baseline/splash2/codes/null_macros/c.m4.null.POSIX_BARRIER"

#line 1 "memory.C"
/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

#include <float.h>
#include "defs.h"
#include "memory.h"


#line 21
#include <pthread.h>
#line 21
#include <sys/time.h>
#line 21
#include <unistd.h>
#line 21
#include <stdlib.h>
#line 21
#include <malloc.h>
#line 21
#define MAX_THREADS 32
#line 21
pthread_t PThreadTable[MAX_THREADS];
#line 21


g_mem *G_Memory;
local_memory Local[MAX_PROCS];

/*
 *  InitGlobalMemory ()
 *
 *  Args : none.
 *
 *  Returns : nothing.
 *
 *  Side Effects : Allocates all the global storage for G_Memory.
 *
 */
void
InitGlobalMemory ()
{
   G_Memory = (g_mem *) valloc(sizeof(g_mem));;
   G_Memory->i_array = (long *) valloc(Number_Of_Processors * sizeof(long));;
   G_Memory->d_array = (double *) valloc(Number_Of_Processors * sizeof(double));;
   if (G_Memory == NULL) {
      printf("Ran out of global memory in InitGlobalMemory\n");
      exit(-1);
   }
   G_Memory->count = 0;
   G_Memory->id = 0;
   {pthread_mutex_init(&(G_Memory->io_lock), NULL);};
   {pthread_mutex_init(&(G_Memory->mal_lock), NULL);};
   {pthread_mutex_init(&(G_Memory->single_lock), NULL);};
   {pthread_mutex_init(&(G_Memory->count_lock), NULL);};
   {
#line 52
	unsigned long	i, Error;
#line 52

#line 52
	for (i = 0; i < MAX_LOCKS; i++) {
#line 52
		Error = pthread_mutex_init(&G_Memory->lock_array[i], NULL);
#line 52
		if (Error != 0) {
#line 52
			printf("Error while initializing array of locks.\n");
#line 52
			exit(-1);
#line 52
		}
#line 52
	}
#line 52
};
   {
#line 53
	pthread_barrier_init(&(G_Memory->synch), NULL, Number_Of_Processors);
#line 53
};
   G_Memory->max_x = -MAX_REAL;
   G_Memory->min_x = MAX_REAL;
   G_Memory->max_y = -MAX_REAL;
   G_Memory->min_y = MAX_REAL;
}


