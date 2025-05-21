#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITEMS 100
#define ITERATIONS 100000  // Number of repetitions for timing

typedef struct {
    int weight;
    int value;
    double ratio;
} Item;

int compare(const void *a, const void *b) {
    double r1 = ((Item *)a)->ratio;
    double r2 = ((Item *)b)->ratio;
    return (r2 > r1) - (r2 < r1);
}

int maximum(int a, int b) {
    return (a > b) ? a : b;
}

// Traditional 0/1 Knapsack (recursive)
int knapsack(int weight[], int value[], int n, int capacity) {
    if (n == 0 || capacity == 0) return 0;
    if (weight[n - 1] > capacity)
        return knapsack(weight, value, n - 1, capacity);
    else {
        int include = value[n - 1] + knapsack(weight, value, n - 1, capacity - weight[n - 1]);
        int exclude = knapsack(weight, value, n - 1, capacity);
        return maximum(include, exclude);
    }
}

// Optimized (Greedy) Knapsack
int optimized_knapsack(Item items[], int n, int capacity) {
    int total_weight = 0, total_value = 0;
    for (int i = 0; i < n; i++) {
        if (total_weight + items[i].weight <= capacity) {
            total_weight += items[i].weight;
            total_value += items[i].value;
        }
    }
    return total_value;
}

int main() {
    int capacity, n;
    Item items[MAX_ITEMS];

    printf("How many capacity of knapsack: ");
    scanf("%d", &capacity);

    printf("How many things you have: ");
    scanf("%d", &n);

    printf("What are their weights:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d: ", i + 1);
        scanf("%d", &items[i].weight);
    }

    printf("\nWhat are their values:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d: ", i + 1);
        scanf("%d", &items[i].value);
        items[i].ratio = (double)items[i].value / items[i].weight;
    }

    qsort(items, n, sizeof(Item), compare);

    int weight[MAX_ITEMS], value[MAX_ITEMS];
    for (int i = 0; i < n; i++) {
        weight[i] = items[i].weight;
        value[i] = items[i].value;
    }

    // Traditional Knapsack timing
    clock_t start1 = clock();
    int result1 = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        result1 = knapsack(weight, value, n, capacity);
    }
    clock_t end1 = clock();
    double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;  // total time for all iterations

    // Optimized Knapsack timing
    clock_t start2 = clock();
    int result2 = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        result2 = optimized_knapsack(items, n, capacity);
    }
    clock_t end2 = clock();
    double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;  // total time for all iterations

    // Output Results
    printf("\n--- Traditional (Recursive) Knapsack ---\n");
    printf("Maximum value: %d\n", result1);
    printf("Total execution time for %d runs: %.6f seconds\n", ITERATIONS, time1);

    printf("\n--- Optimized (Greedy) Knapsack ---\n");
    printf("Maximum value: %d\n", result2);
    printf("Total execution time for %d runs: %.6f seconds\n", ITERATIONS, time2);

    // Show % of greedy solution relative to optimal
    double ratio = ((double)result2 / result1) * 100;
    printf("\nGreedy solution is %.2f%% of the optimal solution.\n", ratio);

    return 0;
}
