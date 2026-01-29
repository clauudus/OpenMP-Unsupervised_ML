#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <omp.h>

#define INF 9999999

static void init_matrix(float Dist[], int N)
{
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++) {
			if (i == j)
				Dist[i * N + j] = 0;
			else {
				if ((rand() % 100) > 35)
					Dist[i * N + j] =
						rand() / (RAND_MAX / 1000);
				else
					Dist[i * N + j] = INF;
			}
		}
}

static void checksum(float Dist[], int N)
{
	int i, j;
	float checksum = 0;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			checksum += Dist[i * N + j];
	printf("\nCheckSum: %.0f \n", checksum);
	if (checksum != 5271068)
		printf("error!\n");
}

inline void floyd_warshall_in_place(float *C, const float *A, const float *B,
				    const int b, const int n)
{
	for (int k = 0; k < b; k++) {
		int kth = k * n;
		for (int i = 0; i < b; i++) {
			for (int j = 0; j < b; j++) {
				int sum = A[i * n + k] + B[kth + j];
				if (C[i * n + j] > sum) {
					C[i * n + j] = sum;
				}
			}
		}
	}
}

void floyd_warshall_blocked(float *output, const int n, const int b)
{
	//b divides n
	const int blocks = n / b;
	int i, j;

	//Diagonal block [k][k] - non paralelizable
	for (int k = 0; k < blocks; k++) {
		floyd_warshall_in_place(&output[k * b * n + k * b],
					&output[k * b * n + k * b],
					&output[k * b * n + k * b], b, n);

		//Row k - paralelizable
		#pragma omp parallel for schedule(static)
		for (j = 0; j < blocks; j++) {
			if (j == k)
				continue;
			floyd_warshall_in_place(&output[k * b * n + j * b],
						&output[k * b * n + k * b],
						&output[k * b * n + j * b], b,
						n);
		}
		
		//Column k + 3rd phase - fully paralelizable
		#pragma omp parallel for private(j) schedule(static)
		for (i = 0; i < blocks; i++) {
			if (i == k)
				continue;
			floyd_warshall_in_place(&output[i * b * n + k * b],
						&output[i * b * n + k * b],
						&output[k * b * n + k * b], b,
						n);
			for (j = 0; j < blocks; j++) {
				if (j == k)
					continue;
				floyd_warshall_in_place(
					&output[i * b * n + j * b],
					&output[i * b * n + k * b],
					&output[k * b * n + j * b], b, n);
			}
		}
	}
}

void floyd(float Dist[], int N, int NTHR)
{
	int i, j, k;

	for (k = 0; k < N; k++) {
		#pragma omp parallel for private(i, j) schedule(static)
		for (i = 0; i < N; i++) {
			float Dik = Dist[i * N + k];
			for (j = 0; j < N; j++) {
				float temp = Dik + Dist[k * N + j];
				float t2 = Dist[i * N + j];
				if (t2 > temp)
					Dist[i * N + j] = temp;
			}
		}
	}
}

int main(int argc, char **argv)
{
	int N = 3000;
	int NTHR = 1;

	if (argc > 1) {
		N = atoi(argv[1]);
	}
	if (argc > 2) {
		NTHR = atoi(argv[2]);
	}

	float *Dist = (float *)malloc(sizeof(float) * N * N);
	init_matrix(Dist, N);

	//floyd(Dist, N, tamaño submatriz);
	floyd_warshall_blocked(Dist, N, 8);
	checksum(Dist, N);
	free(Dist);

	return 0;
}
