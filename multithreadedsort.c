#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 8  // Number of elements in the array

// Global arrays
int arr[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6};
int sorted_arr[SIZE];  // Final merged array

// Struct for passing indices to threads
typedef struct {
    int start;
    int end;
} ThreadData;

// Bubble sort function
void bubble_sort(int start, int end) {
    for (int i = start; i < end - 1; i++) {
        for (int j = start; j < end - (i - start) - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Thread function for sorting
void *sort_sublist(void *param) {
    ThreadData *data = (ThreadData *)param;
    bubble_sort(data->start, data->end);
    pthread_exit(0);
}

// Merge two sorted halves into sorted_arr
void merge(int start1, int end1, int start2, int end2) {
    int i = start1, j = start2, k = 0;
    while (i < end1 && j < end2) {
        if (arr[i] < arr[j]) {
            sorted_arr[k++] = arr[i++];
        } else {
            sorted_arr[k++] = arr[j++];
        }
    }
    while (i < end1) {
        sorted_arr[k++] = arr[i++];
    }
    while (j < end2) {
        sorted_arr[k++] = arr[j++];
    }
}

// Thread function for merging
void *merge_sublists(void *param) {
    merge(0, SIZE / 2, SIZE / 2, SIZE);
    pthread_exit(0);
}

int main() {
    pthread_t sorting_thread0, sorting_thread1, merge_thread;
    ThreadData data0 = {0, SIZE / 2};  // First half
    ThreadData data1 = {SIZE / 2, SIZE};  // Second half

    // Create threads for sorting
    pthread_create(&sorting_thread0, NULL, sort_sublist, &data0);
    pthread_create(&sorting_thread1, NULL, sort_sublist, &data1);

    // Wait for sorting threads to complete
    pthread_join(sorting_thread0, NULL);
    pthread_join(sorting_thread1, NULL);

    // Create thread for merging
    pthread_create(&merge_thread, NULL, merge_sublists, NULL);
    
    // Wait for merging thread to complete
    pthread_join(merge_thread, NULL);

    // Print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_arr[i]);
    }
    printf("\n");

    return 0;
}
