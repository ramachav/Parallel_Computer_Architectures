TARGET1 = PARMACS_TEST.pthreads
TARGET2 = PARMACS_TIME.pthreads
FILES1 = parmacs_test.C
FILES2 = parmacs_time.C
CS1 = parmacs_test.c 
CS2 = parmacs_time.c 
OBJS1 = parmacs_test.o
OBJS2 = parmacs_time.o
LIBS = $(PARMACS_DIR)/libparmacs_pthreads.a

PARMACS_DIR = ..
DEFINES = $(DEF01) $(DEF02) $(DEF03)
#DEF01 = -DPARMACS_SHARED_MEMORY=1048576
INC = -I$(PARMACS_DIR)
LIB = -L$(PARMACS_DIR)
MACROS = $(PARMACS_DIR)/c.m4.pthreads
CFLAGS = -O2 -D_REENTRANT $(DEFINES) $(INC)
LDFLAGS = $(LIB) -lparmacs_pthreads $(LIB01) $(LIB02)

# The following is needed for SunOS 5.7
#LIB01 = -lpthread
LIB02 = -lrt

# The following is needed for OSF1 V4.0 564 alpha
LIB01 = -lpthread
#LIB01 = -lpthread -lpthreads

# The following is needed for Irix 64
#LIB01 = -lpthread

# The following may be needed for linux
#DEF03 = -D_MIT_POSIX_THREADS
#LIB01 = -lpthread

# The following may be needed for OpenBSD
#DEF03 = -D_POSIX_THREADS
#LIB01 = -lc_r

# The following is needed for OSF1 1.3 osc1.3 (Mach 3.0 MK)
#LIB01 = -lpthreads -lc_r

#Other:
#LIB01 = -lpthread

M4 = m4 -s -Uindex
CC = gcc

x = *

test: $(TARGET1)
time: $(TARGET2)

$(TARGET1): $(OBJS1) $(LIBS)
	$(CC) $(OBJS1) $(CFLAGS) -o $(TARGET1) $(LDFLAGS)

$(TARGET2): $(OBJS2) $(LIBS)
	$(CC) $(OBJS2) $(CFLAGS) -o $(TARGET2) $(LDFLAGS)

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

.H.h:
	$(M4) ${MACROS} $*.H > $*.h

.C.c:
	$(M4) $(MACROS) $*.C > $*.c

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.C.o:
	$(M4) $(MACROS) $*.C > $*.c
	$(CC) -c $(CFLAGS) $*.c

parmacs_test.o: parmacs_test.C
parmacs_time.o: parmacs_time.C

clean:
	rm -f core $(CS1) $(CS2) $(OBJS1) $(OBJS2)

mrproper: clean
	rm -f $(TARGET1) $(TARGET2)

