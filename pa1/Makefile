#
# Makefile
#
# Computer Science 124
# Coding Project 1
#


# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -ggdb -std=c99 -Wall -Werror -lm

# name for executable
EXE = prim

# space-separated list of header files
HDRS = d_ary_heap.h list.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS =

# space-separated list of source files
SRCS = d_ary_heap.c prim.c list.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
