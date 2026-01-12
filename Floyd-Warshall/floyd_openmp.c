#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

void floyd(float Dist[], int N, int NTHR)
{
	int i, j, k;

	init_matrix(Dist, N);

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

	checksum(Dist, N);
	
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

	floyd(Dist, N, NTHR);
	free(Dist);

	return 0;
}
