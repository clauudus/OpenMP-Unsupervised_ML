/*
    This code is not part of my work, it was provided through the Virtual Campus from the Advanced Architectures course, from Computer Science bachelor degree.
    I am adding this code here only to illustrate the beginning of all the implementations.
    All rights reserved its respective the authors.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define INF 9999999

void floyd(float Dist[], int N, int NTHR)
{
	int i, j, k;

	//Creación matriz
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

	for (k = 0; k < N; k++)
		for (j = 0; j < N; j++)
			for (i = 0; i < N; i++)
				if (Dist[i * N + j] >
				    Dist[i * N + k] + Dist[k * N + j])
					Dist[i * N + j] = Dist[i * N + k] +
							  Dist[k * N + j];

	//Cheksum consola
	float checksum = 0;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			checksum += Dist[i * N + j];
	printf("\nCheckSum: %.0f \n", checksum);
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

	system("pause");

	return 0;
}
