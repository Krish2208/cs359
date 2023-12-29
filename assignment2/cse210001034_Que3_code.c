#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>

#define N (1<<6)

long long a[N],b[N];

void swap(long long *a, long long *b){
	long long c = *a;
	*a = *b;
	*b = c;
}

void sequential_maximums(long long *a)
{
    long long ans = 0, ans1 = 0;
    for (int j = 0; j < N; j++)
    {
        if (a[j] >= ans)
        {
            ans1 = ans;
            ans = a[j];
        }
        else if (a[j] > ans1)
        {
            ans1 = a[j];
        }
    }
    printf("%lld %lld\n", ans, ans1);
}

void parallel_maximums(long long *arr){
	#pragma omp parallel for
		for(int i = 0; i < (N>>1); i++) if(arr[(i<<1)] < arr[(i<<1)+1]) swap(&arr[(i<<1)],&arr[(i<<1)+1]);
	#pragma omp barrier

	for(int i = N>>1; i > 1; i >>= 1){
		#pragma omp parallel for
			for(int j = 0; j < i; j += 2){
				bool cmp1 = (arr[j+1] > arr[j+i]);
				bool cmp2 = (arr[j+i+1] > arr[j]);
				if(cmp2){
					swap(&arr[j],&arr[j+i]);
					swap(&arr[j+1],&arr[j+i+1]);
				}
				else if(! cmp1){
					swap(&arr[j+1],&arr[j+i]);
					if(arr[j+1] > arr[j]) swap(&arr[j+1],&arr[j]);
				}
			}
	}

	printf("%lld %lld \n", arr[0], arr[1]);
}

int main(){
	srand(time(NULL));

	clock_t start,end;
	for(int i = 0; i < N; i++) a[i] = b[i] = rand();
	start = clock();
		sequential_maximums(a);
	end = clock();
	printf("Serial time: %lf\n", (double) (end-start)/CLOCKS_PER_SEC);
	start = clock();
		parallel_maximums(b);
	end = clock();
	printf("Parallel time: %lf\n", (double) (end-start)/(CLOCKS_PER_SEC));
	return 0;
}