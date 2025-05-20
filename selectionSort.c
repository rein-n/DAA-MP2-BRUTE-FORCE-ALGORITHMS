#include <stdio.h>

// Function to print all elements of the array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Original Selection Sort
void originalSelectionSort(int arr[], int size, int descending) {
    int passes = 0;                 // To count how many passes the sort makes
    printf("\nOriginal Selection Sort:\n");

    // Loop through each element in the array (except the last one)
    for (int i = 0; i < size - 1; i++) {
        int selected = i;           // Assume the current position has the correct value

        // Find the minimum or maximum element in the rest of the array
        for (int j = i + 1; j < size; j++) {
            // Choose the correct comparison depending on the sort order
            if ((descending && arr[j] > arr[selected]) || (!descending && arr[j] < arr[selected])) {
                selected = j;
            }
        }

        // Swap the current element with the selected one if they are different
        if (selected != i) {
            int temp = arr[i];
            arr[i] = arr[selected];
            arr[selected] = temp;
        }

        passes++;                   // Increase pass count
        printf("Pass %d: ", passes);
        printArray(arr, size);      // Show the array after each pass
    }

    // Print total passes made by the sort
    printf("Total passes (Original): %d\n", passes);
}

// Optimized Selection Sort (does multiple selections per pass)
void optimizedSelectionSort(int arr[], int size, int descending) {
    int passes = 0;
    int step = 2;                   // Number of elements handled per pass

    printf("\nOptimized Selection Sort:\n");

    // Go through the array in chunks of 'step' size
    for (int i = 0; i < size - 1; i += step) {
        // Loop through up to 'step' elements in this block
        for (int m = 0; m < step && (i + m < size - 1); m++) {
            int selected = i + m;  // Start by assuming current index is correct

            // Find the correct value to swap with (min or max depending on order)
            for (int j = selected + 1; j < size; j++) {
                if ((descending && arr[j] > arr[selected]) || (!descending && arr[j] < arr[selected])) {
                    selected = j;
                }
            }

            // Swap if needed
            if (selected != i + m) {
                int temp = arr[i + m];
                arr[i + m] = arr[selected];
                arr[selected] = temp;
            }
        }

        passes++;                   // Increase pass count
        printf("Pass %d: ", passes);
        printArray(arr, size);      // Show the array after each pass
    }

    // Print total passes made by the optimized sort
    printf("Total passes (Optimized): %d\n", passes);
}

int main() {
    int size, order;

    // Ask the user how many numbers to sort
    printf("Enter number of elements: ");
    scanf("%d", &size);

    int arr1[size], arr2[size];

    // Ask the user to input the numbers
    printf("Enter %d integers: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr1[i]);      // Store value in first array
        arr2[i] = arr1[i];          // Make a copy for the second sort
    }

    // Ask the user whether to sort in ascending or descending order
    printf("Choose order:\n");
    printf("1. Ascending\n");
    printf("2. Descending\n");
    printf("Enter choice: ");
    scanf("%d", &order);

    int descending = (order == 2);  // Use 1 for descending, 0 for ascending

    // Run the original selection sort
    originalSelectionSort(arr1, size, descending);
    printf("Final sorted array (Original): ");
    printArray(arr1, size);

    printf("\n-----------------------------\n");

    // Run the optimized selection sort
    optimizedSelectionSort(arr2, size, descending);
    printf("Final sorted array (Optimized): ");
    printArray(arr2, size);

    return 0;
}
