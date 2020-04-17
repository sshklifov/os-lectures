#include <cstdio>
#include <pthread.h>

int cnt = 0;

void* Thread(void* data)
{
    size_t loops = (size_t)data;
    for (size_t i = 0; i < loops; ++i) {
        ++cnt;
    }

    return NULL;
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
        pthread_create(&threads[i], NULL, Thread, (void*)loopsPerThread);
    }

    printf("Final counter: %d\n", cnt);
}
