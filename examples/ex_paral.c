#include <yacut.h>

#ifndef N
#define N (256 * 1024)
#endif


static int is_prime(unsigned int n)
{
    unsigned int i, p = 1;

    for (i = 2; i < n && p != 0; i++)
        p = n % i;

    return (int)p;
}

static unsigned int core(unsigned int n)
{
    unsigned int i, t = 0;
    for (i = 1; i <= n; i++) {
        if (is_prime(i)) {
            t++;
        }
    }
    return t;
}


YCT_TEST(pt1)
{
    const unsigned int r = core(N);
    YCT_ASSERT(r > 0);
    printf("%u\n", r);
}

YCT_TEST(pt2)
{
    const unsigned int r = core(N);
    YCT_ASSERT(r > 0);
    printf("%u\n", r);
}

int main(void)
{
    YCT_BEGIN("Parallel");

    // pt1 and pt2 can run in parallel
    YCT_PARALLEL()
    {
        YCT_PARALLEL_TEST(pt1)
        YCT_PARALLEL_TEST(pt2)
    }

    YCT_DUMP();

    YCT_END();

    return 0;
}
