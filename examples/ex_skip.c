#include <assert.h>
#include <yacut.h>

/* This file shows all ways to skip a test/suite. */

static int x = 0;

/* This test will never be executed. */
DISABLE_YCT_TEST(test_skipped)
{
    YCT_MESSAGE("SHOULD NOT SEE THIS...");
    assert(0);
}

YCT_TEST(test_dangerous)
{
    YCT_MESSAGE("SHOULD NOT SEE THIS...");
    assert(0);
}

/* This suite will never be executed. */
DISABLE_YCT_SUITE(suite_skipped, NULL, NULL)
{
    YCT_TEST_ADD(test_dangerous);
}
YCT_SUITE_END

YCT_SUITE(suite_dangerous, NULL, NULL)
{
    YCT_TEST_ADD(test_dangerous);
}
YCT_SUITE_END

YCT_TEST(test)
{
    YCT_ASSERT_EQUAL_MSG(0, x, "Initially defined");
    x++;
}

int main(void)
{
    YCT_BEGIN("Skip");

    /* These 2 won't be executed:
       Notice tha "DISABLE_" prepended above in test/suite declaration,
       1st method.
     */
    YCT_TEST_RUN(test_skipped);
    YCT_SUITE_RUN(suite_skipped);

    /* These 2 won't be executed:
       Notice the "NO_" prepended below to test/suite run directives,
       2nd method.
     */
    NO_YCT_TEST_RUN(test_dangerous);
    NO_YCT_SUITE_RUN(suite_dangerous);

    /* After YCT_DISABLE() directive nothing will be executed.
       Use YCT_ENABLE() to restore default behaviour,
       3rd method.
     */
    YCT_DISABLE();
    YCT_TEST_RUN(test_skipped);
    YCT_SUITE_RUN(suite_skipped);

    YCT_ENABLE(); /* Next tests/suites will be executed. */

    YCT_TEST_RUN(test);
    assert(x == 1); /* Check that test is executed. */

    YCT_DUMP();

    YCT_END();

    return 0;
}
