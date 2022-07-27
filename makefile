# Project info
PROJ=ccds

MAJOR=0
MINOR=0
PATCH=1

# Library names
LIB_MAJOR_MINOR_PATCH=lib$(PROJ).so.$(MAJOR).$(MINOR).$(PATCH)
LIB_MAJOR_MINOR=lib$(PROJ).so.$(MAJOR).$(MINOR)
LIB_MAJOR=lib$(PROJ).so.$(MAJOR)
LIB_NAME=lib$(PROJ).so


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

LOCAL_LIB=$(BINDIR)/$(LIB_NAME)

# Creates a directory if it does not yet exist
define make_dir_if_nonexist 
@if test ! -d "$(1)"; then mkdir "$(1)"; fi
endef

# Changes the directory from ./A/B to ./C/B
define re_dir
$(2)/$(notdir $(1))
endef

# 
define cond_compile_param
	@if [ "$(notdir $(1))" = "log" ]; then \
		echo "gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o -DLOG_USE_COLOR" \
		&& gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o -DLOG_USE_COLOR; \
	else \
		echo "gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o" \
		&& gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o; \
	fi
endef
all: $(LOCAL_LIB)

#compiles the tests and outputs them in the bin dir
tests: $(TSTS)

#ensure we have the shared library
$(TSTS): $(LOCAL_LIB)
	gcc $(CSTD) -L$(BINDIR) -l$(PROJ) -lcriterion $*.c -o $(call re_dir,$*,$(BINDIR))

#compiles the ccds library and outputs it into the bin dir
$(LOCAL_LIB): $(OBJS)
	$(call make_dir_if_nonexist,$(BINDIR))
	gcc $(CSTD) -shared -Wl,-soname,$(LOCAL_LIB) -o $(LOCAL_LIB) $(OBJS)

#the object files depend on the src
$(OBJS): $(SRCS)
	$(call make_dir_if_nonexist,$(OBJDIR))
	$(call cond_compile_param,$*)

$(SRCS): $(HEAD)

# cleanout binaries
clean: 
	rm -r $(OBJDIR)
	rm -r $(BINDIR)
