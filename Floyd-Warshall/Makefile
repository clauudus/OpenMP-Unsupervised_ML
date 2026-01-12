.PHONY: run

floyd_scalar: floyd_scalar.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -lgomp -fopenmp

floyd_blocked-matrix: floyd_blocked-matrix.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -lgomp -fopenmp

floyd_openmp: floyd_openmp.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -lgomp -fopenmp

run: floyd_scalar
	perf stat ./floyd_scalar

run: floyd_blocked-matrix
	perf stat ./floyd_blocked-matrix

run: floyd_openmp
	perf stat ./floyd_openmp

clean:
	rm -f floyd_scalar
	rm -f floyd_blocked-matrix
	rm -f floyd_openmp
