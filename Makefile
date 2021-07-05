%.o: %.c
	gcc -c $< -o $@

wordcount: rb.o vector.o wordcount.o
	gcc $^ -lm -o wordcount

.PHONY: clean
clean:
	rm *.o