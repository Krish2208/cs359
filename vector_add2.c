#include <stdio.h>
// time for execution
#include <time.h>
const int n = 10000000;
int a[n], b[n], c[n];
int main()
{
    int i;
    for (i = 0; i < n; i++)
    {
        a[i] = i;
        b[i] = i;
    }
    clock_t start, end;
    start = clock();
    for (i = 0; i < n; i++)
    {
        c[i] = a[i] + b[i];
    }
    end = clock();
    printf("Time taken: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
}