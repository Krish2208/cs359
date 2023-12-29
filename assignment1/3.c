#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include <time.h>
#define N 1048576 //2^20
int a[N];
int main(){
    clock_t start, end;
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        a[i] = rand();
    }

    // Serial
    start = clock();
    int maximum = a[0];
    for(int i=0; i<N; i++){
        maximum = maximum > a[i] ? maximum : a[i];
    }
    printf("Serial Maximum: %d\n", maximum);
    end = clock();
    double serial_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Parallel
    start = clock();
    for(int i=N/2; i>0; i/=2){
        #pragma omp parallel for
        for(int j=0; j<i; j++){
            a[j] = a[j] > a[j+i] ? a[j] : a[j+i];
        }
    }
    printf("Parallel Maximum: %d\n", a[0]);
    end = clock();
    double parallel_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Serial time: %f\n", serial_time);
    printf("Parallel time: %f\n", parallel_time);
    printf("Speedup: %f\n", serial_time/parallel_time);
    return 0;
}