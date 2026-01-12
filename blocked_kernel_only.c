// blocked_kernel_only.c
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define INF 9999999.0f

void init_matrix(float *Dist, int N)
{
    int i, j;
    srand(0);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            if (i == j) Dist[i*N + j] = 0.0f;
            else {
                if ((rand() % 100) > 35)
                    Dist[i*N + j] = rand() / (RAND_MAX/1000);
                else
                    Dist[i*N + j] = INF;
            }
        }
}

void blocked_floyd_kernel(float *Dist, int N, int B, int NTHR)
{
    omp_set_num_threads(NTHR);
    for (int bk = 0; bk < N; bk += B) {
        int kend = (bk + B < N) ? (bk + B) : N;

        // Phase 1
        for (int k = bk; k < kend; k++) {
            for (int i = bk; i < kend; i++) {
                float Dik = Dist[i*N + k];
                for (int j = bk; j < kend; j++) {
                    float temp = Dik + Dist[k*N + j];
                    float t2 = Dist[i*N + j];
                    if (t2 > temp) t2 = temp;
                    Dist[i*N + j] = t2;
                }
            }
        }

        // Phase 2a
        #pragma omp parallel for schedule(static)
        for (int bj = 0; bj < N; bj += B) {
            if (bj == bk) continue;
            int jend = (bj + B < N) ? (bj + B) : N;
            for (int k = bk; k < kend; k++) {
                for (int i = bk; i < kend; i++) {
                    float Dik = Dist[i*N + k];
                    for (int j = bj; j < jend; j++) {
                        float temp = Dik + Dist[k*N + j];
                        float t2 = Dist[i*N + j];
                        if (t2 > temp) t2 = temp;
                        Dist[i*N + j] = t2;
                    }
                }
            }
        }

        // Phase 2b
        #pragma omp parallel for schedule(static)
        for (int bi = 0; bi < N; bi += B) {
            if (bi == bk) continue;
            int iend = (bi + B < N) ? (bi + B) : N;
            for (int k = bk; k < kend; k++) {
                for (int i = bi; i < iend; i++) {
                    float Dik = Dist[i*N + k];
                    for (int j = bk; j < kend; j++) {
                        float temp = Dik + Dist[k*N + j];
                        float t2 = Dist[i*N + j];
                        if (t2 > temp) t2 = temp;
                        Dist[i*N + j] = t2;
                    }
                }
            }
        }

        // Phase 3
        #pragma omp parallel for collapse(2) schedule(static)
        for (int bi = 0; bi < N; bi += B) {
            if (bi == bk) continue;
            for (int bj = 0; bj < N; bj += B) {
                if (bj == bk) continue;
                int iend = (bi + B < N) ? (bi + B) : N;
                int jend = (bj + B < N) ? (bj + B) : N;
                for (int k = bk; k < kend; k++) {
                    for (int i = bi; i < iend; i++) {
                        float Dik = Dist[i*N + k];
                        for (int j = bj; j < jend; j++) {
                            float temp = Dik + Dist[k*N + j];
                            float t2 = Dist[i*N + j];
                            if (t2 > temp) t2 = temp;
                            Dist[i*N + j] = t2;
                        }
                    }
                }
            }
        }
    }
}

float checksum_matrix(float *Dist, int N)
{
    float checksum = 0.0f;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            checksum += Dist[i*N + j];
    return checksum;
}

int main(int argc, char **argv)
{
    int N = 3000;
    int NTHR = 1;
    int B = 64;

    if (argc > 1) N = atoi(argv[1]);
    if (argc > 2) NTHR = atoi(argv[2]);
    if (argc > 3) B = atoi(argv[3]);

    printf("blocked_kernel_only: N=%d NTHR=%d B=%d\n", N, NTHR, B);

    float *Dist = (float*)malloc(sizeof(float)*N*N);
    if (!Dist) { printf("malloc failed\n"); return 1; }

    init_matrix(Dist, N);

    double t0 = omp_get_wtime();
    blocked_floyd_kernel(Dist, N, B, NTHR);
    double t1 = omp_get_wtime();

    float cs = checksum_matrix(Dist, N);
    printf("Blocked kernel time=%.6f s  CheckSum=%.0f\n", t1 - t0, cs);

    free(Dist);
    return 0;
}
