#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// GENERAL
void Usage(int argc, char *argv[])
{
    if (argc < 1)
    {
        fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
        exit(1);
    }
}

void initialize_vector(double *vec, int len)
{
    time_t t;
    int j;
    srand((unsigned)time(&t));
    for (j = 0; j < len; j++)
    {
        vec[j] = (double)j;
        // vec[j] = (rand() % 1000 - 500.0) / 100.0;
    }
}

void initialize_product(double *product, int len)
{
    int j;
    for (j = 0; j < len; j++)
    {
        product[j] = 0.0;
    }
}

void print_vec(double *product, int sz)
{
    int i;
    for (i = 0; i < sz; i++)
    {
        printf("%d: %.3f\n", i + 1, product[i]);
    }
}

// CRS
typedef struct CRS
{
    int n_rows;
    int n_cols;
    int n_nz;
    int *row_ptrs;
    int *col_indices;
    double *values;
} CRS;

void print_crs(CRS *crs)
{
    int i, j;
    for (i = 0; i < crs->n_rows; i++)
    {
        printf("!%d, %d\n", crs->row_ptrs[i], crs->row_ptrs[i + 1]);
        for (j = crs->row_ptrs[i]; j < crs->row_ptrs[i + 1]; j++)
            printf("%d %d %lg\n", crs->col_indices[j] + 1, i + 1, crs->values[j]);
    }
}

int read_crs(
    const char *f_name,
    CRS *crs)
{
    MM_typecode matcode;
    FILE *f;
    int ret_code;
    int nz, j = 0;

    if ((f = fopen(f_name, "r")) == NULL)
    {
        exit(1);
    }
    int code = mm_read_banner(f, &matcode);

    if (code != 0)
    {
        printf("Could not process Matrix Market banner. %d \n", code);
        exit(1);
    }

    if ((ret_code = mm_read_mtx_crd_size(f, &crs->n_cols, &crs->n_rows, &crs->n_nz)) != 0)
        exit(1);
    mm_write_mtx_crd_size(stdout, crs->n_cols, crs->n_rows, crs->n_nz);
    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode))
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    crs->row_ptrs = calloc(crs->n_rows + 1, sizeof(int));
    crs->col_indices = calloc(crs->n_nz, sizeof(int));
    crs->values = calloc(crs->n_nz, sizeof(double));

    crs->row_ptrs[0] = 0;
    for (nz = 0; nz < crs->n_nz; nz++)
    {
        fscanf(f, "%d %d %lg\n", &crs->col_indices[nz], &j, &crs->values[nz]);
        crs->col_indices[nz]--;
        crs->row_ptrs[j] = nz + 1;
    }

    if (f != stdin)
        fclose(f);

    return EXIT_SUCCESS;
}

// COO
typedef struct COO
{
    int n_rows;
    int n_cols;
    int n_nz;
    int *row_indices;
    int *col_indices;
    double *values;
} COO;

void print_COO(COO *coo)
{
    int i;
    for (i = 0; i < coo->n_nz; i++)
    {
        printf("[%d, %d]: %.3f\n", coo->col_indices[i], coo->row_indices[i], coo->values[i]);
    }
}

int read_COO(
    const char *f_name,
    COO *coo)
{
    MM_typecode matcode;
    FILE *f;
    int ret_code;
    int i;

    if ((f = fopen(f_name, "r")) == NULL)
    {
        exit(1);
    }
    int code = mm_read_banner(f, &matcode);

    if (code != 0)
    {
        printf("Could not process Matrix Market banner. %d \n", code);
        exit(1);
    }

    if ((ret_code = mm_read_mtx_crd_size(f, &coo->n_cols, &coo->n_rows, &coo->n_nz)) != 0)
        exit(1);
    mm_write_mtx_crd_size(stdout, coo->n_cols, coo->n_rows, coo->n_nz);
    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode))
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    coo->row_indices = calloc(coo->n_nz, sizeof(int));
    coo->col_indices = calloc(coo->n_nz, sizeof(int));
    coo->values = calloc(coo->n_nz, sizeof(double));

    for (i = 0; i < coo->n_nz; i++)
    {
        fscanf(f, "%d %d %lg\n", &coo->col_indices[i], &coo->row_indices[i], &coo->values[i]);
        coo->col_indices[i] -= 1;
        coo->row_indices[i] -= 1;
    }

    if (f != stdin)
        fclose(f);

    return EXIT_SUCCESS;
}