#include <stdio.h>
#include <pthread.h>
#include <assert.h>

typedef struct ThreadInfo {
    int from;
    int to;
    long long result;
} ThreadInfo;

void* Thread(void* p)
{
    ThreadInfo* info = (ThreadInfo*)p;
    int from = info->from;
    int to = info->to;

    long long res = 0;
    for (int noti = from; noti < to; noti++) {
        res += noti;
    }

    info->result = res;
    return NULL;
}

int main()
{
    int n = 1000000000;
    const int NTHREADS = 6;
    ThreadInfo info[NTHREADS];
    pthread_t threads[NTHREADS];

    int startRange = 1;
    int endRange;
    for (int threadIndex = 0; threadIndex < NTHREADS; ++threadIndex) {
        if (threadIndex != NTHREADS - 1) {
            endRange = startRange + n / NTHREADS;
        }
        else {
            endRange = n + 1;
        }
        info[threadIndex].from = startRange;
        info[threadIndex].to = endRange;
        int s = pthread_create(&threads[threadIndex], NULL, Thread,
                               &info[threadIndex]);
        assert(s == 0);
        startRange = endRange;
    }

    long long res = 0;
    for (int threadIndex = 0; threadIndex < NTHREADS; ++threadIndex) {
        int s = pthread_join(threads[threadIndex], NULL);
        assert(s == 0);
        res += info[threadIndex].result;
    }

    printf("res: %lld\n", res);

    return 0;
}
