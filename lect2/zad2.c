#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct ThreadInfo {
    int* arr;
    int n;
    int what;
    bool result;
} ThreadInfo;

void* FindNumber(void* p)
{
    ThreadInfo* info = (ThreadInfo*)p;
    int* arr = info->arr;
    int n = info->n;
    int what = info->what;

    for (int i = 0; i < n; ++i) {
        if (arr[i] == what) {
            info->result = true;
            return NULL;
        }
    }

    info->result = false;
    return NULL;
}

bool TimedWork(int* arr, int n)
{
    int what = 987654321;
    const int NTHREADS = 4;
    ThreadInfo infos[NTHREADS];
    pthread_t threads[NTHREADS];

    int* startAddress = arr;
    int remainingElems = n;
    for (int i = 0; i < NTHREADS; ++i) {
        int toProcess;
        if (i != NTHREADS - 1) {
            toProcess = n / NTHREADS;
        }
        else {
            toProcess = remainingElems;
        }
        infos[i].arr = startAddress;
        infos[i].n = toProcess;
        infos[i].what = what;

        int s = pthread_create(&threads[i], NULL, FindNumber, &infos[i]);
        assert(s == 0);
        startAddress = startAddress + toProcess;
        remainingElems -= toProcess;
    }
    for (int i = 0; i < NTHREADS; ++i) {
        int s = pthread_join(threads[i], NULL);
        assert(s == 0);
        if (infos[i].result) {
            return true;
        }
    }
    assert(remainingElems == 0);
    return false;
}

int main()
{
    int n = 500000000;

    size_t bytes = n * sizeof(int); // 2GB!
    int* arr = (int*)malloc(bytes);
    if (arr == NULL) {
        printf("cannot allocate 2GB\n");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }

    /* struct timeval then; */
    /* int s = gettimeofday(&then, NULL); */
    /* assert(s == 0); */

    printf("Starting search!\n");
    bool found = TimedWork(arr, n);

    /* struct timeval now; */
    /* s = gettimeofday(&now, NULL); */
    /* assert(s == 0); */

    if (found) {
        printf("Found number\n");
    }
    else {
        printf("Cound not find\n");
    }

    /* int secs = now.tv_sec - then.tv_sec; */
    /* int msecs = now.tv_usec - then.tv_usec; */
    /* double elapsed = secs + (double)msecs / 1000000; */
    /* printf("Seconds elapsed: %lf\n", elapsed); */

    return 0;
}
