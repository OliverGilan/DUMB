all:

client: DUMBclient.c socketHelper.o
	gcc -std=gnu99 -o DUMBclient DUMBclient.c socketHelper.o

serve: DUMBserver.c


socketHelper.o: socketHelper.c
	gcc -std=gnu99 -c socketHelper.c

clean: 
	rm -rf DUMBclient DUMBserver *.o