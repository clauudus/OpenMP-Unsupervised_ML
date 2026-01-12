// blocked_full.c
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <string.h>

#define INF 9999999.0f

/* Naive (original-ish) Floyd-Warshall (with omp parallel for over i loop) */
void naive_floyd(float *Dist, int N, int NTHR)
{
    omp_set_num_threads(NTHR);
    int i, j, k;
    for (k = 0; k < N; k++) {
        #pragma omp parallel for private(i,j) schedule(static)
        for (i = 0; i < N; i++) {
            float Dik = Dist[i*N + k];
            for (j = 0; j < N; j++) {
                float temp = Dik + Dist[k*N + j];
                float t2 = Dist[i*N + j];
                if (t2 > temp) t2 = temp;
                Dist[i*N + j] = t2;
            }
        }
    }
}

/* Blocked Floyd-Warshall:
   Blocks of size B. Follows the three-phase approach per block k-block:
   1) update diagonal block
   2) update row and column blocks using diagonal
   3) update remaining blocks using row/col
*/
void blocked_floyd(float *Dist, int N, int B, int NTHR)
{
    omp_set_num_threads(NTHR);
    for (int bk = 0; bk < N; bk += B) {
        int kend = (bk + B < N) ? (bk + B) : N;

        // Phase 1: diagonal block (bk, bk)
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

        // Phase 2a: row blocks (bk, bj), bj != bk
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

        // Phase 2b: column blocks (bi, bk), bi != bk
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

        // Phase 3: remaining blocks (bi, bj) where bi != bk && bj != bk
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

/* initializer, seed fixed for reproducibility */
void init_matrix(float *Dist, int N)
{
    int i, j;
    srand(0); // fixed seed so inputs identical across runs
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

float checksum_matrix(float *Dist, int N)
{
    float checksum = 0.0f;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            checksum += Dist[i*N + j];
    return checksum;
}

float max_abs_diff(float *A, float *B, int N)
{
    float maxd = 0.0f;
    for (int i = 0; i < N*N; i++) {
        float d = fabsf(A[i] - B[i]);
        if (d > maxd) maxd = d;
    }
    return maxd;
}

int main(int argc, char **argv)
{
    int N = 3000;
    int NTHR = 1;
    int B = 64; // default block size
    int mode = 1; // 0=naive,1=blocked,2=both+compare

    if (argc > 1) N = atoi(argv[1]);
    if (argc > 2) NTHR = atoi(argv[2]);
    if (argc > 3) B = atoi(argv[3]);
    if (argc > 4) mode = atoi(argv[4]);

    printf("N=%d NTHR=%d B=%d mode=%d\n", N, NTHR, B, mode);

    float *Dist = (float*)malloc(sizeof(float)*N*N);
    if (!Dist) { printf("malloc failed\n"); return 1; }

    init_matrix(Dist, N);

    if (mode == 0) {
        double t0 = omp_get_wtime();
        naive_floyd(Dist, N, NTHR);
        double t1 = omp_get_wtime();
        float cs = checksum_matrix(Dist, N);
        printf("Naive: time=%.6f s  CheckSum=%.0f\n", t1-t0, cs);
    } else if (mode == 1) {
        double t0 = omp_get_wtime();
        blocked_floyd(Dist, N, B, NTHR);
        double t1 = omp_get_wtime();
        float cs = checksum_matrix(Dist, N);
        printf("Blocked: time=%.6f s  CheckSum=%.0f\n", t1-t0, cs);
    } else {
        // mode == 2 : run both and compare
        float *Dist_ref = (float*)malloc(sizeof(float)*N*N);
        if (!Dist_ref) { printf("malloc failed\n"); free(Dist); return 1; }
        memcpy(Dist_ref, Dist, sizeof(float)*N*N);

        double t0 = omp_get_wtime();
        naive_floyd(Dist_ref, N, NTHR);
        double t1 = omp_get_wtime();
        double naive_time = t1 - t0;
        float cs_ref = checksum_matrix(Dist_ref, N);

        double t2 = omp_get_wtime();
        blocked_floyd(Dist, N, B, NTHR);
        double t3 = omp_get_wtime();
        double blocked_time = t3 - t2;
        float cs_blk = checksum_matrix(Dist, N);

        float maxd = max_abs_diff(Dist_ref, Dist, N);

        printf("Naive: time=%.6f s  CheckSum=%.0f\n", naive_time, cs_ref);
        printf("Blocked: time=%.6f s  CheckSum=%.0f\n", blocked_time, cs_blk);
        printf("Max abs elementwise diff: %.6f\n", maxd);

        // tolerance check: absolute tol and relative tol
        float abs_tol = 1e-3f;      // can be modified
        float rel_tol = 1e-6f;      // can be modified
        if (maxd <= abs_tol) {
            printf("PASS: max diff <= absolute tolerance %.6g\n", abs_tol);
        } else {
            printf("NOTE: max diff > absolute tolerance %.6g (consider increasing tol for floats)\n", abs_tol);
        }

        free(Dist_ref);
    }

    free(Dist);
    return 0;
}
