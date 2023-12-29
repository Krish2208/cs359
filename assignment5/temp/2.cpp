#include <bits/stdc++.h>
#include <omp.h>

#define N 1 << 12

using namespace std;

void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> Left(n1), Right(n2);

    for (int i = 0; i < n1; i++)
        Left[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        Right[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2)
    {
        if (Left[i] <= Right[j])
        {
            arr[k] = Left[i];
            i++;
        }
        else
        {
            arr[k] = Right[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = Left[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = Right[j];
        j++;
        k++;
    }
}

void seq_mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        seq_mergeSort(arr, l, m);
        seq_mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int par_search(int target, vector<int> &arr)
{
    int n = arr.size();
    int a1 = 0, a2 = n - 1;
    int x = sqrt(n);
#pragma omp parallel for
    for (int i = 0; i < x; i++)
    {
        if (target <= arr[(i + 1) * sqrt(n) - 1] && target >= arr[i * sqrt(n)])
        {
            a1 = i * sqrt(n);
            a2 = i * sqrt(n) + sqrt(n) - 1;
        }
    }

    int ans = -1;

#pragma omp parallel for
    for (int i = a1; i <= a2; i++)
    {
        if (arr[i] == target)
        {
            ans = i;
        }
        else if (i != 0)
        {
            if (arr[i - 1] < target && arr[i] > target)
            {
                ans = i;
            }
        }
    }

    if (ans == -1)
    {
        if (target > arr[a2])
        {
            ans = a2 + 1;
        }
        else if (target < arr[a1])
        {
            ans = a1;
        }
    }
    return ans;
}

void par_merge(vector<int> &x, int left, int middle, int right)
{
    int n = middle - left + 1;
    int m = right - middle;
    vector<int> A(n), B(m);

#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
        A[i] = x[left + i];
    }

#pragma omp parallel for
    for (int j = 0; j < m; ++j)
    {
        B[j] = x[middle + 1 + j];
    }
    vector<int> c(n + m);
#pragma omp parallel
    {
#pragma omp parallel for
        for (int i = 0; i < n; i++)
        {
            int r = par_search(A[i], B);
            c[i + r] = A[i];
        }

#pragma omp parallel for
        for (int i = 0; i < m; i++)
        {
            int r = par_search(B[i], A);
            c[i + r] = B[i];
        }
    }

#pragma omp parallel for
    for (int i = 0; i < n + m; i++)
    {
        x[left + i] = c[i];
    }
}

void par_mergeSort(vector<int> &array, int const begin, int const end)
{
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
#pragma omp parallel sections
    {
#pragma omp section
        par_mergeSort(array, begin, mid);

#pragma omp section
        par_mergeSort(array, mid + 1, end);
    }

    par_merge(array, begin, mid, end);
}

int main()
{
    vector<int> a;
    vector<int> b;
    for (int i = 0; i < N; i++)
    {
        a.push_back(-1 * i * 2);
        b.push_back(-1 * i * 2);
    }
    double t1, t2, t3, t4;
    t1 = omp_get_wtime();
    par_mergeSort(a, 0, N - 1);
    t2 = omp_get_wtime();
    t3 = omp_get_wtime();
    seq_mergeSort(b, 0, N - 1);
    t4 = omp_get_wtime();
    cout << "Parallel Time: " << t2 - t1 << endl;
    cout << "Sequential time: " << t4 - t3 << endl;

    return 0;
}