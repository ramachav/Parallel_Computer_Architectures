/*
 * parmacs_time.C
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
 *    PARMACS_TIME [seconds]
 *       seconds: seconds to wait
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

MAIN_ENV

int main(argc, argv)
int argc;
char *argv[];
{
    int i;
    long t1, t2;
    int nseconds = 10;

    MAIN_INITENV()

    if (argc > 1) nseconds = atoi(argv[1]);
    if (nseconds < 1) nseconds = 10;

    printf("Computing PARMACS Time Units. Waiting %d seconds...\r\n",nseconds);
    CLOCK(t1)
    sleep(nseconds);
    CLOCK(t2)
    printf(" - time units: %d sec -> %lu - %lu = %lu\r\n",nseconds,t2,t1,t2-t1);
    MAIN_END()
}

