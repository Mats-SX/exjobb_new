

DEBUG	= true		# default is debug on
ifeq ($(DEBUG), true)
CPPFLAGS += -g
LDFLAGS += -g
else
CPPFLAGS += -O3
endif

# Paths
HOME		= /home/sx/repo/xjobb/head/
THIS		= $(HOME)code/autotests/
VPATH		= $(THIS)src/
BIN		= $(HOME)common/bins/

# ld flags
LDLIBS		+= #ADD libs here

# compiler and preprocessor flags
CXX		= g++
CC		= g++
CXXFLAGS 	+= -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS	+= -m64 -mtune=corei7-avx -march=corei7-avx

# Programs
PRGS		= autotests
OBJS		= *.o
DFILES		= $(addprefix $(VPATH), $(wildcard *.d))

# default targets
.PHONY: all install clean

all: $(PRGS)

install:
	mv $(PRGS) $(BIN) 

clean:
	$(RM) $(OBJS)

cleanest: clean
	$(RM) $(DFILES) 

# Linking
autotests: autotests.o

# Removing dependencies on .h-files:

include $(DFILES)

# Dependencies
%.d: %.cc
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
