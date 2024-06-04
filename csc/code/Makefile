CC := gcc
HEADER := -I../include

.PHONY: clean

all: target

target: fibo.o main.o

fibo.o: fibo.c
	$(CC) $(HEADER) -c fibo.c -o fibo.o

main.o: main.c
	$(CC) $(HEADER) -c main.c -o main.o

clean:
	rm *.o
