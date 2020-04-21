#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdbool.h>

bool IsPrime(int d)
{
    assert(d >= 2);
    for (int i = 2; i < d; ++i) {
        if (d % i == 0) {
            return false;
        }
    }
    return true;
}

void* Thread(void* data)
{
    size_t n = (size_t)data;
    size_t cnt = 0;
    for (size_t i = 2; i <= n; ++i) {
        if (IsPrime(i)) ++cnt;
    }
    return (void*)cnt;
}

int main()
{
    // Program must read 4 integers from stdin. For each integer, it must find
    // the number of prime numbers less than or equal to it.

    int n[4];
    scanf("%d %d %d %d", n, n + 1, n + 2, n + 3);

    pthread_t threads[4];
    for (int i = 0; i < 4; ++i) {
        size_t threadArg = n[i];
        int s = pthread_create(&threads[i], NULL, Thread, (void*)threadArg);
        assert(s == 0);
    }

    for (int i = 0; i < 4; ++i) {
        void* rval;
        int s = pthread_join(threads[i], &rval);
        assert(s == 0);

        size_t res = (size_t)rval;
        printf("# primes less/eq than %d are: %lu\n", n[i], res);
    }
}

