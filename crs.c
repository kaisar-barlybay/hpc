typedef struct Sparse_CSR
{
    int n_rows;
    int n_cols;
    int n_nz;
    int *row_ptrs;
    int *col_indices;
    double *values;
} Sparse_CSR;

void initialize_vectors(double *vec, double *product, int len)
{
    int j;
    for (j = 0; j < len; j++)
    {
        vec[j] = (double)j;
        // vec[j] = rand() % 50;
        product[j] = 0.0;
    }
}

void print_sparse_csr(Sparse_CSR *ssr)
{
    int i, j;
    for (i = 0; i < ssr->n_rows; i++)
    {
        printf("!%d, %d\n", ssr->row_ptrs[i], ssr->row_ptrs[i + 1]);
        for (j = ssr->row_ptrs[i]; j < ssr->row_ptrs[i + 1]; j++)
            printf("%d %d %lg\n", ssr->col_indices[j] + 1, i + 1, ssr->values[j]);
    }
}

void Usage(int argc, char *argv[])
{
    if (argc < 1)
    {
        fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
        exit(1);
    }
}

int create_sparse_csr(
    const char *f_name,
    Sparse_CSR *ssr)
{
    MM_typecode matcode;
    FILE *f;
    int ret_code;
    int i, j;

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

    if ((ret_code = mm_read_mtx_crd_size(f, &ssr->n_cols, &ssr->n_rows, &ssr->n_nz)) != 0)
        exit(1);
    mm_write_mtx_crd_size(stdout, ssr->n_cols, ssr->n_rows, ssr->n_nz);
    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode))
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    ssr->row_ptrs = calloc(ssr->n_rows + 1, sizeof(int));
    ssr->col_indices = calloc(ssr->n_nz, sizeof(int));
    ssr->values = calloc(ssr->n_nz, sizeof(double));

    for (i = 0; i < ssr->n_nz; i++)
    {
        ssr->row_ptrs[j] = i;
        fscanf(f, "%d %d %lg\n", &ssr->col_indices[i], &j, &ssr->values[i]);
        ssr->col_indices[i]--;
    }

    ssr->row_ptrs[j] = i;

    if (f != stdin)
        fclose(f);

    return EXIT_SUCCESS;
}