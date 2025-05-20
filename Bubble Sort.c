#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHUNK_SIZE 5

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void copy_array(int *dest, const int *src, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

bool is_sorted(int *arr, int start, int end, int ascending) {
    for (int i = start; i < end - 1; i++) {
        if ((ascending && arr[i] > arr[i + 1]) || (!ascending && arr[i] < arr[i + 1]))
            return false;
    }
    return true;
}

void swap_if_needed(int *a, int *b, bool ascending, bool *swapped) {
    if ((ascending && *a > *b) || (!ascending && *a < *b)) {
        int temp = *a;
        *a = *b;
        *b = temp;
        *swapped = true;
    }
}

void optimized_bubble_sort(int *arr, int n, int *passes, int ascending) {
    printf("\n--- Optimized Bubble Sort Iterations ---\n");
    bool swapped;
    do {
        (*passes)++;
        swapped = false;
        printf("Pass %d: ", *passes);

        for (int chunk = 0; chunk < n; chunk += CHUNK_SIZE) {
            int end = (chunk + CHUNK_SIZE > n) ? n : chunk + CHUNK_SIZE;
            if (is_sorted(arr, chunk, end, ascending)) continue;

            for (int i = chunk; i < end - 1; i++)
                swap_if_needed(&arr[i], &arr[i + 1], ascending, &swapped);
        }

        for (int i = 0; i < n - 1; i++)
            swap_if_needed(&arr[i], &arr[i + 1], ascending, &swapped);

        print_array(arr, n);
    } while (swapped);
}

void normal_bubble_sort(int *arr, int n, int *passes, int ascending) {
    printf("\n--- Normal Bubble Sort Iterations ---\n");
    for (int i = 0; i < n - 1; i++) {
        (*passes)++;
        bool swapped = false;
        printf("Pass %d: ", *passes);

        for (int j = 0; j < n - i - 1; j++)
            swap_if_needed(&arr[j], &arr[j + 1], ascending, &swapped);

        print_array(arr, n);
        if (!swapped) break;
    }
}

int main() {
    int n, order, again;
    do {
        printf("\nEnter the number of elements: ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            fprintf(stderr, "Invalid input. Please enter a positive integer.\n");
            while (getchar() != '\n');
            continue;
        }

        int *original = malloc(n * sizeof(int));
        int *arr_opt = malloc(n * sizeof(int));
        int *arr_norm = malloc(n * sizeof(int));

        if (!original || !arr_opt || !arr_norm) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(original); free(arr_opt); free(arr_norm);
            return 1;
        }

        printf("Enter %d integers (unsorted):\n", n);
        for (int i = 0; i < n; i++) {
            if (scanf("%d", &original[i]) != 1) {
                fprintf(stderr, "Invalid input. Please enter integers only.\n");
                free(original); free(arr_opt); free(arr_norm);
                return 1;
            }
        }

        printf("Sort order (1 for Ascending, 0 for Descending): ");
        if (scanf("%d", &order) != 1 || (order != 0 && order != 1)) {
            fprintf(stderr, "Invalid sort order.\n");
            free(original); free(arr_opt); free(arr_norm);
            return 1;
        }

        int ascending = order;

        copy_array(arr_opt, original, n);
        copy_array(arr_norm, original, n);

        int opt_passes = 0, norm_passes = 0;
        optimized_bubble_sort(arr_opt, n, &opt_passes, ascending);
        normal_bubble_sort(arr_norm, n, &norm_passes, ascending);

        printf("\n=== Sorting Summary ===\n");
        printf("> Optimized Bubble Sort Result:\n");
        print_array(arr_opt, n);
        printf("Total Passes: %d\n", opt_passes);

        printf("> Normal Bubble Sort Result:\n");
        print_array(arr_norm, n);
        printf("Total Passes: %d\n", norm_passes);

        if (opt_passes < norm_passes)
            printf("\nOptimized sort was more efficient.\n");
        else if (opt_passes > norm_passes)
            printf("\nNormal bubble sort was more efficient (unexpected!).\n");
        else
            printf("\nBoth algorithms used the same number of passes.\n");

        free(original); free(arr_opt); free(arr_norm);

        printf("\nDo you want to sort again? (1 for Yes, 0 for No): ");
        if (scanf("%d", &again) != 1 || (again != 0 && again != 1)) {
            fprintf(stderr, "Invalid input. Exiting.\n");
            break;
        }
    } while (again == 1);

    printf("\nThank you for using the program!\n");
    return 0;
}