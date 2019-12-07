all:

client: DUMBclient.c socketHelper.o
	gcc -std=gnu11 -O3 -o DUMBclient DUMBclient.c socketHelper.o

serve: DUMBserver.c socketHelper.o
	gcc -std=gnu11 -pthread -o DUMBserve DUMBserver.c socketHelper.o

socketHelper.o: socketHelper.c
	gcc -std=gnu11 -c socketHelper.c

clean: 
	rm -rf DUMBclient DUMBserve *.o