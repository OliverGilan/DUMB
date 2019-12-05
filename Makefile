all:

client: DUMBclient.c socketHelper.o
	gcc -std=c99 -o DUMBclient DUMBclient.c socketHelper.o

serve: DUMBserver.c


socketHelper.o: socketHelper.c
	gcc -c -std=c99 socketHelper.c

clean: 