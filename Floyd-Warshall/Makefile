.PHONY: run

floyd: floyd.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -lgomp -fopenmp

run: floyd
	perf stat ./floyd

clean:
	rm -f floydmod
