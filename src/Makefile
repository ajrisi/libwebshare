CCOMPILER=gcc -ansi -Wall -ggdb -D_REENTRANT

SYSLIBS= -lpthread -lcurl

LIBS = $(SYSLIBS) 

INCLUDE=-I../include -I./

all: webshare.o client_thread.o http.o hashtable.o util.o pagetable.o

webshare.o: webshare.c
	$(CCOMPILER) -o webshare.o -c webshare.c $(INCLUDE)

client_thread.o: client_thread.c
	$(CCOMPILER) -o client_thread.o -c client_thread.c $(INCLUDE)

http.o: http.c
	$(CCOMPILER) -o http.o -c http.c $(INCLUDE)

hashtable.o: hashtable.c
	$(CCOMPILER) -o hashtable.o -c hashtable.c $(INCLUDE)

util.o: util.c
	$(CCOMPILER) -o util.o -c util.c $(INCLUDE)

pagetable.o: pagetable.c
	$(CCOMPILER) -o pagetable.o -c pagetable.c $(INCLUDE)

clean:
	rm -f *.o

distclean: clean
	rm -f *~
