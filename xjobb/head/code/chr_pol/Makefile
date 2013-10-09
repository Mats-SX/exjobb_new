

DEBUG	= true
ifeq ($(DEBUG), true)
CXXFLAGS += -g
LDFLAGS += -g
else
CXXFLAGS += -O3
endif

# Paths
HOME		= /home/sx/repo/xjobb/head/
THIS		= $(HOME)code/chr_pol/
VPATH		= $(THIS)src/
BIN		= $(HOME)common/bins/

# ld flags
LDLIBS		+= -lpari		# For PARI links
LDLIBS		+= -lntl		# For NTL links
LDLIBS		+= -lgmpxx -lgmp	# For GMP links

# compiler and preprocessor flags
CXX		= g++
CC		= g++
CXXFLAGS 	+= -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS	+= -m64 -mtune=corei7-avx -march=corei7-avx

# Programs
PRGS		= chr_pol_pari chr_pol_ntl
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
chr_pol_pari: chr_pol_pari.o utils.o parimisc.o
chr_pol_ntl: chr_pol_ntl.o utils.o ntlmisc.o

# Removing dependencies on .h-files:

include $(DFILES)

# Dependencies
%.d: %.cc
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
