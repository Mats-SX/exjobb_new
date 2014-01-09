

DEBUG	= false
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
LDLIBS		+= -lflint -lmpfr -lmpir
LDLIBS		+= -lgmpxx -lgmp	# For GMP links
LDLIBS		+= -lpthread		# For c++11 threads

# compiler and preprocessor flags
CXX		= g++
CC		= g++
CXXFLAGS 	+= -Wall -Wextra -pedantic-errors -std=c++11 -pthread
CXXFLAGS	+= -m64 -mtune=corei7-avx -march=corei7-avx

# Programs
PRGS		= chr_pol_pari chr_pol_flint chr_pol_ntl
OBJS		= *.o
DFILES		= $(addprefix $(VPATH), $(wildcard *.d))

# default targets
.PHONY: all install clean

all: $(PRGS)

install: all
	mv $(PRGS) $(BIN) 

clean:
	$(RM) $(OBJS)

cleanest: clean
	$(RM) $(DFILES) 

# Compilation
chr_pol_pari.o: chr_pol_pari.cc utils.cc parimisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DPARI -c -o $@ $<
pariutils.o: utils.cc parimisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DPARI -c -o $@ $<
parimisc.o: parimisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DPARI -c -o $@ $<

chr_pol_ntl.o: chr_pol_ntl.cc utils.cc ntlmisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DNTLLIB -c -o $@ $<
ntlutils.o: utils.cc ntlmisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DNTLLIB -c -o $@ $<
ntlmisc.o: ntlmisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DNTLLIB -c -o $@ $<

chr_pol_flint.o: chr_pol_flint.cc utils.cc flintmisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DFLINT -c -o $@ $<
flintutils.o: utils.cc flintmisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DFLINT -c -o $@ $<
flintmisc.o: flintmisc.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DFLINT -c -o $@ $<

# Linking
chr_pol_pari: chr_pol_pari.o pariutils.o parimisc.o
chr_pol_ntl: chr_pol_ntl.o ntlutils.o ntlmisc.o
chr_pol_flint: chr_pol_flint.o flintutils.o flintmisc.o

# Removing dependencies on .h-files:

include $(DFILES)

# Dependencies
%.d: %.cc
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
