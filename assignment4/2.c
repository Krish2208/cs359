#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define N 100000
char A[N];
char serial_ans[N];
char par_ans[N];

int main()
{
    clock_t start, end;
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        int x = rand();
        if (x % 52 < 26)
        {
            A[i] = 'a' + x % 26;
        }
        else
        {
            A[i] = 'A' + x % 26;
        }
    }

    // Sequential Code
    start = clock();
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (A[i] <= 'Z' && A[i] >= 'A')
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
    int take[N];
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (A[i] <= 'Z' && A[i] >= 'A')
        {
            take[i] = 1;
        }
        else
        {
            take[i] = 0;
            A[i] = -1;
        }
    }
    int c[N];
    for (int h = 1; h < N; h *= 2)
    {
#pragma omp parallel for
        for (int i = 0; i < N; i++)
        {
            c[i] = take[i];
        }
#pragma omp parallel for
        for (int i = 0; i < N - h; i++)
        {
            take[i + h] += c[i];
        }
    }
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (A[i] != -1)
        {
            par_ans[take[i] - 1] = A[i];
        }
    }
    end = clock();
    double par_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Parallel Time: %lf\n", par_time);
    printf("\nSpeed up: %lf\n", (seq_time / par_time));
    return 0;
}