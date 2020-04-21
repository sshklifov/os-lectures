#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int FindNumber(int* arr, int n, int what)
{
    for (int i = 0; i < n; ++i) {
        if (arr[i] == what) {
            return i;
        }
    }
    return -1;
}

int main()
{
    int n = 1000000000;
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
    int idx = FindNumber(arr, n, what);

    clock_t elapsed = clock() - now;
    double secs = (double)elapsed / CLOCKS_PER_SEC;

    if (idx >= 0) {
        printf("Found the number %d\n", what);
    }
    else {
        printf("Cound not find %d\n", what);
    }
    printf("Seconds elapsed: %lf\n", secs);

    return 0;
}
