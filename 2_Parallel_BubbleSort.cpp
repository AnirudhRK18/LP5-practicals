#include <iostream>
#include <omp.h>

using namespace std;

void bubble(int array[], int n) {
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
        }
    }
}

void pBubble(int array[], int n) {
    for (int i = 0; i < n; i++) {
        #pragma omp parallel
        {
            // Odd indexed pass
            #pragma omp for
            for (int j = 1; j < n - 1; j += 2) {
                if (array[j] > array[j + 1])
                    swap(array[j], array[j + 1]);
            }

            // Synchronize threads
            #pragma omp barrier

            // Even indexed pass
            #pragma omp for
            for (int j = 0; j < n - 1; j += 2) {
                if (array[j] > array[j + 1])
                    swap(array[j], array[j + 1]);
            }
        }
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    int arr[n], temp[n];
    cout << "Enter the elements of the array: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        temp[i] = arr[i];  // Store for reuse
    }

    double start_time, end_time;

    // Sequential Bubble Sort
    start_time = omp_get_wtime();
    bubble(arr, n);
    end_time = omp_get_wtime();
    cout << "\nSequential Bubble Sort took: " << end_time - start_time << " seconds.\n";
    printArray(arr, n);

    // Parallel Bubble Sort
    for (int i = 0; i < n; i++) arr[i] = temp[i];  // Restore original data

    start_time = omp_get_wtime();
    pBubble(arr, n);
    end_time = omp_get_wtime();
    cout << "\nParallel Bubble Sort took: " << end_time - start_time << " seconds.\n";
    printArray(arr, n);

    return 0;
}
