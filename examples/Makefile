CCOMPILER = gcc -ansi -Wall -ggdb 
INCLUDE = -I../include -I../src
LIBS = ../src/*.o

all: serve_buffer

serve_buffer: serve_buffer.c
	$(CCOMPILER) -o serve_buffer serve_buffer.c $(INCLUDE) $(LIBS)

clean:
	rm -f *.o
	rm serve_buffer

distclean: clean
	rm -f *~

