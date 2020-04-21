#include <stdio.h>

int main()
{
    int n = 1000000000;
    /// NOTE: use long long, overflow otherwise
    long long res = 0;
    for (int i = 1; i <= n; ++i) {
        res += i;
    }

    printf("sum 1..%d is: %lld\n", n, res);

    return 0;
}
