build/%.o: src/%.c
	gcc -c $< -o $@ -Iheaders

wordcount: rb.o vector.o wordcount.o
	gcc -g $^ -lm -o wordcount -Iheaders

.PHONY: clean
clean:
	rm build/*

.PHONY: all
all: lsmtree

lsmtree: build/common.o build/serde.o build/rb.o build/vector.o build/main.o
	gcc -g -I./headers build/common.o build/serde.o build/rb.o build/vector.o build/main.o -lm -o build/lsmtree