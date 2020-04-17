#include <cstdio>
#include <pthread.h>
#include <cassert>

void* Thread(void* data)
{
    size_t loops = (size_t)data;
    size_t cnt = 0;
    for (size_t i = 0; i < loops; ++i) {
        ++cnt;
    }

    return (void*)cnt;
}

int main()
{
    /// Note that compiler can optimize out the loop

    int n = 1'000'000'000;

    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        // NOTE: n mod NUM_THREADS is 0
        size_t loopsPerThread = n / NUM_THREADS;
        int s =
            pthread_create(&threads[i], NULL, Thread, (void*)loopsPerThread);
        assert(s == 0);
    }

    int cnt = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        void* rval;
        int s = pthread_join(threads[i], &rval);
        assert(s == 0);

        cnt += (size_t)rval;
    }

    printf("Final counter: %d\n", cnt);
}
