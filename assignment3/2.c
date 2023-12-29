#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>

#define N 1024

int mat[N][N];
int ans_seq[N][N], ans_par[N][N];

int main()
{
	clock_t start_time, end_time;
	double seq_time, par_time;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			mat[i][j] = rand();

	// Sequential Code
	for (int block_size = 1; block_size <= N; block_size <<= 1)
	{
		printf("Block Size: %d \n", block_size);
		start_time = clock();
		for (int j = 0; j < N; j++)
		{
			for (int i = 0; i < N; i++)
			{
				ans_seq[i][j] = mat[j][i];
			}
		}
		end_time = clock();
		seq_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

		// Parallel Code
		printf("Serial Time: %lf\n", seq_time);
		start_time = clock();
#pragma omp parallel for
		for (int i = 0; i < N; i += block_size)
		{
#pragma omp parallel for
			for (int j = 0; j < N; j += block_size)
			{
				for (int k = 0; k + i < N && k < block_size; k++)
				{
					for (int l = 0; l + j < N && l < block_size; l++)
					{
						ans_par[k + i][l + j] = mat[l + j][k + i];
					}
				}
			}
		}
		end_time = clock();
		par_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		printf("Parallel Time: %lf\n", par_time);
		printf("Speed up :- %lf \n \n", (seq_time / par_time));
	}
	return 0;
}
