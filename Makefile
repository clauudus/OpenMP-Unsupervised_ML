.PHONY: all floyd_targets kmeans_targets run run_floyd run_kmeans format clean

# --- Floyd-Warshall ---

floyd_scalar: Floyd-Warshall/floyd_scalar.c
	gcc -O3 $^ -march=native -o floyd_scalar -Wall -Wextra

floyd_blocked-matrix: Floyd-Warshall/floyd_blocked_matrix.c
	gcc -O3 $^ -march=native -o floyd_blocked_matrix -Wall -Wextra -fopenmp

floyd_openmp: Floyd-Warshall/floyd_openmp.c
	gcc -O3 $^ -march=native -o floyd_openmp -Wall -Wextra -fopenmp

floyd_paralalize_k: Floyd-Warshall/floyd_paralalize_k.c
	gcc -O3 $^ -march=native -o floyd_paralalize_k -Wall -Wextra -fopenmp

run_floyd: floyd_scalar floyd_blocked-matrix floyd_openmp
	perf stat ./floyd_scalar
	perf stat ./floyd_blocked_matrix
	perf stat ./floyd_openmp
  perf stat ./floyd_paralalize_k

format_floyd:
	clang-format -i Floyd-Warshall/floyd_scalar.c
	clang-format -i Floyd-Warshall/floyd_blocked_matrix.c
	clang-format -i Floyd-Warshall/floyd_openmp.c
  clang-format -i Floyd-Warshall/floyd_paralalize_k.c

clean_floyd:
	rm -f floyd_scalar
	rm -f floyd_blocked_matrix
	rm -f floyd_openmp
  rm -f floyd_paralalize_k

# --- K-means ---

kmeans_scalar: K-means/kmeans_scalar.c
	gcc -O3 $^ -march=native -o kmeans_scalar -Wall -Wextra

kmeans_openmp: K-means/kmeans_openmp.c
	gcc -O3 $^ -march=native -o kmeans_openmp -Wall -Wextra -fopenmp

run_kmeans: kmeans_scalar kmeans_openmp
	perf stat ./kmeans_scalar
	perf stat ./kmeans_openmp

format_kmeans:
	clang-format -i K-means/kmeans_scalar.c
	clang-format -i K-means/kmeans_openmp.c

clean_kmeans:
	rm -f kmeans_scalar
	rm -f kmeans_openmp

# --- Targets at main directory ---

all: floyd_scalar kmeans_scalar kmeans_openmp floyd_blocked-matrix floyd_openmp

run: run_floyd run_kmeans

format: format_floyd format_kmeans

clean: clean_floyd clean_kmeans
