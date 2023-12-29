#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define N 1000000
int a[N], b[N], seq[2 * N], par[2 * N];

int parallel_binary_search(int *arr, int n, int target)
{
    int p = 4;
    int check[p];
    int start = 0, end = n - 1;
    int ans = -1;
    while ((end - start + 1) > p)
    {
        printf("%d %d %d\n", start, end, target);
        int np = (end - start + 1) / p;
#pragma omp parallel for
        for (int i = 0; i < p; i++)
        {
            int s = i * np + start;
            int e = s + np - 1;
            if (arr[e] < target)
            {
                check[i] = 0;
            }
            else
            {
                check[i] = 1;
            }
        }
#pragma omp barrier
        if (check[0] == 1)
        {
            start = 0;
            end = np;
        }
        else
        {
#pragma omp parallel for
            for (int i = 1; i < p; i++)
            {
                if (check[i] == 1 && check[i - 1] == 0)
                {
                    start = i * np + start;
                    end = start + np;
                }
            }
        }
    }
#pragma omp parallel for
    for (int i = start; i <= end; i++)
    {
        if (arr[i] >= target)
            ans = i;
    }
    return ans;
};

void parallel_merge(int ia, int ja, int ib, int jb)
{
    int sizea = ja - ia + 1;
    int sizeb = jb - ib + 1;
    if (sizea == 1)
    {
        int pos1 = parallel_binary_search(b, sizeb, a[ia]);
        par[pos1 + ia] = a[ia];
#pragma omp parallel for
        for (int i = pos1; i >= ib; i--)
        {
            par[i + ia] = b[i];
        }
#pragma omp parallel for
        for (int i = pos1; i <= jb; i++)
        {
            par[i + ia] = b[i];
        }
        return;
    }
    if (sizeb == 1)
    {
        int pos2 = parallel_binary_search(a, sizea, b[ib]);
        par[pos2 + ib] = b[ib];
#pragma omp parallel for
        for (int i = pos2; i >= ia; i--)
        {
            par[i + ib] = b[i];
        }
#pragma omp parallel for
        for (int i = pos2; i <= ja; i++)
        {
            par[i + ib] = b[i];
        }
        return;
    }
    if (sizea <= sizeb)
    {
        int nsqrt = sqrt(sizea);
        int p = sizea / nsqrt;
#pragma omp parallel for
        for (int i = 0; i < p; i++)
        {
            int start = i * nsqrt + ia;
            int end = start + nsqrt - 1;
            int pos1 = parallel_binary_search(b, sizeb, a[start]);
            int pos2 = parallel_binary_search(b, sizeb, a[end]);
            par[start + pos1] = a[start];
            par[end + pos2] = a[end];
            parallel_merge(start, end, pos1, pos2);
        }
    }
    else
    {
        int nsqrt = sqrt(sizeb);
        int p = sizeb / nsqrt;
#pragma omp parallel for
        for (int i = 0; i < p; i++)
        {
            int start = i * nsqrt + ib;
            int end = start + nsqrt - 1;
            int pos1 = parallel_binary_search(a, sizea, b[start]);
            int pos2 = parallel_binary_search(a, sizea, b[end]);
            par[start + pos1] = b[start];
            par[end + pos2] = b[end];
            parallel_merge(start, end, pos1, pos2);
        }
    }
}

int main()
{

    clock_t start, end;
    omp_set_num_threads(4);
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = 2 * i;
    }

    // // Sequential Code
    // start = clock();
    // int i = 0, j = 0;
    // while (i >= 0 && j >= 0)
    // {
    //     if (a[i] < b[j])
    //     {
    //         seq[i + j] = a[i];
    //         i++;
    //     }
    //     else
    //     {
    //         seq[i + j] = b[j];
    //         j++;
    //     }
    // }
    // while (i >= 0)
    // {
    //     seq[i + j] = a[i];
    //     i++;
    // }
    // while (j >= 0)
    // {
    //     seq[i + j] = b[j];
    //     j++;
    // }
    // end = clock();

    // double seq_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Parallel Code
    start = clock();
    parallel_merge(0, N - 1, 0, N - 1);
    end = clock();

    return 0;
}