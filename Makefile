%.o: %.c
	gcc -c $< -o $@

wordcount: rb.o vector.o wordcount.o
	gcc $^ -lm -o wordcount

.PHONY: clean
clean:
	rm *.o
	rm *.exe

.PHONY: all
all: lsmtree

lsmtree: common.o rb.o vector.o main.o
	gcc common.o rb.o vector.o main.o -lm -o lsmtree