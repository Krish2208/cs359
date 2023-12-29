#include <stdio.h>
#include <omp.h>

int main(void)
{
    int nTh, i;
    nTh = omp_get_num_threads(); /*nTh=1*/
    printf("\nNumber of threads working at this point: %d\n", nTh);
    /*omp_set_num_threads(4);*/
    nTh = omp_get_num_threads(); /*nTh=1*/
    printf("\nNumber of threads working: r%d\n", nTh);
    printf("\nMax Threads = %d\n", omp_get_max_threads());
    printf("\nNum processors = %d\n", omp_get_num_procs());
    #pragma omp parallel for
    for (i = 0; i <= 3; i++) printf("Hello, world : from thread ID: %d.\n", omp_get_thread_num());
    return 0;
}