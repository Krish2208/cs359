#define CHUNKSIZE 1 /*defines the chunk size as 1 contiguous iteration*/

/*forks off the threads*/

int main(){
    #pragma omp parallel private(j, k)
    {
        /*Starts the work sharing construct*/

    #pragma omp for schedule(dynamic, CHUNKSIZE)

        for (i = 2; i <= N - 1; i++) /*i is private by default*/

            for (j = 2; j <= i; j++)

                for (k = 1; k <= M; k++)

                    b[i][j] += a[i - 1][j] / k + a[i + 1][j] / k;
    }
}