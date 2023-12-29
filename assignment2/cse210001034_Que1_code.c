#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N (1<<20)

long long a[N],b[N],c[N];

int main(){
	srand(time(NULL));
	clock_t start,end;
	for(int i = 0; i < N; i++) a[i] = b[i] = 1;
	start = clock();
		for(int i = 1; i < N; i++) a[i] += a[i-1];
	end = clock();
	printf("Serial time: %lf\n", (double) (end-start)/CLOCKS_PER_SEC);
	start = clock();
		for(int i = 1; i < N; i <<= 1){
			#pragma omp parallel for
				for(int j = 0; j < N; j++) c[j] = b[j];
			#pragma omp parallel for
				for(int j = i; j < N; j++) b[j] += c[j-i];

		}
	end = clock();
	printf("Parallel time: %lf\n", (double) (end-start)/(CLOCKS_PER_SEC));
	return 0;
}