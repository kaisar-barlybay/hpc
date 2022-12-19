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