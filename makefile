PROJ=ccds
WFLAGS=-Wall -Wextra 
CSTD=-std=gnu17
CFLAGS=-c -fpic $(CSTD)

SRCDIR=./src/
OBJDIR=./obj/
TSTDIR=./tst/
BINDIR=./bin/

SRCS=$(wildcard $(SRCDIR)*.c)
HEAD=$(wildcard $(SRCDIR)*.h)
OBJS=$(addprefix $(OBJDIR), $(patsubst %.c, %.o, $(notdir $(SRCS))))
TSTS=$(wildcard $(TSTDIR)*.c)


MAJOR=0
MINOR=0
PATCH=1

all: log shared

#compiles the tests and outputs them in the bin dir
tests: $(TSTS)

#ensure we have the shared library
$(TSTS): shared
	gcc $(CSTD) -L./bin -lccds -lcriterion $*.c -o $(BINDIR)$(notdir $*)

#compiles the ccds library and outputs it into the bin dir
shared: $(OBJS)
	@if test ! -d "./bin"; then mkdir "bin"; fi
	gcc $(CSTD) -shared -lpthread -Wl,-soname,$(BINDIR)lib$(PROJ).so -o \
		$(BINDIR)lib$(PROJ).so $(OBJS)

#compile our logfile (allow for diffrent level of logs in library)
log: $(SRCDIR)log.c $(SRCDIR)log.h
	@if test ! -d "./obj"; then mkdir "obj"; fi
	gcc $(CFLAGS) $(WFLAGS) $(SRCDIR)log.c -o $(OBJDIR)log.o -DLOG_USE_COLOR

#the object files depend on the src
$(OBJS): $(SRCS)
	@if test ! -d "./obj"; then mkdir "obj"; fi
	gcc $(CFLAGS) $(WFLAGS) $(SRCDIR)$(notdir $*).c -lpthread -o $*.o

$(SRCS): $(HEAD)


#cleanout binaries
clean: 
	rm $(OBJDIR)*
	rm $(BINDIR)*
