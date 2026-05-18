/*
Write a program to implement Parallel Bubble Sort and Merge sort using OpenMP.
Use existing algorithms and measure the performance of sequential and parallel algorithms.
*/
#include <iostream>
#include <vector>
#include <omp.h>
#include <iomanip>
using namespace std;

// 🔹 Utility: Print Array
void printArray(vector<int> &arr)
{
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

// ======================================================
// SEQUENTIAL BUBBLE SORT
// ======================================================
void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ======================================================
// PARALLEL BUBBLE SORT (Odd-Even Sort)
// ======================================================
void parallelBubbleSort(vector<int> &arr)
{
    int n = arr.size();

    for (int i = 0; i < n; i++)
    {

// Even phase
#pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }

// Odd phase
#pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ======================================================
// MERGE FUNCTION
// ======================================================
void merge(vector<int> &arr, int left, int mid, int right)
{
    vector<int> temp(right - left + 1); // temporary array that stores merged elements
    int i = left, j = mid + 1, k = 0;   // k is Index for temp array

    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];

    for (int i = left; i <= right; i++)
        arr[i] = temp[i - left];
}

// ======================================================
// SEQUENTIAL MERGE SORT
// ======================================================
void mergeSort(vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;

    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// ======================================================
// PARALLEL MERGE SORT (OpenMP Tasks)
// ======================================================
void parallelMergeSort(vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;

    int mid = (left + right) / 2;

#pragma omp task shared(arr)
    parallelMergeSort(arr, left, mid);

#pragma omp task shared(arr) // variables should be shared among all threads in a parallel team
    parallelMergeSort(arr, mid + 1, right);

#pragma omp taskwait
    merge(arr, left, mid, right);
}

// Wrapper
void runParallelMergeSort(vector<int> &arr)
{
#pragma omp parallel
    {
#pragma omp single
        {
            parallelMergeSort(arr, 0, arr.size() - 1);
        }
    }
}

// ======================================================
// 🔹 MAIN FUNCTION
// ======================================================
int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // Copy arrays
    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double start, end;

    cout << fixed << setprecision(6);
    // 🔸 Sequential Bubble
    start = omp_get_wtime();
    bubbleSort(arr1);
    end = omp_get_wtime();
    cout << left << setw(35) << "\nSequential Bubble Sort Time: " << (end - start);

    // 🔸 Parallel Bubble
    start = omp_get_wtime();
    parallelBubbleSort(arr2);
    end = omp_get_wtime();
    cout << left << setw(35) << "\nParallel Bubble Sort Time: " << (end - start);

    // 🔸 Sequential Merge
    start = omp_get_wtime();
    mergeSort(arr3, 0, n - 1);
    end = omp_get_wtime();
    cout << left << setw(35) << "\nSequential Merge Sort Time: " << (end - start);

    // 🔸 Parallel Merge
    start = omp_get_wtime();
    runParallelMergeSort(arr4);
    end = omp_get_wtime();
    cout << left << setw(35) << "\nParallel Merge Sort Time: " << (end - start);

    // Output sorted array
    cout << "\n\nSorted Array (Merge Sort): ";
    printArray(arr4);

    return 0;
}