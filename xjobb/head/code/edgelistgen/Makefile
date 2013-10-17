

DEBUG	= true		# default is debug on
ifeq ($(DEBUG), true)
CPPFLAGS += -g
LDFLAGS += -g
else
CPPFLAGS += -O3
endif

# Paths
HOME		= /home/sx/repo/xjobb/head/
THIS		= $(HOME)code/edgelistgen/
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
PRGS		= EdgeListGen
OBJS		= *.o

# default targets
.PHONY: all install clean

all: $(PRGS)

install:
	mv $(PRGS) $(BIN) 

# Linking
EdgeListGen:	$(VPATH)$(PRGS).java
	javac -d $(BIN) $(VPATH)$(PRGS).java

