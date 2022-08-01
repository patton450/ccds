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
HEADDIR=./src/include
OBJDIR=./obj
TSTDIR=./test
LIBDIR=./lib
BINDIR=./bin

# Files
SRCS=$(wildcard $(SRCDIR)/*.c)
HEAD=$(wildcard $(HEADDIR)/*.h)
OBJS=$(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(notdir $(SRCS))))
TSTS=$(wildcard $(TSTDIR)/*.c)

# Library names
LIB_MAJOR_MINOR_PATCH=lib$(PROJ).so.$(MAJOR).$(MINOR).$(PATCH)
LIB_MAJOR_MINOR=lib$(PROJ).so.$(MAJOR).$(MINOR)
LIB_MAJOR=lib$(PROJ).so.$(MAJOR)
LIB_NAME=lib$(PROJ).so
LIB_SNAME=lib$(PROJ).a

LOCAL_LIB_MMP=$(LIBDIR)/$(LIB_MAJOR_MINOR_PATCH)
LOCAL_LIB_MM=$(LIBDIR)/$(LIB_MAJOR_MINOR)
LOCAL_LIB_M=$(LIBDIR)/$(LIB_MAJOR)
LOCAL_LIB=$(LIBDIR)/$(LIB_NAME)
LOCAL_SLIB=$(LIBDIR)/$(LIB_SNAME)

# Creates a directory if it does not yet exist
define make_dir_if_nonexist 
@if test ! -d "$(1)"; then mkdir "$(1)"; fi
endef

# Changes the directory from ./A/B to ./C/B
define re_dir
$(2)/$(notdir $(1))
endef

# compile objects with optional params 
define cond_compile_param
	@if [ "$(notdir $(1))" = "log" ]; then \
		echo "gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o -DLOG_USE_COLOR" \
		&& gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o -DLOG_USE_COLOR; \
	else \
		echo "gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o" \
		&& gcc $(CFLAGS) $(WFLAGS) $(call re_dir,$(1),$(SRCDIR)).c -lpthread -o $(1).o; \
	fi
endef

all: $(LOCAL_LIB) $(LOCAL_SLIB)

#compiles the tests and outputs them in the bin dir
tests: $(TSTS)

#ensure we have the shared library
$(TSTS): $(LOCAL_LIB)
	$(call make_dir_if_nonexist,$(BINDIR))
	gcc $(CSTD) $*.c -o $(call re_dir,$*,$(BINDIR)) -L$(LIBDIR) -l$(PROJ) -lcriterion 

#compiles the ccds library and outputs it into the bin dir
$(LOCAL_LIB): $(OBJS)
	$(call make_dir_if_nonexist,$(LIBDIR))
	gcc $(CSTD) -shared $(OBJS) -Wl,-soname,$(LOCAL_LIB_MM) -o $(LOCAL_LIB_MMP)
	ln -s $(LOCAL_LIB_MMP) $(LOCAL_LIB_M)
	ln -s $(LOCAL_LIB_MMP) $(LOCAL_LIB)

# Compiles static CCDS library
$(LOCAL_SLIB): $(OBJS)	
	$(call make_dir_if_nonexist,$(LIBDIR))
	ar rcs $(LOCAL_SLIB) $(OBJS) 

#the object files depend on the src
$(OBJS): $(SRCS)
	$(call make_dir_if_nonexist,$(OBJDIR))
	$(call cond_compile_param,$*)

$(SRCS): $(HEAD)

# cleanout binaries
clean: 
	rm -r $(OBJDIR)
	rm -r $(LIBDIR)
	rm -r $(BINDIR)
