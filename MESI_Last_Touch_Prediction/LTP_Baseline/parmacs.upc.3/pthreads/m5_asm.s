#include <asm.h>

#define SYS_m5_mutex_lock	-1
#define SYS_m5_mutex_trylock	-2
#define SYS_m5_mutex_unlock	-3
#define SYS_m5_cond_signal	-4
#define SYS_m5_cond_broadcast	-5
#define SYS_m5_cond_wait_impl	-6
#define SYS_m5_thread_exit	-7

#define M5_SYSCALL(x)			\
        .align  4;			\
        .globl  x;			\
        .ent    x,0;			\
        .frame  $sp,0,$ra;		\
x:	ldiq    $v0,SYS_/**/x;		\
        .set    noat;			\
        call_pal PAL_callsys;		\
        .set    at;			\
	ret;				\
	.end	x

M5_SYSCALL(m5_mutex_lock)
M5_SYSCALL(m5_mutex_trylock)
M5_SYSCALL(m5_mutex_unlock)
M5_SYSCALL(m5_cond_signal)
M5_SYSCALL(m5_cond_broadcast)
M5_SYSCALL(m5_cond_wait_impl)
M5_SYSCALL(m5_thread_exit)

