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

    for (i = 0; i < crs->n_nz; i++)
    {
        crs->row_ptrs[j] = i;
        fscanf(f, "%d %d %lg\n", &crs->col_indices[i], &j, &crs->values[i]);
        crs->col_indices[i]--;
    }

    crs->row_ptrs[j] = i;

    if (f != stdin)
        fclose(f);

    return EXIT_SUCCESS;
}