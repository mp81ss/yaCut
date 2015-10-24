#include <assert.h>
#include <yacut.h>

/* This file shows blocking modes (behaviour after assertions failure).
   Once blocked, NO tests/suites will be run in the current YCT_BEGIN/YCT_END
   context.
 */

static int x = 0;


YCT_TEST(test_set_x_to_1_and_fail)
{
    x = 1;
    YCT_ASSERT_EQUAL_MSG(0, x, "Was set to 1");
}

YCT_TEST(test_set_x_to_2)
{
    YCT_MESSAGE("Setting x to 2");
    x = 2;
}

YCT_SUITE(suite, NULL, NULL)
{
    YCT_TEST_ADD(test_set_x_to_1_and_fail);
}
YCT_SUITE_END

static void show_blocking(void)
{
    YCT_BEGIN("Blocking");

    /* Default behaviour is to continue running tests when a test fails. */
    YCT_TEST_RUN(test_set_x_to_1_and_fail); /* This test fails, but... */
    YCT_TEST_RUN(test_set_x_to_2); /* This is executed in default mode. */

    /* Now set block mode, after a test fails with an assertion, following
       tests will not execute.*/
    YCT_SET_BLOCKED();
    YCT_TEST_RUN(test_set_x_to_1_and_fail); /* This test fails, x = 1 */
    YCT_TEST_RUN(test_set_x_to_2); /* This doesn't run in blocking mode. */
    assert(x == 1);
    x = 2;
    YCT_SUITE_RUN(suite); /* This is not executed in blocking mode. */

    YCT_DUMP();
    YCT_DUMP_SHORT();

    putchar('\n');

    YCT_END();
}

YCT_TEST(test_warning_set_x_to_1)
{
    x = 1;
    YCT_WARNING_MSG(x == 0, "Was set to 1");
}

static void show_full_blocking(void)
{
    const char* title = "Full Blocking";

    YCT_BEGIN(title);

    /* Full blocking mode is like blocking mode but even warnings cause
       blocking, so you can have full control of checks flow.
     */
    YCT_SET_FULL_BLOCKED();
    YCT_TEST_RUN(test_warning_set_x_to_1); /* This test triggers a warning */
    YCT_TEST_RUN(test_set_x_to_2); /* This is not executed. */
    assert(x == 1);
    x = 2;
    YCT_SUITE_RUN(suite); /* This is not executed. */
    assert(x == 2);

    YCT_DUMP();
    YCT_DUMP_SHORT();

    YCT_END();
}

int main(void)
{
    show_blocking();
    assert(x == 2);
    show_full_blocking();
    assert(x == 2);

    puts("\nALL OK!");

    return x - 2;
}
