CCOMPILER=gcc -ansi -Wall -D_REENTRANT

SYSLIBS= -lpthread -lcurl

LIBS = $(SYSLIBS) 

all: 