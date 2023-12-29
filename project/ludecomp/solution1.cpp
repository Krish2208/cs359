#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <time.h>

using namespace std;

void title()
{
    cout << "LU Decomposition" << endl;
    cout << "Simple Parallel Algorithm" << endl;
}

void lu_decomposition(float **a, float **l, float **u, int size)
{
#pragma omp parallel shared(a, l, u)
    {
        for (int i = 0; i < size; i++)
        {
#pragma omp for
            for (int j = 0; j < size; j++)
            {
                if (j < i)
                {
                    l[j][i] = 0;
                    continue;
                }
                l[j][i] = a[j][i];
                for (int k = 0; k < i; k++)
                {
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
#pragma omp for
            for (int j = 0; j < size; j++)
            {
                if (j < i)
                {
                    u[i][j] = 0;
                    continue;
                }
                if (j == i)
                {
                    u[i][j] = 1;
                    continue;
                }
                u[i][j] = a[i][j] / l[i][i];
                for (int k = 0; k < i; k++)
                {
                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
                }
            }
        }
    }
}

void initialize_matrices(float **a, float **l, float **u, int size)
{
#pragma omp schedule(static)
    for (int i = 0; i < size; i++)
    {
        a[i] = new float[size];
        l[i] = new float[size];
        u[i] = new float[size];
    }
}

int main()
{
    title();
    srand(time(NULL));
    cout << "Size\tRuntime" << endl;
    for (int size = 2; size <= 4096; size *= 2)
    {
        double runtime;
        float **a = new float *[size];
        float **l = new float *[size];
        float **u = new float *[size];
        initialize_matrices(a, l, u, size);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                a[i][j] = (float)((rand() % 10) + 1);
            }
        }
        runtime = omp_get_wtime();
        lu_decomposition(a, l, u, size);
        runtime = omp_get_wtime() - runtime;
        cout << size << '\t' << runtime << endl;
    }
    return 0;
}