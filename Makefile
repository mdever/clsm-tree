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

.PHONY: debug
debug:
	gcc -g src/record.c src/bloom.c src/serde.c src/rb.c src/vector.c src/hashmap.c src/lsmt.c src/main.c -lm -Iheaders -o build/lsmtree

lsmtree: build/record.o build/bloom.o build/serde.o build/rb.o build/vector.o build/hashmap.o build/lsmt.o build/main.o
	gcc -g -I./headers build/record.o build/bloom.o build/serde.o build/rb.o build/vector.o build/hashmap.o build/lsmt.o build/main.o -lm -o build/lsmtree