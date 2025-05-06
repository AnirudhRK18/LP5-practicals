#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

void merge(vector<int>& arr, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    vector<int> left(n1);
    vector<int> right(n2);

    // Copy all left elements
    for (int i = 0; i < n1; i++) left[i] = arr[low + i];

    // Copy all right elements
    for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];

    // Compare and place elements
    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // If any elements are left out
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void parallelMergeSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, low, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, high);
            }
        }
        merge(arr, low, mid, high);
    }
}

void mergeSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

// Function to print the array
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    // Using vector for dynamic array size
    vector<int> arr(n);
    double start_time, end_time;

    // Enter the elements of the array
    cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    // Measure Sequential Time
    start_time = omp_get_wtime();
    mergeSort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Time taken by sequential algorithm: " << end_time - start_time << " seconds\n";
    cout << "Sorted array (Sequential): ";
    printArray(arr);

    // Reset the array
    cout << "Enter the elements of the array again: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    // Measure Parallel time
    start_time = omp_get_wtime();
    parallelMergeSort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Time taken by parallel algorithm: " << end_time - start_time << " seconds\n";
    cout << "Sorted array (Parallel): ";
    printArray(arr);

    return 0;
}
