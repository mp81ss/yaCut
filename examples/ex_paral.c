/* Check execution time the serial version and the parallel if you have
   at least 2 CPUs. Compile the parallel enabling -DYCT_OPT_ENABLE_PARALLEL
   and /openmp
*/
#include <yacut.h>

#ifndef N
#define N (128 * 1024) /* Tune this for your CPU speed */
#endif

static int activations = 0;

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

static void act(void)
{
    YCT_SYNCHRONIZED() /* Block executed serially, one thread at time */
    {
        printf("Activation %d\n", ++activations);
    }

    /* Note: For increments/decrements see YCT_ATOMIC() doc! */

    core(N);
}

/* just a normal function */
static void parallel(void)
{
#ifndef YCT_OPT_DISABLE_TIMING
    YCT_TIMER t;
    float seconds;
#endif

    /* Extra time functions offered by yaCut, no tests required */

    YCT_START_TIMER(t);

    /* no semicolon on parallel directives! */
    YCT_PARALLEL()
    {
        YCT_GO()
        act(); /* This will run in parallel... */
        
        YCT_GO()
        {
            act(); /* ... with this */
        }
    }

    YCT_STOP_TIMER(t);
    YCT_GET_TIME(t, seconds);
#ifndef YCT_OPT_DISABLE_TIMING
    printf("Elapsed %.2f seconds\n", seconds);
#endif

    printf("%d activations\n", activations);
}

int main(void)
{
    YCT_BEGIN("Parallel");

    YCT_PARALLEL() /* Please note no semicolon on parallel directives */
    {
        YCT_SCHEDULE_TEST()
        YCT_PARALLEL_TEST(pt1) /* pt1 will run in parallel... */
        YCT_JOIN_TEST()
        YCT_COLLECT_TEST_RESULTS()

        YCT_SCHEDULE_TEST()
        YCT_PARALLEL_TEST(pt2) /* ... with pt2 */
        YCT_JOIN_TEST()
        YCT_COLLECT_TEST_RESULTS()
    }

    YCT_DUMP();

    YCT_END();

    puts("\n");

    /* This is to demonstrate extra-yaCut parallel capabilities */
    parallel();  

    return 0;
}
