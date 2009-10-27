all: 
	make -C ./src
	make -C ./examples

clean:
	make -C ./src clean
	make -C ./examples clean

distclean: 
	make -C ./src distclean
	make -C ./examples distclean


