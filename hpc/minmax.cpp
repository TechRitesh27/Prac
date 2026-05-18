
/* Implement Min, Max, Sum and Average operations using Parallel Reduction.

Without reduction, this would cause:

❌ Race conditions
❌ Incorrect results
#include <climits>

int min_val = INT_MAX;
int max_val = INT_MIN;
Because multiple threads would try to update the same variable simultaneously.
*/

#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    double s, e;

    // ---------------- MIN ----------------
    int min_seq = arr[0];
    s = omp_get_wtime();
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < min_seq)
            min_seq = arr[i];
    }
    e = omp_get_wtime();
    double t_min_seq = e - s;

    int min_par = arr[0];
    s = omp_get_wtime();
#pragma omp parallel for reduction(min : min_par)
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < min_par)
            min_par = arr[i];
    }
    e = omp_get_wtime();
    double t_min_par = e - s;

    // ---------------- MAX ----------------
    int max_seq = arr[0];
    s = omp_get_wtime();
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max_seq)
            max_seq = arr[i];
    }
    e = omp_get_wtime();
    double t_max_seq = e - s;

    int max_par = arr[0];
    s = omp_get_wtime();
#pragma omp parallel for reduction(max : max_par)
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max_par)
            max_par = arr[i];
    }
    e = omp_get_wtime();
    double t_max_par = e - s;

    // ---------------- SUM ----------------
    int sum_seq = 0;
    s = omp_get_wtime();
    for (int i = 0; i < n; i++)
    {
        sum_seq += arr[i];
    }
    e = omp_get_wtime();
    double t_sum_seq = e - s;

    int sum_par = 0;
    s = omp_get_wtime();
#pragma omp parallel for reduction(+ : sum_par)
    for (int i = 0; i < n; i++)
    {
        sum_par += arr[i];
    }
    e = omp_get_wtime();
    double t_sum_par = e - s;

    // ---------------- AVG ----------------
    double avg_seq, avg_par;

    s = omp_get_wtime();
    int temp_sum1 = 0;
    for (int i = 0; i < n; i++)
    {
        temp_sum1 += arr[i];
    }
    avg_seq = (double)temp_sum1 / n;
    e = omp_get_wtime();
    double t_avg_seq = e - s;

    s = omp_get_wtime();
    int temp_sum2 = 0;
#pragma omp parallel for reduction(+ : temp_sum2)
    for (int i = 0; i < n; i++)
    {
        temp_sum2 += arr[i];
    }
    avg_par = (double)temp_sum2 / n;
    e = omp_get_wtime();
    double t_avg_par = e - s;

    // ---------------- OUTPUT ----------------
    cout << "\n----- MIN -----\n";
    cout << "Sequential Min: " << min_seq << " Time: " << t_min_seq << endl;
    cout << "Parallel   Min: " << min_par << " Time: " << t_min_par << endl;

    cout << "\n----- MAX -----\n";
    cout << "Sequential Max: " << max_seq << " Time: " << t_max_seq << endl;
    cout << "Parallel   Max: " << max_par << " Time: " << t_max_par << endl;

    cout << "\n----- SUM -----\n";
    cout << "Sequential Sum: " << sum_seq << " Time: " << t_sum_seq << endl;
    cout << "Parallel   Sum: " << sum_par << " Time: " << t_sum_par << endl;

    cout << "\n----- AVG -----\n";
    cout << "Sequential Avg: " << avg_seq << " Time: " << t_avg_seq << endl;
    cout << "Parallel   Avg: " << avg_par << " Time: " << t_avg_par << endl;

    return 0;
}

/*
Enter n: 5
Enter elements:
4
98
5
64
2

----- MIN -----
Sequential Min: 2 Time: 0
Parallel   Min: 2 Time: 0.00300002

----- MAX -----
Sequential Max: 98 Time: 0
Parallel   Max: 98 Time: 0

----- SUM -----
Sequential Sum: 173 Time: 0
Parallel   Sum: 173 Time: 0

----- AVG -----
Sequential Avg: 34.6 Time: 0
Parallel   Avg: 34.6 Time: 0
*/