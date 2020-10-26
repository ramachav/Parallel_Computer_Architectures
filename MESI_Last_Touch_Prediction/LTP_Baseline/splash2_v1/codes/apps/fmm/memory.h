
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

#ifndef _Memory_H
#define _Memory_H 1

#include "defs.h"
#include "particle.h"
#include "box.h"

#define MAX_LOCKS 2048



#ifndef _PARMACS_COMMON_INCLUDES_
#define _PARMACS_COMMON_INCLUDES_

#include <pthread.h>
#include <parmacs_config.h>
#include <parmacs_types.h>
#include <parmacs_decl.h>
#include <parmacs.h>

#endif /* _PARMACS_COMMON_INCLUDES_ */



typedef struct _G_Mem g_mem;

struct _G_Mem
{
   parmacs_lock_t (io_lock);
   parmacs_lock_t (mal_lock);
   parmacs_lock_t (single_lock);
   parmacs_lock_t (count_lock);
   int count;
   parmacs_lock_t (lock_array);
   parmacs_barrier_t (synch);
   volatile int *i_array;
   volatile double *d_array;
   real f_array[MAX_PROCS][NUM_DIM_POW_2];
   real max_x;
   real min_x;
   real max_y;
   real min_y;
   int id;
};
extern g_mem *G_Memory;

typedef struct _Local_Memory local_memory;
struct _Local_Memory {
   int pad_begin[PAD_SIZE];

   box *B_Heap;
   int Index_B_Heap;
   int Max_B_Heap;

   particle **Particles;
   int Num_Particles;
   int Max_Particles;

   box *Childless_Partition;
   box *Parent_Partition[MAX_LEVEL];
   int Max_Parent_Level;

   box *Local_Grid;
   real Local_X_Max;
   real Local_X_Min;
   real Local_Y_Max;
   real Local_Y_Min;

   int Total_Work;
   int Min_Work;
   int Max_Work;

   int Time_Step;
   double Time;
   unsigned int init_done_times;
   time_info Timing[MAX_TIME_STEPS];
   
   int pad_end[PAD_SIZE];
};
extern local_memory Local[MAX_PROCS];

extern void InitGlobalMemory();

#endif /* _Memory_H */
