# Project info
PROJ=ccds

MAJOR=0
MINOR=0
PATCH=1

# Complining flags
WFLAGS=-Wall -Wextra
CSTD=-std=gnu17
CFLAGS=-c -fpic $(CSTD)

# Direcotries
SRCDIR=./src
OBJDIR=./obj
TSTDIR=./test
BINDIR=./bin

# Files
SRCS=$(wildcard $(SRCDIR)/*.c)
HEAD=$(wildcard $(SRCDIR)/*.h)
OBJS=$(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(notdir $(SRCS))))
TSTS=$(wildcard $(TSTDIR)/*.c)

#Local Libray path + name
LOCAL_LIB=$(BINDIR)/lib$(PROJ).so.$(MAJOR).$(MINOR).$(PATCH)

# Creates a directory if it does not yet exist
make_dir_if_nonexist = @if test ! -d "$(1)"; then mkdir "$(1)"; fi

all: log shared

#compiles the tests and outputs them in the bin dir
tests: $(TSTS)

#ensure we have the shared library
$(TSTS): shared
	gcc $(CSTD) -L$(BINDIR) -lccds -lcriterion $*.c -o $(BINDIR)/$(notdir $*)

#compiles the ccds library and outputs it into the bin dir
shared: $(OBJS)
	$(call make_dir_if_nonexist,$(BINDIR))
	gcc $(CSTD) -shared -lpthread -Wl,-soname,$(BINDIR)/lib$(PROJ).so -o \
	$(BINDIR)/lib$(PROJ).so $(OBJS)

#compile our logfile (allow for diffrent level of logs in library)
log: $(SRCDIR)/log.c $(SRCDIR)/log.h
	$(call make_dir_if_nonexist,$(OBJDIR))
	gcc $(CFLAGS) $(WFLAGS) $(SRCDIR)/log.c -o $(OBJDIR)/log.o -DLOG_USE_COLOR

#the object files depend on the src
$(OBJS): $(SRCS)
	$(call make_dir_if_nonexist,$(OBJDIR))
	gcc $(CFLAGS) $(WFLAGS) $(SRCDIR)/$(notdir $*).c -lpthread -o $*.o

$(SRCS): $(HEAD)


#cleanout binaries
clean: 
	rm -r $(OBJDIR)
	rm -r $(BINDIR)
