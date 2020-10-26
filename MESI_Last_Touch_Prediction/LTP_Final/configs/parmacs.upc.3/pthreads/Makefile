#
# Parmacs Makefile
# gsomk@ac.upc.es (DAC-UPC)
#

TARGET = libparmacs_pthreads.a

AS = as
CC = cc
AR = ar
RANLIB = ranlib
RM = rm
TOUCH = touch

DEFINES = -D_REENTRANT -DPARMACS_INTERNALS \
           $(DEF01) $(DEF02) $(DEF03) $(DEF04) $(DEF05) $(DEF06) $(DEF07) \
           $(DEF08) $(DEF09) $(DEF10) $(DEF11) $(DEF12) $(DEF13) $(DEF14) \
           $(DEF15) $(DEF16) $(DEF17) $(DEF18) $(DEF19) $(DEF20) $(DEF21)

#DEF01 = -DPARMACS_NO_SETCONCURRENCY
#DEF02 = -DPARMACS_SPIN_LOCK
#DEF03 = -DPARMACS_SPIN_COUNT=1
#DEF04 = -DPARMACS_MAX_BARRIER=128
#DEF05 = -DPARMACS_MAX_GLOBAL=128
#DEF06 = -DPARMACS_MAX_EVENT=128
#DEF07 = -DPARMACS_MAX_LOCK=128
#DEF08 = -DPARMACS_PAGE_SIZE=4096
#DEF09 = -DPARMACS_STACK_SIZE=8
#DEF10 = -DPARMACS_SHARED_MEMORY=16777216
#DEF11 = -DPARMACS_MEM_ALIGNMENT=4
#DEF12 = -DPARMACS_KERNEL_LIMIT=0
#DEF13 = -DPARMACS_MAX_PROCESS=128
#DEF14 = -DPARMACS_TIMESTAMP=0

#OSF1 V4.0 may need the following:
#DEF15 = -Dpthread_key_create=pthread_keycreate

#Some Linuxes may need the following
#DEF15 = -D_MIT_POSIX_THREADS

#OpenBSD may need the following
#DEF15 = -D_POSIX_THREADS

#OSF1 1.3 osc1.3 (Mach 3.0 MK) may need the following
#DEF16 = -DPOSIX_1003_4a
#DEF17 = -Dsched_yield=pthread_yield
#DEF18 = -DPARMACS_MALLOC_NOT_REENTRANT

INC = -I.
CFLAGS = $(OPTIMIZE) $(DEBUG) $(INC) $(DEFINES)
#DEBUG = -g
#Optimization flags
#Optimize cc (HP-UX)
#OPTIMIZE = +O2
#Optimize cc (OSF1)
OPTIMIZE = -O2

OBJS = parmacs_support.o parmacs_sync.o m5.o m5_asm.o


all: $(TARGET)

$(TARGET): $(OBJS)
	$(RM) -f $@
	$(AR) -r $@ $(OBJS)
	$(RANLIB) $@

parmacs_support.o: parmacs_support.c parmacs_internals.h m5.h
	$(CC) -c $(CFLAGS) parmacs_support.c

parmacs_sync.o: parmacs_sync.c parmacs_internals.h m5.h
	$(CC) -c $(CFLAGS) parmacs_sync.c

m5.o: m5.c m5.h
	$(CC) -c $(CFLAGS) m5.c

m5_asm.o: m5_asm.s m5.h
	$(AS) m5_asm.s -o m5_asm.o

parmacs_internals.h: parmacs.h parmacs_config.h parmacs_types.h parmacs_decl.h
	$(TOUCH) parmacs_internals.h

clean:
	rm -f core *.o

mrproper:
	rm -f core *.o $(TARGET)
