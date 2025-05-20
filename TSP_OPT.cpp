#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 20 // Maximum number of cities that can be implemented in this code

int n; // Number of cities
char labels[MAX]; // Labels for the cities (e.g., A, B, C...)
int weight[MAX][MAX]; // Weights between cities
int turned_off_weights[MAX * MAX]; // Weights that are "turned off"
int turn_off_count = 0; // Number of turned off weights

// Check if a weight is turned off
int is_turned_off(int w) {
    for (int i = 0; i < turn_off_count; i++) {
        if (turned_off_weights[i] == w) return 1;
    }
    return 0;
}

// Compare function for qsort (descending order)
int compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

// Determines the weights to be "turned off" based which are the highest weights in the matrix
void determine_turned_off_weights() {
    int all_weights[MAX * MAX];
    int count = 0;

    // Collect all unique positive weights between different cities
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && weight[i][j] > 0) {
                all_weights[count++] = weight[i][j];
            }
        }
    }

    if (count == 0) return;
    qsort(all_weights, count, sizeof(int), compare); // Sort all weights in descending order

    // Extract unique weights
    int unique[MAX * MAX], ucount = 0;
    for (int i = 0; i < count; i++) {
        if (i == 0 || all_weights[i] != all_weights[i-1]) {
            unique[ucount++] = all_weights[i];
        }
    }

    // Decide how many weights to turn off: 2 + 2 for every 10 cities
    turn_off_count = 2 + (n / 10) * 2;
    if (turn_off_count > ucount) turn_off_count = ucount;

    // Store the highest weights as turned off
    for (int i = 0; i < turn_off_count; i++) {
        turned_off_weights[i] = unique[i];
    }
}

int opt_cost = INT_MAX; // Minimum path cost found
int opt_path[MAX+1]; // Best path found (including return to start)
int opened[MAX * MAX]; // Temporarily used turned-off weights during DFS
int opened_count = 0;
int opt_opened[MAX * MAX]; // Final used turned-off weights for best path
int opt_opened_count = 0;

// Depth-First Search for TSP with backtracking (optimized version)
void dfs(int depth, int cost, int path[], int used[]) {
    if (depth == n) { // All cities have been visited
        int last_leg = weight[path[n-1]][0]; // Return trip to start
        if (last_leg == 0) return;
        int added = 0;

        // Check if return leg uses a turned-off weight
        if (is_turned_off(last_leg)) {
            int already_added = 0;
            for (int k = 0; k < opened_count; k++) {
                if (opened[k] == last_leg) {
                    already_added = 1;
                    break;
                }
            }
            if (!already_added) {
                opened[opened_count++] = last_leg;
                added = 1;
            }
        }

        cost += last_leg;
        if (cost < opt_cost) {
            opt_cost = cost;
            memcpy(opt_path, path, n * sizeof(int));
            opt_path[n] = 0; // Complete the tour
            memcpy(opt_opened, opened, opened_count * sizeof(int)); // Save which turned-off weights were used
            opt_opened_count = opened_count;
        }

        if (added) opened_count--;
        return;
    }

    // Try visiting all unvisited cities
    for (int i = 1; i < n; i++) {
        if (!used[i] && weight[path[depth-1]][i] > 0) {
            int w = weight[path[depth-1]][i];
            int added = 0;

            // Check if this weight is turned off and not yet tracked
            if (is_turned_off(w)) {
                int already_added = 0;
                for (int k = 0; k < opened_count; k++) {
                    if (opened[k] == w) {
                        already_added = 1;
                        break;
                    }
                }
                if (!already_added) {
                    opened[opened_count++] = w;
                    added = 1;
                }
            }

            used[i] = 1;
            path[depth] = i;
            dfs(depth + 1, cost + w, path, used);
            used[i] = 0;

            if (added) opened_count--; // Backtrack turned-off usage
        }
    }
}

// Brute-force DFS without optimization
int tsp_cost = INT_MAX;
int tsp_path[MAX+1];
void tsp_dfs(int depth, int cost, int path[], int used[]) {
    if (depth == n) {
        int last_leg = weight[path[n-1]][0];
        if (last_leg == 0) return;
        cost += last_leg;
        if (cost < tsp_cost) {
            tsp_cost = cost;
            memcpy(tsp_path, path, n * sizeof(int));
            tsp_path[n] = 0;
        }
        return;
    }

    for (int i = 1; i < n; i++) {
        if (!used[i] && weight[path[depth-1]][i] > 0) {
            used[i] = 1;
            path[depth] = i;
            tsp_dfs(depth + 1, cost + weight[path[depth-1]][i], path, used);
            used[i] = 0;
        }
    }
}

void tsp() {
    int path[MAX], used[MAX] = {0};
    path[0] = 0;
    used[0] = 1;
    tsp_dfs(1, 0, path, used);

    printf("\n--- TSP Simple (Non-Optimized) Solution ---\n");
    if (tsp_cost == INT_MAX) {
        printf("No valid path found.\n");
        return;
    }

    printf("Path: ");
    for (int i = 0; i <= n; i++) {
        printf("%c", labels[tsp_path[i]]);
        if (i != n) printf(" -> ");
    }
    printf("\nTotal Cost: %d\n", tsp_cost);
}

// Entry point to start the TSP solver
void opt_tsp() {
    int path[MAX];
    int used[MAX] = {0};
    path[0] = 0;
    used[0] = 1; // Start from city 0
    dfs(1, 0, path, used);

    printf("\n--- TSP Optimized Solution ---\n");
    if (opt_cost == INT_MAX) {
        printf("No valid path found.\n");
        return;
    }

    printf("Path: ");
    for (int i = 0; i <= n; i++) {
        printf("%c", labels[opt_path[i]]);
        if (i != n) printf(" -> ");
    }
    printf("\nTotal Cost: %d\n", opt_cost);

    printf("Turned Off Weights: ");
    for (int i = 0; i < turn_off_count; i++) printf("%d ", turned_off_weights[i]);
    printf("\nOpened (Used Anyway): ");
    for (int i = 0; i < opt_opened_count; i++) printf("%d ", opt_opened[i]);
    printf("\n");
}

int main() {
    // Input number of cities
    printf("How many cities?\nEnter here: ");
    scanf("%d", &n);
    printf("\n");
    
    if (n < 2 || n > MAX) {
        printf("Invalid number of cities (must be between 2 and %d).\n", MAX);
        return 1;
    }

    // Input city labels (e.g., A, B, C...)
    for (int i = 0; i < n; i++) {
        printf("Label [City %d]: ", i + 1);
        scanf(" %c", &labels[i]);
    }

    // Input weight matrix
    for (int i = 0; i < n; i++) {
        printf("\nFrom %c:\n", labels[i]);
        for (int j = 0; j < n; j++) {
            if (i == j) {
                weight[i][j] = 0; // No travel to the same city
                continue;
            }
            printf("  Weight to %c: ", labels[j]);
            scanf("%d", &weight[i][j]);
        }
    }

    determine_turned_off_weights(); // Identify top weights to discourage

    tsp(); // Solve using non-optimized version
    opt_tsp();        // Solve using optimized version

    return 0;
}


