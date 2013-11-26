
# Paths
HOME		= /home/sx/repo/xjobb/head/
THIS		= $(HOME)code/unique/
VPATH		= $(THIS)src/
BIN		= $(HOME)common/bins/

# Programs
PRGS		= Unique
SRCS		= Unique.java UniqueColouring.java VertexSet.java

# default targets
.PHONY: all install clean

all: $(PRGS)

# Linking
#Unique:	$(addprefix $(VPATH), $(SRCS))
#	javac -d $(BIN) $(addprefix $(VPATH), $(SRCS))

Unique:	$(VPATH)Unique.java
	javac -d $(BIN) $(VPATH)Unique.java
