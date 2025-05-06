#include <iostream>
#include <omp.h>

using namespace std;

int minval(int arr[], int n) {
    int minval = arr[0];
    #pragma omp parallel for reduction(min : minval)
    for (int i = 0; i < n; i++) {
        if (arr[i] < minval)
            minval = arr[i];
    }
    return minval;
}

int maxval(int arr[], int n) {
    int maxval = arr[0];
    #pragma omp parallel for reduction(max : maxval)
    for (int i = 0; i < n; i++) {
        if (arr[i] > maxval)
            maxval = arr[i];
    }
    return maxval;
}

int sum(int arr[], int n) {
    int sum = 0;
    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

double average(int arr[], int n) {
    return (double)sum(arr, n) / n;
}

int main() {
    cout << "Enter number of elements: ";
    int n;
    cin >> n;
    int arr[n];
    cout << "Enter elements of array: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "The minimum value is: " << minval(arr, n) << '\n';
    cout << "The maximum value is: " << maxval(arr, n) << '\n';
    cout << "The summation is: " << sum(arr, n) << '\n';
    cout << "The average is: " << average(arr, n) << '\n';
    
    return 0;
}
