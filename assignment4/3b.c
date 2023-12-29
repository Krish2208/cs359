#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

double f(double x)
{
    return 4 * sqrt(1.0 - (x * x));
}

int main()
{
    clock_t start_time, end_time;
    double seq_time, par_time;
    int num_intervals = 1e9;
    double interval_width = 1.0 / num_intervals;
    double sum_par = 0.0;
    double sum_seq = 0.0;
    
    start_time = clock();
    for (int i = 0; i < num_intervals; i++)
    {
        double x1 = i * interval_width;
        double y1 = f(x1);
        double area = y1 * interval_width;
        sum_seq += area;
    }
    end_time = clock();
    seq_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    start_time = clock();
#pragma omp parallel for reduction(+ : sum_par)
    for (int i = 0; i < num_intervals; i++)
    {
        double x1 = i * interval_width;
        double y1 = f(x1);
        double area = y1 * interval_width;
        sum_par += area;
    }
    end_time = clock();
    par_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;


    printf("pi (sequential): %0.7lf\n", sum_seq);
    printf("Serial Time: %lf\n", seq_time);
    printf("pi (parallel): %0.7lf\n", sum_par);
    printf("Parallel Time: %lf\n", par_time);
    printf("\nSpeed up: %lf\n", (seq_time / par_time));
    return 0;
}