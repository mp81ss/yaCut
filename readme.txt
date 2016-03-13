yaCut v1.1

Yet Another C "Unit" Test framework

- Single header, ready to use
- Very fast (no system call or complex operations)
- Highly Portable (C99, 16/32/64bit support, windows/nix/bsd)
- Follow xUnit architecture
- Support Messages, Warnings and Assertions
- Lot of assertions (values, floating point, strings, arrays, objects)
- Any assertion can add a message
- Support Suites with setup/teardown per test and per suite too
- Disabling/enabling (skip) test/suites in several ways
- Blocking behaviour configurable on assertions and even warnings failure
- Detailed and short result display
- Batch mode support (parametric output + error code)
- Unicode aware
- Free (BSD-license, see file LICENSE for details)


To declare a test:


YCT_TEST(test) {
    YCT_MESSAGE("A positioned message");
    YCT_WARNING(1 != 1);
    YCT_ASSERT_EQUAL_MSG(1, 1, "mmmmm");
    YCT_ASSERT_NULL(NULL);
}

To run it:

int main(void) {
    char* title = "TITLE";

    YCT_BEGIN(title);

    YCT_TEST_RUN(test);
    /* add your tests here */

    YCT_DUMP(); /* print results */

    YCT_END();

    return 0;
}


Suites:

void setup(void) {
    x = 0;
}

/* declare a suite with setup function but not a teardown */

YCT_SUITE(suite, &setup, NULL) {
    YCT_TEST_ADD(test);
    /* add your tests here */
}
YCT_SUITE_END

To run the suite:

YCT_BEGIN("");
YCT_SUITE_RUN(test);
/* add your suites/tests here */

YCT_DUMP_SHORT();
YCT_END();

For a complete reference of yaCut APIs, please see file help.txt
