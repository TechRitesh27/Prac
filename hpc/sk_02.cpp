#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace chrono;

void print(vector<int> a) {
    for (int x : a) cout << x << " ";
    cout << endl;
}

void bubbleSort(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

void parallelBubbleSort(vector<int>& a) {
    int n = a.size();
    for (int phase = 0; phase < n; phase++) {
        #pragma omp parallel for
        for (int i = phase % 2; i < n - 1; i += 2)
            if (a[i] > a[i + 1])
                swap(a[i], a[i + 1]);
    }
}

void merge(vector<int>& a, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r)
        temp.push_back(a[i] < a[j] ? a[i++] : a[j++]);

    while (i <= m) temp.push_back(a[i++]);
    while (j <= r) temp.push_back(a[j++]);

    for (int k = l; k <= r; k++)
        a[k] = temp[k - l];
}

void mergeSort(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;
    mergeSort(a, l, m);
    mergeSort(a, m + 1, r);
    merge(a, l, m, r);
}

void parallelMergeSort(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        mergeSort(a, l, m);

        #pragma omp section
        mergeSort(a, m + 1, r);
    }

    merge(a, l, m, r);
}

double getTime() {
    static auto start = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    duration<double> diff = now - start;
    return diff.count();
}

int main() {
    vector<int> a = {64, 34, 25, 12, 22, 11, 90, 45, 7, 3};

    vector<int> b1 = a, b2 = a, m1 = a, m2 = a;
    double start, end;

    cout << "Original Array: ";
    print(a);

    start = getTime();
    bubbleSort(b1);
    end = getTime();
    cout << "\nSequential Bubble Sort: ";
    print(b1);
    cout << "Time: " << end - start << " sec\n";

    start = getTime();
    parallelBubbleSort(b2);
    end = getTime();
    cout << "\nParallel Bubble Sort: ";
    print(b2);
    cout << "Time: " << end - start << " sec\n";

    start = getTime();
    mergeSort(m1, 0, m1.size() - 1);
    end = getTime();
    cout << "\nSequential Merge Sort: ";
    print(m1);
    cout << "Time: " << end - start << " sec\n";

    start = getTime();
    parallelMergeSort(m2, 0, m2.size() - 1);
    end = getTime();
    cout << "\nParallel Merge Sort: ";
    print(m2);
    cout << "Time: " << end - start << " sec\n";

    return 0;
}