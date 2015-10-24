/* This file shows a minimal example: how to setup and run a single test. */

#include <yacut.h>

static int x = 1;

YCT_TEST(test)
{
    const char* msg = "Just a message";
    void* p = &x;

    YCT_MESSAGE(msg);
    YCT_WARNING(x == 0);
    YCT_ASSERT_EQUAL_MSG(1, x, "mmmmm");
    YCT_ASSERT_NOT_NULL(p);
}

YCT_MAIN(test)
