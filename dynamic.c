/*
 *   gcc -g -Wall -fopenmp dynamic.c -o dynamic mmio.c
 *   ./dynamic <file_name>.mtx
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mmio.h"
#include "crs.c"

void calc(
    Sparse_CSR *ssr, double *vec, double *product, int thread_count)
{
    double start, finish;
    int i, j, nz_id;
    start = omp_get_wtime();
#pragma omp parallel num_threads(thread_count) private(nz_id, i, j) shared(ssr, vec, product)
    {
#pragma omp parallel for schedule(dynamic, 1)
        for (i = 0; i < ssr->n_rows; i++)
        {
            for (nz_id = ssr->row_ptrs[i]; nz_id < ssr->row_ptrs[i + 1]; nz_id++)
            {
                j = ssr->col_indices[nz_id];
                product[i] += ssr->values[nz_id] * vec[j];
            }
        }
    }
    finish = omp_get_wtime();

    printf("thread_count=%d Elapsed time = %.3f seconds\n", thread_count, finish - start);
}

int main(int argc, char *argv[])
{

    int i, len;
    double *vec, *product;
    Sparse_CSR ssr;

    Usage(argc, argv);

    create_sparse_csr(argv[1], &ssr);

    len = ssr.n_cols;

    vec = (double *)malloc(len * sizeof(double));
    product = (double *)malloc(len * sizeof(double));

    initialize_vectors(vec, product, len);
    int runs[7] = {1, 2, 4, 8, 16, 32, 64};
    for (i = 0; i < sizeof(runs) / sizeof(runs[0]); i++)
    {
        calc(&ssr, vec, product, runs[i]);
    }

    return 0;
}
