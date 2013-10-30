
# Paths
HOME		= /home/sx/repo/xjobb/head/
THIS		= $(HOME)code/javatests/
VPATH		= $(THIS)src/
BIN		= $(HOME)common/bins/

# Programs
PRGS		= Tester.class

# default targets
.PHONY: all install clean

all: $(PRGS)

# Linking
Tester.class:	$(VPATH)Tester.java
	javac -d $(BIN) $(VPATH)Tester.java

