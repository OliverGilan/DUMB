all: client serve

client: DUMBclient.c socketHelper.o 
	gcc -std=gnu11 -O3 -o DUMBclient DUMBclient.c socketHelper.o

serve: DUMBserver.c socketHelper.o list.o messageBox.o
	gcc -std=gnu11 -pthread -o DUMBserve DUMBserver.c socketHelper.o list.o messageBox.o

socketHelper.o: socketHelper.c
	gcc -std=gnu11 -c socketHelper.c

list.o: list.c
	gcc -std=gnu11 -c list.c

messageBox.o: messageBox.c
	gcc -std=gnu11 -c messageBox.c

clean: 
	rm -rf DUMBclient DUMBserve *.o