/*
 *   gcc -g -Wall -fopenmp crs.c -o crs mmio.c
 *   ./crs <file_name>.mtx
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mmio.h"
#include "helpers.c"

void calculate_static(
    CRS *crs, double *vec, double *product, int thread_count, FILE *fptr)
{
    double start, finish;
    int i, j, nz_id;
    start = omp_get_wtime();

#pragma omp parallel for num_threads(thread_count) schedule(static, 1) private(nz_id, i, j) shared(crs, vec, product)
    for (i = 0; i < crs->n_rows; i++)
    {
#pragma omp parallel for num_threads(thread_count) schedule(static, 1)
        for (nz_id = crs->row_ptrs[i]; nz_id < crs->row_ptrs[i + 1]; nz_id++)
        {
            j = crs->col_indices[nz_id];
            product[i] += crs->values[nz_id] * vec[j];
#ifdef DEBUG
            int thread_num = omp_get_thread_num();
            int actual_thread_count = omp_get_num_threads();
            printf("from %d/%d: %d, %d %.3fx%.3f => += %.3f\n", thread_num, actual_thread_count, i, j, crs->values[nz_id], vec[j], crs->values[nz_id] * vec[j]);
#endif
        }
    }
#ifdef DEBUG
    print_vec(product, crs->n_cols);
#endif
    finish = omp_get_wtime();

    printf("thread_count=%d Elapsed time = %.3f seconds\n", thread_count, finish - start);
    fprintf(fptr, "%.3f\t", finish - start);
}

void calculate_dynamic(
    CRS *crs, double *vec, double *product, int thread_count, FILE *fptr)
{
    double start, finish;
    int i, j, nz_id;
    start = omp_get_wtime();

#pragma omp parallel for num_threads(thread_count) schedule(dynamic, 1) private(nz_id, i, j) shared(crs, vec, product)
    for (i = 0; i < crs->n_rows; i++)
    {
#pragma omp parallel for num_threads(thread_count) schedule(dynamic, 1)
        for (nz_id = crs->row_ptrs[i]; nz_id < crs->row_ptrs[i + 1]; nz_id++)
        {
            j = crs->col_indices[nz_id];
            product[i] += crs->values[nz_id] * vec[j];
#ifdef DEBUG
            int thread_num = omp_get_thread_num();
            int actual_thread_count = omp_get_num_threads();
            printf("from %d/%d: %d, %d %.3fx%.3f => += %.3f\n", thread_num, actual_thread_count, i, j, crs->values[nz_id], vec[j], crs->values[nz_id] * vec[j]);
#endif
        }
    }
#ifdef DEBUG
    print_vec(product, crs->n_cols);
#endif
    finish = omp_get_wtime();

    printf("thread_count=%d Elapsed time = %.3f seconds\n", thread_count, finish - start);
    fprintf(fptr, "%.3f\t", finish - start);
}

void calculate_guided(
    CRS *crs, double *vec, double *product, int thread_count, FILE *fptr)
{
    double start, finish;
    int i, j, nz_id;
    start = omp_get_wtime();

#pragma omp parallel for num_threads(thread_count) schedule(guided, 1) private(nz_id, i, j) shared(crs, vec, product)
    for (i = 0; i < crs->n_rows; i++)
    {
#pragma omp parallel for num_threads(thread_count) schedule(guided, 1)
        for (nz_id = crs->row_ptrs[i]; nz_id < crs->row_ptrs[i + 1]; nz_id++)
        {
            j = crs->col_indices[nz_id];
            product[i] += crs->values[nz_id] * vec[j];
#ifdef DEBUG
            int thread_num = omp_get_thread_num();
            int actual_thread_count = omp_get_num_threads();
            printf("from %d/%d: %d, %d %.3fx%.3f => += %.3f\n", thread_num, actual_thread_count, i, j, crs->values[nz_id], vec[j], crs->values[nz_id] * vec[j]);
#endif
        }
    }
#ifdef DEBUG
    print_vec(product, crs->n_cols);
#endif
    finish = omp_get_wtime();

    printf("thread_count=%d Elapsed time = %.3f seconds\n", thread_count, finish - start);
    fprintf(fptr, "%.3f\t", finish - start);
}

int main(int argc, char *argv[])
{

    int i, len;
    double *vec, *product;
    CRS crs;
    FILE *fptr;
    const char *f_name;

    Usage(argc, argv);

    char output_filename[32];
    f_name = argv[1];
    snprintf(output_filename, sizeof(char) * 32, "crs_%s_output.txt", f_name);
    fptr = fopen(output_filename, "w");
    read_crs(f_name, &crs);

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
    // static
    fprintf(fptr, "\nstatic, crs\t");
    printf("\nstatic, crs\t");
    for (i = 0; i < sizeof(runs) / sizeof(runs[0]); i++)
    {
        initialize_product(product, len);
        calculate_static(&crs, vec, product, runs[i], fptr);
    }

    // dynamic
    fprintf(fptr, "\ndynamic, crs\t");
    printf("\ndynamic, crs\t");
    for (i = 0; i < sizeof(runs) / sizeof(runs[0]); i++)
    {
        initialize_product(product, len);
        calculate_dynamic(&crs, vec, product, runs[i], fptr);
    }

    // guided
    fprintf(fptr, "\nguided, crs\t");
    printf("\nguided, crs\t");
    for (i = 0; i < sizeof(runs) / sizeof(runs[0]); i++)
    {
        initialize_product(product, len);
        calculate_guided(&crs, vec, product, runs[i], fptr);
    }

    fclose(fptr);

    return 0;
}
