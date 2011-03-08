CFLAGS=-g

test : nftltest.o nftl.o flashemul.o
        gcc $(CFLAGS) -o test nftltest.o nftl.o flashemul.o

nftltest.o : nftltest.c fm_def.h nftl.h
        gcc $(CFLAGS) -c nftltest.c

nftl.o : nftl.c fm_def.h
        gcc $(CFLAGS) -c nftl.c

flashemul.o : flashemul.c fm_def.h
        gcc $(CFLAGS) -c flashemul.c

clean :
        rm *.o
        rm test
