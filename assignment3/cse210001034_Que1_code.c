#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define N (1 << 12)

int a[N][N];
int b[N];
long long ans_seq[N];
long long ans_par[N];

int main()
{
    srand(time(NULL));
    clock_t start_time, end_time;
    double seq_time, par_time;
#pragma omp parallel for
    for (int i = 0; i < N; i++)
#pragma omp parallel for
        for (int j = 0; j < N; j++)
        {
            a[i][j] = rand();
        }

#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        b[i] = rand();
    }

    // Sequential Code
    start_time = clock();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            ans_seq[i] += a[j][i] * b[j];
        }
    }

    end_time = clock();
    seq_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Serial Time :%lf\n", seq_time);

    // Parallel Code
    start_time = clock();
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        ans_par[i] = 0;
        for (int j = 0; j < N; j++)
        {
            ans_par[i] += a[j][i] * b[j];
        }
    }
    end_time = clock();
    par_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Parallel Time: %lf\n", par_time);
    printf("Speed up :- %lf \n \n", (seq_time / par_time));
    return 0;
}