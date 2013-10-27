#include <stdlib.h>
#include <stdio.h>

/* Allocate memory for an m by n matrix. */
void alloc_matrix(int m, int n, float ***a);

/* Free memory for an m by n matrix. */
void free_matrix(int m, float **a);

/* Read an m by n matrix from an open file. */
void read_matrix(FILE *fp, int m, int n, float **a);

/*
 * Multiply two matrices.
 *
 * a: m by k
 * b: k by n
 * c: a * b (result)
 */
void mult_matrix(int m, int k, int n, float **a, float **b, float **c);

int main(int argc, char *argv[]) {
    int M, K, N;
    float **A = NULL,
          **B = NULL,
          **C = NULL;
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

    // Allocate memory for matrices.
    alloc_matrix(M, K, &A);
    alloc_matrix(K, N, &B);
    alloc_matrix(M, N, &C);

    // Read matrices from file.
    read_matrix(fp, M, K, A);
    read_matrix(fp, K, N, B);

    fclose(fp);

    // Multiply.
    mult_matrix(M, K, N, A, B, C);

    free_matrix(M, A);
    free_matrix(K, B);
    free_matrix(M, C);

    return EXIT_SUCCESS;
}

void alloc_matrix(int m, int n, float ***a) {
    int i;
    (*a) = (float **) calloc(m, sizeof(float *));
    for (i = 0; i < m; ++i) {
        (*a)[i] = (float *) calloc(n, sizeof(float));
    }
}

void free_matrix(int m, float **a) {
    int i;
    for (i = 0; i < m; ++i) {
        free(a[i]);
    }
    free(a);
}

void read_matrix(FILE *fp, int m, int n, float **a) {
    int i, j;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            fscanf(fp, "%f", &a[i][j]);
        }
    }
}
void mult_matrix(int m, int k, int n, float **a, float **b, float **c) {
    int i, j, h;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < k; ++j) {
            c[i][j] = 0;
            for(h = 0; h < n; ++h) {
                c[i][j] += a[i][h] * b[h][j];
            }
        }
    }
}
