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
TSTS=$(wildcard $(SRCDIR)*.c)


MAJOR=0
MINOR=0
PATCH=1

all: log shared

shared: $(OBJS)
	@if test ! -d "./bin"; then mkdir "bin"; fi
	gcc $(CSTD) -shared -lpthread -Wl,-soname,$(BINDIR)lib$(PROJ).so -o \
		$(BINDIR)lib$(PROJ).so $(OBJS)

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
