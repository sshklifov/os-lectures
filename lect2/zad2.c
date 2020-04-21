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
} ThreadInfo;

void* Thread(void* arg)
{
    ThreadInfo* pInfo = (ThreadInfo*)arg;
    int* start = pInfo->start;
    int n = pInfo->n;
    int what = pInfo->what;

    for (int i = 0; i < n; ++i) {
        if (start[i] == what) {
            long long res = i;
            return (void*)res;
        }
    }
    long long res = -1;
    return (void*)res;
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

    struct timeval then;
    int s = gettimeofday(&then, NULL);
    assert(s == 0);

    printf("Starting search!\n");
    int what = 987654321;

    pthread_t threads[NTHREADS];
    ThreadInfo info[NTHREADS];

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
        int s = pthread_create(&threads[i], NULL, Thread, &info[i]);
        assert(s == 0);
        numFinished += toFinish;
    }
    bool found = false;
    numFinished = 0;
    for (int i = 0; i < NTHREADS; ++i) {
        void* rval;
        int s = pthread_join(threads[i], &rval);
        assert(s == 0);

        long long index = (long long)rval;
        if (index >= 0) {
            printf("Thread_%d found %d @ index %lld\n", i, what, index);
            int realIndex = numFinished + index;
            printf("Number is originally @ index %d\n", realIndex);
            found = true;
        }
        numFinished += info[i].n;
    }
    if (!found) {
        printf("Cound not find %d\n", what);
    }

    struct timeval now;
    s = gettimeofday(&now, NULL);
    assert(s == 0);

    int secs = now.tv_sec - then.tv_sec;
    int msecs = now.tv_usec - then.tv_usec;
    double elapsed = secs + (double)msecs / 1000000;
    printf("Seconds elapsed: %lf\n", elapsed);

    return 0;
}
