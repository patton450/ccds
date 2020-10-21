PROJ=ccds
WFLAGS=-Wall -Wextra 
CFLAGS=-c -std=c11 -fpic

SRCDIR=./src/
OBJDIR=./obj/
TSTDIR=./tst/
BINDIR=./bin/

SRCS=$(wildcard $(SRCDIR)*.c)
HEAD=$(wildcard $(SRCDIR)*.h)
OBJS=$(addprefix $(OBJDIR), $(patsubst %.c, %.o, $(notdir $(SRCS))))
TSTS=$(wildcard $(SRCDIR)*.c)

all: log shared

shared: $(OBJS)
	@if test ! -d "./bin"; then mkdir "bin"; fi
	gcc -shared -o $(BINDIR)lib$(PROJ).so $(OBJS)

log: $(SRCDIR)log.c $(SRCDIR)log.h
	@if test ! -d "./obj"; then mkdir "obj"; fi
	gcc $(CFLAGS) $(WFLAGS) $(SRCDIR)log.c -o $(OBJDIR)log.o -DLOG_USE_COLOR
#the object files depend on the src
$(OBJS): $(SRCS)
	@if test ! -d "./obj"; then mkdir "obj"; fi
	gcc $(CFLAGS) $(WFLAGS) $(SRCDIR)$(notdir $*).c -o $*.o

$(SRCS): $(HEAD)

#cleanout binaries
clean: 
	rm $(OBJDIR)*
