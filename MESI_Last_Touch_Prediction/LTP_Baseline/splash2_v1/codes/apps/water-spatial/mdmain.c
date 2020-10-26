
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



#ifndef _PARMACS_COMMON_INCLUDES_
#define _PARMACS_COMMON_INCLUDES_

#include <pthread.h>
#include <parmacs_config.h>
#include <parmacs_types.h>
#include <parmacs_decl.h>
#include <parmacs.h>

#endif /* _PARMACS_COMMON_INCLUDES_ */



#include <stdio.h>

#include "parameters.h"
#include "mdvar.h"
#include "water.h"
#include "wwpot.h"
#include "cnst.h"
#include "mddata.h"
#include "fileio.h"
#include "split.h"
#include "global.h"

/************************************************************************/

double  MDMAIN(NFSV,NFRST,NSTEP,NRST,NPRINT,NSAVE,LKT,NORD1,ProcID)
  int NFSV,NFRST,NSTEP,NRST,NPRINT,NSAVE,LKT,NORD1;
  unsigned ProcID;
  
{
    
    double TVIR = 0.0;
    double TTMV = 0.0;
    double TKIN = 0.0;
    double XTT;
    int i,j,k;
    double POTA,POTR,POTRF;
    double XVIR,AVGT,TEN;
    struct link *curr_ptr;
    struct list_of_boxes *new_box, *curr_box;
    
    for (i=start_end[ProcID]->box[XDIR][FIRST]; i<=start_end[ProcID]->box[XDIR][LAST]; i++) {
        for (j=start_end[ProcID]->box[YDIR][FIRST]; j<=start_end[ProcID]->box[YDIR][LAST]; j++) {
            for (k=start_end[ProcID]->box[ZDIR][FIRST]; k<=start_end[ProcID]->box[ZDIR][LAST]; k++) {
                new_box = (box_list *) _parmacs_memory_alloc((int)(sizeof(box_list)));;
                new_box->coord[XDIR] = i;
                new_box->coord[YDIR] = j;
                new_box->coord[ZDIR] = k;
                new_box->next_box = NULL;
                curr_box = my_boxes[ProcID];
                if (curr_box == NULL)
                    my_boxes[ProcID] = new_box;
                else {
                    while (curr_box->next_box != NULL)
                        curr_box = curr_box->next_box;
                    curr_box->next_box = new_box;
                } /* else */
            }
        }
    }
    
    /* calculate initial value for acceleration */
    
    INTRAF(&gl->VIR,ProcID);
    
    {
    _parmacs_barrier((gl->start), (NumProcs));
};
    
    INTERF(ACC,&gl->VIR,ProcID);
    
    {
    _parmacs_barrier((gl->start), (NumProcs));
};
    
    /* MOLECULAR DYNAMICS LOOP */
    
    for (i=1;i <= NSTEP; i++) {
        TTMV=TTMV+1.00;
        
        /* POSSIBLE ENHANCEMENT:  Here's where one start measurements to avoid 
           cold-start effects.  Recommended to do this at the beginning of the
           second timestep; i.e. if (i == 2).
           */
        
        /* initialize various shared sums */
        if (ProcID == 0) {
            int dir;
            if (i >= 2) {
                {(gl->trackstart) = _parmacs_clock();};
            }                
            gl->VIR = 0.0;
            gl->POTA = 0.0;
            gl->POTR = 0.0;
            gl->POTRF = 0.0;
            for (dir = XDIR; dir <= ZDIR; dir++)
                gl->SUM[dir] = 0.0;
        }
        
        if ((ProcID == 0) && (i >= 2)) {
            {(gl->intrastart) = _parmacs_clock();};
        }
        
        {
    _parmacs_barrier((gl->start), (NumProcs));
};

        PREDIC(TLC,NORD1,ProcID);
        INTRAF(&gl->VIR,ProcID);
        
        {
    _parmacs_barrier((gl->start), (NumProcs));
};
        
        if ((ProcID == 0) && (i >= 2)) {
            {(gl->intraend) = _parmacs_clock();};
            gl->intratime += gl->intraend - gl->intrastart;
        }
        
        if ((ProcID == 0) && (i >= 2)) {
            {(gl->interstart) = _parmacs_clock();};
        }
        
        INTERF(FORCES,&gl->VIR,ProcID); 
        
        if ((ProcID == 0) && (i >= 2)) {
            {(gl->interend) = _parmacs_clock();};
            gl->intertime += gl->interend - gl->interstart;
        }
        
        CORREC(PCC,NORD1,ProcID);
        
        BNDRY(ProcID);
        
        KINETI(NMOL,gl->SUM,HMAS,OMAS,ProcID);
        
        {
    _parmacs_barrier((gl->start), (NumProcs));
};
        
        if ((ProcID == 0) && (i >= 2)) {
            {(gl->intraend) = _parmacs_clock();};
            gl->intratime += gl->intraend - gl->interend;
        }
        
        TKIN=TKIN+gl->SUM[0]+gl->SUM[1]+gl->SUM[2];
        TVIR=TVIR-gl->VIR;
        
        /* CHECK if  PRINTING AND/OR SAVING IS TO BE DONE */
        
        if ( ((i % NPRINT) == 0) || ((NSAVE > 0) && ((i % NSAVE) == 0))) {
            
            /* if so, call poteng to compute potential energy.  Note
               that we are attributing all the time in poteng to intermolecular
               computation although some of it is intramolecular (see poteng.C) */
            
            if ((ProcID == 0) && (i >= 2)) {
                {(gl->interstart) = _parmacs_clock();};
            }
            
            POTENG(&gl->POTA,&gl->POTR,&gl->POTRF,ProcID);
            
            {
    _parmacs_barrier((gl->start), (NumProcs));
};
            
            if ((ProcID == 0) && (i >= 2)) {
                {(gl->interend) = _parmacs_clock();};
                gl->intertime += gl->interend - gl->interstart;
            }
            
            POTA=gl->POTA*FPOT;
            POTR=gl->POTR*FPOT;
            POTRF=gl->POTRF*FPOT;
            XVIR=TVIR*FPOT*0.50/TTMV;
            AVGT=TKIN*FKIN*TEMP*2.00/(3.00*TTMV);
            TEN=(gl->SUM[0]+gl->SUM[1]+gl->SUM[2])*FKIN;
            XTT=POTA+POTR+POTRF+TEN;
            
            /* if it is time to print output as well ... */      
            if ((i % NPRINT) == 0 && ProcID == 0) {
                {
    _parmacs_lock(gl->IOLock);
};
                fprintf(six,"     %5d %14.5lf %12.5lf %12.5lf %12.5lf \n"
                        ,i,TEN,POTA,POTR,POTRF);
                fprintf(six," %16.3lf %16.5lf %16.5lf\n",XTT,AVGT,XVIR);
                fflush(six);
                {
    _parmacs_unlock(gl->IOLock);
};
            }
            
        }
        
        {
    _parmacs_barrier((gl->start), (NumProcs));
};
        
        if ((ProcID == 0) && (i >= 2)) {
            {(gl->trackend) = _parmacs_clock();};
            gl->tracktime += gl->trackend - gl->trackstart;
        }
        
    } /* for i */
    
    return(XTT);
    
} /* mdmain.c */

