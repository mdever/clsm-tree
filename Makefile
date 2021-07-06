build/%.o: src/%.c
	gcc -c $< -o $@ -Iheaders

wordcount: build/rb.o build/vector.o
	gcc -g $^ utilities/wordcount.c -lm -o build/wordcount -Iheaders

.PHONY: clean
clean:
	rm build/*

.PHONY: all
all: lsmtree

.PHONY: tests
tests:
	echo "Not Implemented"

lsmtree: build/common.o build/serde.o build/rb.o build/vector.o build/main.o
	gcc -g -I./headers build/common.o build/serde.o build/rb.o build/vector.o build/main.o -lm -o build/lsmtree