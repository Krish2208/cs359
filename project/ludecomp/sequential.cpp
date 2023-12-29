#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>

using namespace std;

void title()
{
    cout << "LU Decomposition" << endl;
    cout << "Sequential" << endl;
}

void lu_decomposition(float **a, float **l, float **u, int size)
{
    // Sequential LU Decomposition
    for(int i = 0; i<size; i++){
        for(int j = i; j<size; j++){
            int sum = 0;
            for(int k = 0; k<i; k++){
                sum += l[i][k] * u[k][j];
            }
            u[i][j] = a[i][j] - sum;
        }
        for(int j = i; j<size; j++){
            if(j==i) l[i][j] = 1;
            else{
                int sum = 0;
                for(int k = 0; k<i; k++){
                    sum += l[j][k] * u[k][i];
                }
                l[j][i] = (a[j][i] - sum) / u[i][i];
            }
        }
    }
}

void initialize_matrices(float **a, float **l, float **u, int size)
{
    for (int i = 0; i < size; i++)
    {
        a[i] = new float[size];
        l[i] = new float[size];
        u[i] = new float[size];
    }
}

int main()
{
    title();
    srand(time(NULL));
    cout << "Size\tRuntime" << endl;
    for (int size = 2; size <= 4096; size *= 2)
    {
        double runtime;
        float **a = new float *[size];
        float **l = new float *[size];
        float **u = new float *[size];
        initialize_matrices(a, l, u, size);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                a[i][j] = (float)((rand() % 10) + 1);
            }
        }
        clock_t start = clock();
        lu_decomposition(a, l, u, size);
        clock_t end = clock();
        runtime = (double)(end - start) / CLOCKS_PER_SEC;
        cout << size << '\t' << runtime << endl;
    }
    return 0;
}