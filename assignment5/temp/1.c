#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define N 8

int* par_binary_search(int *l, int *r, int target, int proc){
    printf("%d %d\n", *l, *r);
    int n = r-l;
    int ans;
    if(n<=proc){
        #pragma omp parallel for
        for(int i = 0; i<n; i++){
            if(*(l+i)>=target && *(l+i-1)<target) ans = (l+i);
        }
        #pragma omp barrier
        return ans;
    }
    int blocks_size = n/proc;
    int check[proc+2];
    check[0] = 0;
    check[proc+1] = 1;
    #pragma omp parallel for
    for(int i = 1; i<=proc; i++){
        int x = *(l+((i)*blocks_size)-1);
        printf("x: %d\n", x);
        if(x<target) check[i] = 0;
        else check[i] = 1;
    }

    for(int i = 0; i<proc+2; i++) printf("%d ", check[i]);
    printf("\n");

    #pragma omp barrier

    int ind;
    #pragma omp parallel for
    for(int i = 0; i<proc+2; i++){
        if(check[i]==0 && check[i+1]==1) ind = i;
    }
    return par_binary_search(l+(ind*blocks_size), l+((ind+1)*blocks_size)-1, target, proc);
}

int main(){
    int arr[] = {1, 3, 8, 13, 15, 16, 17, 18};
    printf("%d\n", *par_binary_search(arr, arr+8, 6, 4));
    return 0;
}