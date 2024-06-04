CC := gcc


all : target

target: hello.c
	$(CC) -o hello hello.c

run : hello
	./hello

clean : hello
	rm hello
