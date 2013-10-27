#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
    // Matrix A: M by K
    // Matrix B: K by N
    // C = A * B
    int M, K, N;
    int **A, **B, **C;
    int i, j, k;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "Usage: mm <input>\n");
        return EXIT_FAILURE;
    }

    // Read matrix sizes.
    fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &M);
    fscanf(fp, "%d", &K);
    fscanf(fp, "%d", &N);

    // Read first matrix.
    A = calloc(M, sizeof(int *));
    for (i = 0; i < M; ++i) {
        A[i] = calloc(K, sizeof(int));
        for (j = 0; j < K; ++j) {
            fscanf(fp, "%d", &A[i][j]);
        }
    }

    // Read second matrix.
    B = calloc(K, sizeof(int *));
    for (i = 0; i < K; ++i) {
        B[i] = calloc(N, sizeof(int));
        for (j = 0; j < N; ++j) {
            fscanf(fp, "%d", &B[i][j]);
        }
    }

    fclose(fp);

    // Allocate third matrix.
    C = calloc(M, sizeof(int *));
    for (i = 0; i < M; ++i) {
        C[i] = calloc(N, sizeof(int));
        for (j = 0; j < N; ++j) {
            C[i][j] = 0;
        }
    }

    // Multiply.
    for (i = 0; i < M; ++i) {
        for (j = 0; j < K; ++j) {
            for(k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return EXIT_SUCCESS;
}
