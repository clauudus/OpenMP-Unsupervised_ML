.PHONY: run

kmeans_scalar: kmeans_scalar.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra

kmeans_openmp: kmeans_openmp.c
	gcc -O3 $^ -march=native -o $@ -Wall -Wextra-fopenmp

run: kmeans_openmp
	perf stat ./kmeans_openmp

run: kmeans_scalar
	perf stat ./kmeans_scalar

clean:
	rm -f kmeans_scalar
	rm -f kmeans_openmp
