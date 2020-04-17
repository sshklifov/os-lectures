#include <cstdio>

int main()
{
    /// Note that compiler can optimize out the loop

    int n = 1'000'000'000;
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        ++cnt;
    }

    printf("Final counter: %d\n", cnt);
}
