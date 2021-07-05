%.o: %.c
	gcc -c $< -o $@

wordcount: rb.o vector.o wordcount.o
	gcc -g $^ -lm -o wordcount

.PHONY: clean
clean:
	rm *.o
	rm *.exe

.PHONY: all
all: lsmtree

lsmtree: common.o serde.o rb.o vector.o main.o
	gcc -g common.o serde.o rb.o vector.o main.o -lm -o lsmtree