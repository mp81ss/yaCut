/* This file shows usage of suites, according to xUnit architecture. */

#include <yacut.h>


static int x = 1;


static void setup(void)
{
    x = 0;
}

static void teardown(void)
{
    x = -1;
}

YCT_TEST(test_setup)
{
    const char* msg = "WAS SETUP CALLED?";
    YCT_ASSERT_EQUAL_MSG(0, x, msg);
}

YCT_TEST(test_teardown)
{
    YCT_ASSERT_EQUAL_MSG(-1, x, "WAS TEARDOWN CALLED?");
}

/* Note that NULL can be passed for setup/teardown if not needed. */
YCT_SUITE(suite, &setup, &teardown)
{
    /* suite_setup(); Any defined function as suite setup. */

    /* setup() will be executed BEFORE EACH TEST in the suite. */

    YCT_TEST_ADD(test_setup);
    /* add your tests here */

    /* teardown will be executed AFTER EACH TEST in the suite. */

    /* suite_teardown(); Any defined function as suite teardown. */

}
YCT_SUITE_END

int main(void)
{
    int status;

    YCT_BEGIN("xUnit");

    YCT_SUITE_RUN(suite);
    YCT_TEST_RUN(test_teardown);
    /* add your suites/tests here */

    /*  default is stdout, you can pass your FILE* or NULL to disable output,
        for example in batch mode (see YCT_GET_RETURN_VALUE).
     */
    YCT_SET_OUTPUT(NULL);
    YCT_DUMP_SHORT(); /* this won't be written. */
    YCT_SET_OUTPUT(stderr);
    YCT_DUMP(); /* extended dump will be written to stderr. */

    YCT_GET_RETURN_VALUE(status); /* 0 if OK, 1 on warnings, 2 on failed. */

    YCT_END();

    return status;
}
