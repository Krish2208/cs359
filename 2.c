#include <stdio.h>
#include <omp.h>

int main(void)
{
    omp_set_num_threads(5); /*to set number of threads*/
    #pragma omp parallel
    printf("Hello, world : from thread ID: %d.\n", omp_get_thread_num());
    return 0;
}
