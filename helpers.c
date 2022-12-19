#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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