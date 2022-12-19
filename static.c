/*
 *   gcc -g -Wall -fopenmp static.c -o static mmio.c
 *   ./static <file_name>.mtx
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mmio.h"
#include "crs.c"
#include "helpers.c"

void calc(
    CRS *crs, double *vec, double *product, int thread_count)
{
    double start, finish;
    int i, j, nz_id;
    start = omp_get_wtime();

#pragma omp parallel for num_threads(thread_count) schedule(static, 1) private(nz_id, i, j) shared(crs, vec, product)
    for (i = 0; i < crs->n_rows; i++)
    {
        int thread_num = omp_get_thread_num();
        int actual_thread_count = omp_get_num_threads();
#pragma omp parallel for num_threads(thread_count)
        for (nz_id = crs->row_ptrs[i]; nz_id < crs->row_ptrs[i + 1]; nz_id++)
        {
            j = crs->col_indices[nz_id];
            product[i] += crs->values[nz_id] * vec[j];
#ifdef DEBUG
            printf("from %d/%d: %d, %d %.3fx%.3f => += %.3f\n", thread_num, actual_thread_count, i, j, crs->values[nz_id], vec[j], crs->values[nz_id] * vec[j]);
#endif
        }
    }
#ifdef DEBUG
    print_vec(product, crs->n_cols);
#endif
    finish = omp_get_wtime();

    printf("thread_count=%d Elapsed time = %.3f seconds\n", thread_count, finish - start);
}

int main(int argc, char *argv[])
{

    int i, len;
    double *vec, *product;
    CRS crs;

    Usage(argc, argv);

    read_crs(argv[1], &crs);

    len = crs.n_cols;

    vec = (double *)malloc(len * sizeof(double));
    product = (double *)malloc(len * sizeof(double));

    initialize_vector(vec, len);

#ifdef DEBUG
    print_crs(&crs);
    print_vec(vec, crs.n_cols);
    print_vec(product, crs.n_cols);
#endif

    int runs[7] = {1, 2, 4, 8, 16, 32, 64};
    for (i = 0; i < sizeof(runs) / sizeof(runs[0]); i++)
    {
        initialize_product(product, len);
        calc(&crs, vec, product, runs[i]);
    }

    return 0;
}
