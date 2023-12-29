#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include <time.h>
#define N 1048576 //2^20
int a[N], b[N], c[N], d[N];
int main(){
    clock_t start, end;
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        a[i] = rand();
        b[i] = rand();
    }
    
    // Serial
    start = clock();
    for(int i=0; i<N; i++){
        d[i] = a[i] + b[i];
    }
    end = clock();
    double serial_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Parallel
    start = clock();
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        c[i] = a[i] + b[i];
    }
    end = clock();
    double parallel_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Serial time: %f\n", serial_time);
    printf("Parallel time: %f\n", parallel_time);
    printf("Speedup: %f\n", serial_time/parallel_time);
    return 0;
}