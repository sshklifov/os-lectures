#include <stdio.h>
#include <assert.h>
#include <pthread.h>

typedef struct ThreadInfo {
    int from;
    int to;
} ThreadInfo;

void* Thread(void* arg)
{
    ThreadInfo* pinfo = (ThreadInfo*)arg;
    int from = pinfo->from;
    int to = pinfo->to;

    long long res = 0;
    for (int i = from; i < to; ++i) {
        res += i;
    }
    return (void*)res;
}

int main()
{
    const int NTHREADS = 4;
    int n = 1000000000;

    /// NOTE: use long long, overflow otherwise
    ThreadInfo info[NTHREADS];
    pthread_t threads[NTHREADS];

    int threadFrom = 0, threadTo = 0;
    for (int i = 0; i < NTHREADS; ++i) {
        threadFrom = threadTo;
        if (i != NTHREADS - 1) {
            threadTo = threadFrom + n / 4;
        }
        else {
            threadTo = n + 1;
        }

        info[i].from = threadFrom;
        info[i].to = threadTo;
        int s = pthread_create(&threads[i], NULL, Thread, &info[i]);
        assert(s == 0);
    }

    long long res = 0;
    for (int i = 0; i < NTHREADS; ++i) {
        void* rval;
        int s = pthread_join(threads[i], &rval);
        assert(s == 0);

        res += (long long)rval;
    }
    printf("sum 1..%d is: %lld\n", n, res);

    return 0;
}
