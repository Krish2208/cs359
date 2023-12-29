#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])

{

    int th_id, nthreads;

#pragma omp parallel private(th_id) shared(nthreads)

    {

        th_id = omp_get_thread_num();

#pragma omp critical

        {
            printf("Hello World from thread %d\n", th_id);
        }

#pragma omp barrier

#pragma omp master

        {

            nthreads = omp_get_num_threads();

            printf("There are %d threads\n", nthreads);
        }
    }

    return 0;
}