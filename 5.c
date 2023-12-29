#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    const int n = 100000;
    int i, a[n];
    for (i = 0; i < n; i++) a[i] = i;
    return 0;
}