.PHONY: run format

floyd_scalar: floyd_scalar.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra

floyd_blocked-matrix: floyd_blocked_matrix.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -fopenmp

floyd_openmp: floyd_openmp.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra -fopenmp

run: floyd
	perf stat ./floyd_scalar
	perf stat ./floyd_blocked_matrix
	perf stat ./floyd_openmp

format:
	clang-format -i floyd_scalar.c
	clang-format -i floyd_blocked_matrix.c
	clang-format -i floyd_openmp.c

clean:
	rm -f floyd_scalar
	rm -f floyd_blocked_matrix
	rm -f floyd_openmp
