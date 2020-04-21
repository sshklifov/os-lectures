#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int Thread(int* start, int n, int what)
{
    for (int i = 0; i < n; ++i) {
        if (start[i] == what) {
            return i;
        }
    }
    return -1;
}

int main()
{
    int n = 1000000000;
    const int NTHREADS = 4;

    size_t bytes = n * sizeof(int); // 4GB!
    int* arr = (int*)malloc(bytes);
    if (arr == NULL) {
        printf("cannot allocate 4GB\n");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }

    clock_t now = clock();
    printf("Starting search!\n");
    int what = 987654321;

    int numFinished = 0;
    bool found = false;
    for (int i = 0; i < NTHREADS; ++i) {
        int toFinish;
        if (i != NTHREADS - 1) {
            toFinish = n / NTHREADS;
        }
        else {
            toFinish = n - numFinished;
        }
        int idx = Thread(arr + numFinished, toFinish, what);
        if (idx >= 0) {
            found = true;
            break;
        }
        numFinished += toFinish;
    }
    if (found) {
        printf("Found the number %d\n", what);
    }
    else {
        printf("Cound not find %d\n", what);
    }

    clock_t elapsed = clock() - now;
    double secs = (double)elapsed / CLOCKS_PER_SEC;
    printf("Seconds elapsed: %lf\n", secs);

    return 0;
}
