#line 185 "/home/min/a/ramachav/ECE-66600-Homeworks-Projects/Homework_2/gem5_working_baseline/splash2/codes/null_macros/c.m4.null.POSIX_BARRIER"

#line 1 "grav.H"
#ifndef _GRAV_H_
#define _GRAV_H_

void hackgrav(bodyptr p, long ProcessId);
void gravsub(register nodeptr p, long ProcessId);
void hackwalk(long ProcessId);
void walksub(nodeptr n, real dsq, long ProcessId);
bool subdivp(register nodeptr p, real dsq, long ProcessId);

#endif
