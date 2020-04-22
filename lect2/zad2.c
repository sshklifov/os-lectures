#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>

typedef struct ThreadInfo {
    int* start;
    int n;
    int what;
    bool* pGlobalRes;
} ThreadInfo;

void* PerThreadWork(void* arg)
{
    ThreadInfo* pInfo = (ThreadInfo*)arg;
    int* start = pInfo->start;
    int n = pInfo->n;
    int what = pInfo->what;
    bool* pGlobalRes = pInfo->pGlobalRes;

    for (int i = 0; i < n; ++i) {
        if (*pGlobalRes) {
            break;
        }
        if (start[i] == what) {
            *pGlobalRes = 1;
            break;
        }
    }
    return NULL;
}

bool DoWork(int* arr, int n, int what)
{
    const int NTHREADS = 4;
    pthread_t threads[NTHREADS];
    ThreadInfo info[NTHREADS];
    bool res = 0;

    int numFinished = 0;
    for (int i = 0; i < NTHREADS; ++i) {
        int toFinish;
        if (i != NTHREADS - 1) {
            toFinish = n / NTHREADS;
        }
        else {
            toFinish = n - numFinished;
        }
        info[i].start = arr + numFinished;
        info[i].n = toFinish;
        info[i].what = what;
        info[i].pGlobalRes = &res;
        int s = pthread_create(&threads[i], NULL, PerThreadWork, &info[i]);
        assert(s == 0);
        numFinished += toFinish;
    }
    bool found = false;
    numFinished = 0;
    for (int i = 0; i < NTHREADS; ++i) {
        int s = pthread_join(threads[i], NULL);
        assert(s == 0);
    }
    return res;
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

    struct timeval then;
    int s = gettimeofday(&then, NULL);
    assert(s == 0);

    printf("Starting search!\n");
    int what = 4;
    bool found = DoWork(arr, n, what);

    struct timeval now;
    s = gettimeofday(&now, NULL);
    assert(s == 0);

    if (found) {
        printf("Found %d!\n", what);
    }
    else {
        printf("Cound not find %d\n", what);
    }

    int secs = now.tv_sec - then.tv_sec;
    int msecs = now.tv_usec - then.tv_usec;
    double elapsed = secs + (double)msecs / 1000000;
    printf("Seconds elapsed: %lf\n", elapsed);

    return 0;
}
