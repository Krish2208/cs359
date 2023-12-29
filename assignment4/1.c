#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define N 100000
int A[N];
long long serial_ans[N];
long long par_ans[N];

int main()
{

    clock_t start, end;
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        A[i] = rand();
    }

    // Sequential Code
    start = clock();
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (A[i] % 5 != 0 && A[i] % 2 != 0)
        {
            serial_ans[count] = A[i];
            count++;
        }
    }

    end = clock();
    double seq_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Serial Time: %lf\n", seq_time);

    // Parallel Code
    start = clock();
    int C[N];
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (A[i] % 5 != 0 && A[i] % 2 != 0)
        {
            C[i] = 1;
        }
        else
        {
            C[i] = 0;
            A[i] = -1;
        }
    }
    int c[N];
    for (int h = 1; h < N; h *= 2)
    {
#pragma omp parallel for
        for (int i = 0; i < N; i++)
        {
            c[i] = C[i];
        }
#pragma omp parallel for
        for (int i = 0; i < N - h; i++)
        {
            C[i + h] += c[i];
        }
    }
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (A[i] != -1)
        {
            par_ans[C[i] - 1] = A[i];
        }
    }
    end = clock();
    double par_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Parallel Time: %lf\n", par_time);
    printf("\nSpeed up: %lf\n", (seq_time / par_time));
    return 0;
}