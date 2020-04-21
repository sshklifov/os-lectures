#include <stdio.h>
#include <assert.h>

long long Thread(int from, int to)
{
    long long res = 0;
    for (int i = from; i <= to; ++i) {
        res += i;
    }
    return res;
}

int main()
{
    int n = 1000000000;
    assert(n >= 8);
    /// NOTE: use long long, overflow otherwise
    long long res = 0;

    // Iteration 1
    int threadFrom = 0;
    int threadTo = n / 4;
    res += Thread(threadFrom, threadTo);

    // Iteration 2
    threadFrom = threadTo + 1;
    threadTo = threadFrom + n / 4;
    res += Thread(threadFrom, threadTo);

    // Iteration 3
    threadFrom = threadTo + 1;
    threadTo = threadFrom + n / 4;
    res += Thread(threadFrom, threadTo);

    // Iteration 4
    threadFrom = threadTo + 1;
    threadTo = n;
    res += Thread(threadFrom, threadTo);

    printf("sum 1..%d is: %lld\n", n, res);

    return 0;
}
