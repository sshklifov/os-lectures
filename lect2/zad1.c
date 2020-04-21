#include <stdio.h>
#include <assert.h>

long long Thread(int from, int to)
{
    long long res = 0;
    for (int i = from; i < to; ++i) {
        res += i;
    }
    return res;
}

int main()
{
    const int NTHREADS = 4;
    int n = 1000000000;

    /// NOTE: use long long, overflow otherwise
    long long res = 0;

    int threadFrom = 0, threadTo = 0;
    for (int i = 0; i < NTHREADS; ++i) {
        threadFrom = threadTo;
        if (i != NTHREADS - 1) {
            threadTo = threadFrom + n / 4;
        }
        else {
            threadTo = n + 1;
        }
        res += Thread(threadFrom, threadTo);
    }
    printf("sum 1..%d is: %lld\n", n, res);

    return 0;
}
