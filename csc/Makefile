CC := gcc

.PHONY: clean

all: target

target: code/fibo.o code/main.o
	gcc code/*.o -o fibo

code/fibo.o:
	cd code && make

code/main.o:
	cd code && make

clean:
	rm code/*.o
