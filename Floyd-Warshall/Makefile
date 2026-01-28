.PHONY: run format

floyd_scalar: floyd_scalar.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra

floyd_blocked-matrix: floyd_blocked-matrix.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -fopenmp

floyd_openmp: floyd_openmp.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -fopenmp

run: floyd
	perf stat ./floyd_scalar
	perf stat ./floyd_blocked-matrix
	perf stat ./floyd_openmp

format: format
	clang-format floyd_scalar.c
	clang-format floyd_blocked-matrix.c
	clang-format floyd_openmp.c

clean:
	rm -f floyd_scalar
	rm -f floyd_blocked-matrix
	rm -f floyd_openmp
