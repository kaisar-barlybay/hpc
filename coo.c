/*
 *   gcc -g -Wall -fopenmp coo.c -o coo mmio.c
 *   ./coo <file_name>.mtx
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mmio.h"
#include "helpers.c"

void calc(
    COO *coo, double *vec, double *product, int thread_count, FILE *fptr)
{
    double start, finish;
    int i, j, nz_id;
    start = omp_get_wtime();
#pragma omp parallel for num_threads(thread_count) schedule(static, 1) private(nz_id, i, j) shared(coo, vec, product)
    for (nz_id = 0; nz_id < coo->n_nz; nz_id++)
    {
#ifdef DEBUG
        int thread_num = omp_get_thread_num();
        int actual_thread_count = omp_get_num_threads();
        printf("from %d/%d: %d, %d %.3fx%.3f => += %.3f\n", thread_num, actual_thread_count, i, j, coo->values[nz_id], vec[j], coo->values[nz_id] * vec[j]);
#endif
        i = coo->row_indices[nz_id];
        j = coo->col_indices[nz_id];
        product[i] += coo->values[nz_id] * vec[j];
    }
    finish = omp_get_wtime();
#ifdef DEBUG
    print_vec(product, coo->n_cols);
#endif

    printf("%.3f\t", finish - start);
    fprintf(fptr, "%.3f\t", finish - start);
}

int main(int argc, char *argv[])
{

    int i, len;
    double *vec, *product;
    COO coo;
    FILE *fptr;
    fptr = fopen("coo_output.txt", "w");

    Usage(argc, argv);

    read_COO(argv[1], &coo);

    len = coo.n_cols;

    vec = (double *)malloc(len * sizeof(double));
    product = (double *)malloc(len * sizeof(double));

    initialize_vector(vec, len);
#ifdef DEBUG
    print_COO(&coo);
    print_vec(vec, coo.n_cols);
    print_vec(product, coo.n_cols);
#endif
    int runs[7] = {1, 2, 4, 8, 16, 32, 64};
    for (i = 0; i < sizeof(runs) / sizeof(runs[0]); i++)
    {
        initialize_product(product, len);
        calc(&coo, vec, product, runs[i], fptr);
    }
    fclose(fptr);

    return 0;
}
