#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function to generate the Fibonacci series
void fibonacciGenerator(int* series, int length) {
    series[0] = 0;
    series[1] = 1;

    for (int i = 2; i < length; i++) {
        series[i] = series[i - 1] + series[i - 2];
    }
}

// Function to count the number of even numbers in a series
int countEvenNumbers(const int* series, int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (series[i] % 2 == 0) {
            count++;
        }
    }
    return count;
}

// Function to count the number of odd numbers in a series
int countOddNumbers(const int* series, int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (series[i] % 2 != 0) {
            count++;
        }
    }
    return count;
}

// Function to calculate the sum of a series and write it to a file
void calculateSum(const int* series, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += series[i];
    }
    FILE* file = fopen("sum.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", sum);
        fclose(file);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("errorn");
        return 1;
    }

    int N = atoi(argv[1]);
    int* series = (int*)malloc(N * sizeof(int));

    // Generate the Fibonacci series
    fibonacciGenerator(series, N);

    // Print the Fibonacci series
    printf("ID = %ld, Series:", pthread_self());
    for (int i = 0; i < N; i++) {
        printf(" %d", series[i]);
    }
    printf("\n");

    // Count the number of even numbers
    int evenCount = countEvenNumbers(series, N);
    printf("ID = %ld, Even Numbers: %d\n", pthread_self(), evenCount);

    // Count the number of odd numbers
    int oddCount = countOddNumbers(series, N);
    printf("ID = %ld, Odd Numbers: %d\n", pthread_self(), oddCount);

    // Calculate the sum and write it to a file
    calculateSum(series, N);
    FILE* file = fopen("sum.txt", "r");
    int sum;
    if (file != NULL) {
        fscanf(file, "%d", &sum);
        fclose(file);
        printf("ID = %ld, Sum: %d\n", pthread_self(), sum);
    }

    // Free the allocated memory
    free(series);

    return 0;
}
