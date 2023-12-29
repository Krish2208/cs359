#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include <time.h>
#define N 1048576 //2^20
long long a[N];
int main(){
    clock_t start, end;
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        a[i] = rand();
    }

    // Serial
    start = clock();
    long long sum = 0;
    for(int i=0; i<N; i++){
        sum += a[i];
    }
    printf("Serial Sum: %lld\n", sum);
    end = clock();
    double serial_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Parallel
    start = clock();
    for(int i=N/2; i>0; i/=2){
        #pragma omp parallel for
        for(int j=0; j<i; j++){
            a[j] += a[j+i];
        }
    }
    printf("Parallel Sum: %lld\n", a[0]);
    end = clock();
    double parallel_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Serial time: %f\n", serial_time);
    printf("Parallel time: %f\n", parallel_time);
    printf("Speedup: %f\n", serial_time/parallel_time);
    return 0;
}