#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <inttypes.h>

extern "C" bool AvxPfpColMeans(const double* x, int nrows, int ncols, double* col_means);

double* aligned_malloc(int n, int alignment){
    void* ptrx;
    int rc = posix_memalign(&ptrx,alignment,n*sizeof(double));
    return (double*)ptrx;
}

bool AvxPfpColMeansCpp(const double* x, int nrows, int ncols, double* col_means)
{
    // Make sure nrows and ncols are valid
    if ((nrows <= 0) || (ncols <= 0))
        return false;

    // Make sure col_means is properly aligned
    if (((uintptr_t)col_means & 0x1f) != 0)
        return false;

    // Calculate column means
    memset(col_means, 0, ncols * sizeof(double));

    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
            col_means[j] += x[i * ncols + j];
    }

    for (int j = 0; j < ncols; j++)
        col_means[j] /= nrows;

    return true;
}

int main(int argc, char* argv[])
{
    const int nrows = 13;
    const int ncols = 11;
	double* x = (double*)malloc(nrows * ncols * sizeof(double));
    double* col_means1 = (double*)aligned_malloc(ncols, 32);
    double* col_means2 = (double*)aligned_malloc(ncols, 32);

    srand(47);
    rand();

    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
            x[i * ncols + j] = rand() % 511;
    }

    bool rc1 = AvxPfpColMeansCpp(x, nrows, ncols, col_means1);
    bool rc2 = AvxPfpColMeans(x, nrows, ncols, col_means2);
	
    printf("Results for sample program AvxPackedFloatingPointColMeans\n");

    if (rc1 != rc2)
    {
        printf("Bad return code (rc1 = %d, rc2 = %d)\n", rc1, rc2);
        return 1;
    }

    printf("\nTest Matrix\n");
    for (int i = 0; i < nrows; i++)
    {
        printf("row %2d: ", i);
        for (int j = 0; j < ncols; j++)
            printf("%5.0lf ", x[i * ncols + j]);
        printf("\n");
    }
    printf("\n");

    for (int j = 0; j < ncols; j++)
    {
        printf("col_means1[%2d]: %12.4lf  ", j, col_means1[j]);
        printf("col_means2[%2d]: %12.4lf  ", j, col_means2[j]);
        printf("\n");
    }
    
    free(x);
    free(col_means1);
    free(col_means2);
	
    return 0;
}