#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int *find_upper_bound(int *start_ptr, int *end_ptr, int value, int num_processors)
{
    int size = end_ptr - start_ptr;

    if (*start_ptr > value)
        return start_ptr;
    omp_set_num_threads(num_processors);
    int arr[num_processors + 1];
    arr[0] = 0;
    while (size > num_processors)
    {
#pragma omp parallel for
        for (int i = 1; i <= num_processors; i++)
        {
            long long ind = size;
            ind *= i;
            ind /= (num_processors + 1);
            int *ptr = start_ptr + ind;
            if (*ptr <= value)
                arr[i] = 0;
            else
                arr[i] = 1;
        }

#pragma omp barrier

        int index = num_processors + 1;

#pragma omp parallel for
        for (int i = 1; i <= num_processors; i++)
        {
            if (arr[i] == 1 && arr[i - 1] == 0)
            {
                index = i;
            }
        }
        long long ind = size;
        ind *= index - 1;
        ind /= (num_processors + 1);
        long long ind2 = size;
        ind2 *= index;
        ind2 /= (num_processors + 1);
        end_ptr = start_ptr + ind2;
        start_ptr = start_ptr + ind;
        size = end_ptr - start_ptr;
    }
    int *ans = end_ptr;
#pragma omp parallel for
    for (int i = 1; i < size; i++)
    {
        if (*(start_ptr + i) > value && *(start_ptr + i - 1) <= value)
            ans = (start_ptr + i);
    }
    return ans;
}

void merge_sort(int *start_ptr, int *end_ptr)
{
    int size = end_ptr - start_ptr;
    if (size == 1)
        return;
    int *mid = start_ptr + (size / 2);
    merge_sort(start_ptr, mid);
    merge_sort(mid, end_ptr);
    int merged[size];
    merge_arrays(start_ptr, mid, mid, end_ptr, merged);
#pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        *(start_ptr + i) = merged[i];
    }
#pragma omp barrier
}

void merge_arrays(int *left_arr_start, int *left_arr_end, int *right_arr_start, int *right_arr_end, int *merged_arr_start)
{
    int left_arr_size, right_arr_size;
    left_arr_size = left_arr_end - left_arr_start;
    right_arr_size = right_arr_end - right_arr_start;
    if (right_arr_start == right_arr_end)
    {
#pragma omp parallel for
        for (int i = 0; i < left_arr_size; i++)
        {
            *(merged_arr_start + i) = *(left_arr_start + i);
        }
#pragma omp barrier
        return;
    }
    if (left_arr_size < right_arr_size)
    {
        int *temp = right_arr_start;
        right_arr_start = left_arr_start;
        left_arr_start = temp;
        temp = right_arr_end;
        right_arr_end = left_arr_end;
        left_arr_end = temp;
        left_arr_size = left_arr_end - left_arr_start;
        right_arr_size = right_arr_end - right_arr_start;
    }
    if (right_arr_start == right_arr_end)
    {
#pragma omp parallel for
        for (int i = 0; i < left_arr_size; i++)
        {
            *(merged_arr_start + i) = *(left_arr_start + i);
        }
#pragma omp barrier
        return;
    }
    if (left_arr_size == 1)
    {
        if (*left_arr_start <= *right_arr_start)
        {
            *merged_arr_start = *left_arr_start;
            *(merged_arr_start + 1) = *right_arr_start;
        }
        else
        {
            *merged_arr_start = *right_arr_start;
            *(merged_arr_start + 1) = *left_arr_start;
        }
        return;
    }
    int div = sqrt(left_arr_size);
    int blocks = (left_arr_size + div - 1) / div;
    int *ptrs[blocks + 1];
    ptrs[0] = right_arr_start;
    ptrs[blocks] = right_arr_end;
    // #pragma omp parallel for
    for (int i = 1; i < blocks; i++)
    {
        ptrs[i] = find_upper_bound(right_arr_start, right_arr_end, *(left_arr_start + (div * i)), div);
    }
    // #pragma omp barrier

    // #pragma omp parallel for
    for (int i = 0; i < blocks; i++)
    {
        int *rt;
        if (i != (blocks - 1))
            rt = left_arr_start + (i + 1) * div;
        else
            rt = left_arr_end;

        merge_arrays(left_arr_start + i * div, rt, ptrs[i], ptrs[i + 1], merged_arr_start + i * div + (ptrs[i] - right_arr_start));
    }
}

void seq_merge_sort(int *l, int *r){
    int size = r-l;
    if(size == 1) return;
    int *mid = l+(size/2);
    seq_merge_sort(l,mid);
    seq_merge_sort(mid,r);
    int c[size];
    int *ptrs[2];
    ptrs[0] = l;
    ptrs[1] = mid;
    int *ptr = c;
    while(ptrs[0] != mid && ptrs[1] != r){
        if(*(ptrs[0]) <= *(ptrs[1])){
            *(ptr++) = *(ptrs[0]++);
        }
        else{
            *(ptr++) = *(ptrs[1]++);
        }
    }
    while(ptrs[0] != mid){
        *(ptr++) = *(ptrs[0]++);
    }
    while(ptrs[1] != r){
        *(ptr++) = *(ptrs[1]++);
    }
    for(int i = 0; i < size; i++)
    {
        *(l+i) = c[i];
    }
}

int main()
{
    clock_t start, end;
    int n = 1024;
    srand(time(NULL));
    int a[n], b[n];
    for (int i = 0; i < n; i++)
        a[i] = b[i] = rand();
    start = clock();
    merge_sort(a, a + n);
    end = clock();
    double par_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Parallel: %f\n", par_time);

    start = clock();
    seq_merge_sort(b, b + n);
    end = clock();
    double seq_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sequential: %f\n", seq_time);

    printf("Speedup: %f\n", seq_time / par_time);
    for (int i = 0; i < 10; i++)
        printf("%d ", a[i]);
    printf("\n");
    for (int i = 0; i < 10; i++)
        printf("%d ", b[i]);
    printf("\n");
    return 0;
}