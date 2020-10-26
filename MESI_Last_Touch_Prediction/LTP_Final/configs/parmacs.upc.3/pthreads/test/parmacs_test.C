/*
 * parmacs_test.C
 * Description: example/test code for PARMACS macros.
 * Barcelona, 1996
 * Copyright (c) 1996 by GSOMK DAC-UPC. All Rights Reserved.
 * GSOMK team at Department of Computer Architecture, UPC.
 * gsomk@ac.upc.es
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

/*
 * Usage:
 *    TESTING [nprocs] [loops] [waste]
 *       nprocs: number of processes
 *       loops : iterations in the self-scheduled loop
 *       waste : waste time in loop body
 */

#include <stdlib.h>
#include <stdio.h>

MAIN_ENV

BARDEC(barrier)
LOCKDEC(lock)
LOCKDEC(memory)
int *shared;
int procs = 0;
int loops = 0;
int waste = 0;
PAUSEDEC(event)
GSDEC(gs)

#define DEFAULT_PROCS 1
#define DEFAULT_LOOP 24
#define DEFAULT_WASTE 100000

main(argc, argv)
int argc;
char *argv[];
{
    int i;
    extern void routine();
    long timing1, timing2;

    MAIN_INITENV()
    
    printf("main: parmacs_initialized\n");
    
    if (argc > 1) procs = atoi(argv[1]);
    if (!procs) procs = DEFAULT_PROCS;
    if (argc > 2) loops = atoi(argv[2]);
    if (!loops) loops = (procs-1) * 8;
    if (argc > 3) waste = atoi(argv[3]);
    if (!waste) waste = DEFAULT_WASTE;

    CLOCK(timing1)

    /* Initialize */
    BARINIT(barrier)
    LOCKINIT(lock)
    LOCKINIT(memory)
    PAUSEINIT(event)
    GSINIT(gs)

    printf("main: synchronization variables initialized\n");
    
    /* allocate shared memory: 2 entries per child + 1 additional entry */
    shared = (int*)G_MALLOC((procs*2-1) * sizeof(int));
    printf("main: shared memory allocated at %p\n", shared);
    shared[0] = 0;

    /* create children */
    for (i = 0; i < (procs-1); i++) {
        CREATE(routine)
    }

    /* Let children start */
    printf("main: all created; setting event\n");
    SETPAUSE(event)

    /* Synchronize with children (first step) */
    BARRIER(barrier, procs)

    /* Wait for children to finish (second step) */
    WAIT_FOR_END(procs - 1)
    printf("main: all children finished\n");
    printf("main: shared memory entries used by children: %d\n", shared[0]);
    printf("main: shared memory contains:\n      ");
    for (i = 1; i < (procs*2-1); i++) printf("%d ", shared[i]);
    printf("\n");

    /* timestamps */
    CLOCK(timing2)
    printf("main: timestamps: %lu %lu\n", timing1, timing2);

    MAIN_END()
}

int waste_time(w)
int w;
{
    volatile int dummy;
    int i;
    for (i=0; i < w; i++) {
        dummy++;
    }
}

void routine()
{
    int local;  /* assigned entry in shared memory array */
    int *array; /* local array for recording assigned subscripts */
    int idx;
    int myself;

    /* initialize */
    LOCK(memory)
    array = (int *)malloc(loops * sizeof(int));
    UNLOCK(memory)

    /* Wait for main to invoke SETPAUSE */
    PAUSE(event)
    /* PAUSE resets the event; so, only one process executes this at a time */

    myself = local = ++(shared[0]);
    printf("child: started (%d)\n", myself);

    /* Let another process start */
    SETPAUSE(event)

    /* first step: write "1" in assigned entry in the shared array */
    shared[local] = 1;

    /* Synchronize with other processes: first step finished */
    BARRIER(barrier, procs)

    /* Get another entry in the shared array */
    LOCK(lock)
    local = ++(shared[0]);
    UNLOCK(lock)

    /* second step: write "2" in assigned entry in the shared array */
    shared[local] = 2;

    /* execute self-scheduled loop */
    idx = 0;
    do {
        GETSUB(gs, local, loops, procs-1);
        if (local != -1) {
            array[idx++] = local;
            waste_time(waste);
        }
    } while (local != -1);

    /* print out assigned subscripts */
    LOCK(lock)
    printf("child: (%d) got ", myself);
    for (local = 0; local < idx; local++) printf("%d ", array[local]);
    printf("\n");
    UNLOCK(lock)

    /* child finishes... */
}
 
