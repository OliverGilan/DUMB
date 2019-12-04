all:

client: DUMBclient.c socketHelper.o
	gcc -c=99 -o DUMBclient DUMBclient.c socketHelper.o

serve: DUMBserver.c


socketHelper.o: socketHelper.c
	gcc -c socketHelper.c

clean: 