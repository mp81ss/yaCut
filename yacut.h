/*
 *  yaCut v1.0 - Yet Another C Unit Test
 *  Copyright (c) 2015 - Michele Pes
 *
 *  Distributed under the BSD License
 *  See accompanying file LICENSE.txt or copy at
 *  http://opensource.org/licenses/BSD-3-Clause
 */

#ifndef YACUT_H_
#define YACUT_H_

#include <stdio.h>

#ifndef YCT_FUNC_NAME
#if (defined(_MSC_VER) && (_MSC_VER >= 1310)) || defined(__WATCOMC__)
#define YCT_FUNC_NAME __FUNCTION__
#elif (defined(__BORLANDC__) && (__BORLANDC__ >= 0x550))
#define YCT_FUNC_NAME __FUNC__
#elif (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901))
#define YCT_FUNC_NAME __func__
#elif defined(__GNUC__)
#define YCT_FUNC_NAME __PRETTY_FUNCTION__
#else
#define YCT_FUNC_NAME "YCT_TEST"
#endif
#endif

#define VNUT_YCT_FLAGS_BLOCKING_MODE      1
#define VNUT_YCT_FLAGS_FULL_BLOCKING_MODE 2
#define VNUT_YCT_FLAGS_LOCKED             4
#define VNUT_YCT_FLAGS_DISABLED           8
#define VNUT_YCT_FLAGS_IS_SUITE           16

#define YCT_GET_NAME()      "yaCut"
#define YCT_VERSION_MAJOR() 1
#define YCT_VERSION_MINOR() 0

struct yct_context {
    const char* msg;
    FILE* out;
    int suites;
    int messages;
    int tests;
    int checks;
    int warnings;
    int failed;
    int flags;
};

#define VNUT_YCT_INIT_DATA(ctx) \
    ctx.msg = NULL;             \
    ctx.suites   = 0;           \
    ctx.messages = 0;           \
    ctx.tests    = 0;           \
    ctx.checks   = 0;           \
    ctx.warnings = 0;           \
    ctx.failed   = 0;           \
    ctx.flags    = 0

#define VNUT_YCT_GET_BIT(var, mask) ((var) & (mask))
#define VNUT_YCT_CLEAR_BIT(var, mask) ((var) &= ~(mask))
#define VNUT_YCT_SET_BIT(var, mask) { VNUT_YCT_CLEAR_BIT((var), (mask)); \
    ((var) |= (mask)); }
#define VNUT_YCT_COPY_BIT(src, dest, mask) \
    VNUT_YCT_SET_BIT((dest), VNUT_YCT_GET_BIT((src), (mask)));

#define VNUT_YCT_ABS(a) ((a) < (-(a)) ? -(a) : (a))

#define YCT_DEFAULT_OUTPUT (stdout)

#define YCT_SET_OUTPUT(handle) (yct_main_ctx_.out = (handle))

#define YCT_GET_DATA(ctx) ((ctx) = *p_yct_ctx_)

#define YCT_GET_RETURN_VALUE(val) {   \
    if (p_yct_ctx_->failed > 0)       \
        val = 2;                      \
    else {                            \
        if (p_yct_ctx_->warnings > 0) \
            val = 1;                  \
        else                          \
            val = 0;                  \
    }                                 \
}

/* Enable/Disable */
#define YCT_ENABLE() VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, \
                                        VNUT_YCT_FLAGS_DISABLED)
#define YCT_DISABLE() VNUT_YCT_SET_BIT(p_yct_ctx_->flags, \
                                       VNUT_YCT_FLAGS_DISABLED)
#define YCT_IS_ENABLED() \
    (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED) == 0)

/* Blocking */
#define YCT_SET_BLOCKED() {                                \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                  \
                       VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE); }

#define YCT_SET_FULL_BLOCKED() {                                 \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE); \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); }

#define YCT_DISABLE_BLOCKING() {                                 \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE | \
                                      VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); }

#define YCT_IS_LOCKED() VNUT_YCT_GET_BIT(p_yct_ctx_->flags, \
                                         VNUT_YCT_FLAGS_LOCKED)

#define VNUT_YCT_IF_OK if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, \
    VNUT_YCT_FLAGS_DISABLED | VNUT_YCT_FLAGS_LOCKED) == 0)

#define VNUT_YCT_IF_SET_BLOCKED() if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,   \
    VNUT_YCT_FLAGS_BLOCKING_MODE | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) != 0) \
    { VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED); }

/* General */
#define YCT_BEGIN(name) {                                             \
    struct yct_context yct_main_ctx_;                                 \
    struct yct_context* const p_yct_ctx_ = &yct_main_ctx_;            \
    VNUT_YCT_INIT_DATA(yct_main_ctx_);                                \
    yct_main_ctx_.out = YCT_DEFAULT_OUTPUT;                           \
    if ((name) != NULL && *(name) != 0) {                             \
        if (*(name) != '\"') yct_main_ctx_.msg = (const char*)(name); \
        else yct_main_ctx_.msg = #name;                               \
    } else p_yct_ctx_->msg = ""; VNUT_YCT_GET_START_TIME();

#define YCT_TEST(test_name) \
    static void test_name(struct yct_context* const p_yct_ctx_)

#define DISABLE_YCT_TEST(test_name) YCT_TEST(test_name) \
    { (void)p_yct_ctx_; return; }                       \
    void test_name##_YCT_DISABLED_TEST_(struct yct_context* const p_yct_ctx_)

#define YCT_SUITE(suite_name, setup, teardown)                     \
    static void suite_name(struct yct_context* const p_yct_ctx_) { \
        void (*f_setup_)(void) = setup; void (*f_tear_down_)(void) = teardown;

#define YCT_SUITE_END                                             \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_IS_SUITE); \
    (void)f_setup_; (void)f_tear_down_; return; }

#define DISABLE_YCT_SUITE(suite_name, setup, teardown)                      \
    YCT_SUITE(suite_name, setup, teardown) YCT_SUITE_END                    \
    void                                                                    \
    test_name##_YCT_DISABLED_SUITE_(struct yct_context* const p_yct_ctx_) { \
        void (*f_setup_)(void) = setup; void (*f_tear_down_)(void) = teardown;

#define YCT_TEST_RUN(test_name)                                   \
    VNUT_YCT_IF_OK {                                              \
        struct yct_context yct_ctx_;                              \
        VNUT_YCT_INIT_DATA(yct_ctx_);                             \
        yct_ctx_.out = p_yct_ctx_->out;                           \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,      \
            VNUT_YCT_FLAGS_BLOCKING_MODE                          \
            | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE);                 \
        test_name(&yct_ctx_);                                     \
        p_yct_ctx_->tests++;                                      \
        p_yct_ctx_->warnings += yct_ctx_.warnings;                \
        p_yct_ctx_->checks += yct_ctx_.checks;                    \
        p_yct_ctx_->messages += yct_ctx_.messages;                \
        if (yct_ctx_.failed > 0) {                                \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,  \
                                          VNUT_YCT_FLAGS_LOCKED); \
            p_yct_ctx_->failed++;                                 \
        }                                                         \
    }

#define NO_YCT_TEST_RUN(test_name) (void)(test_name);

#define YCT_TEST_ADD(yct_name)                                   \
    VNUT_YCT_IF_OK {                                             \
        struct yct_context yct_ctx_;                             \
        VNUT_YCT_INIT_DATA(yct_ctx_);                            \
        yct_ctx_.out = p_yct_ctx_->out;                          \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,     \
            VNUT_YCT_FLAGS_BLOCKING_MODE                         \
            | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE);                \
        if (f_setup_ != NULL)                                    \
            (*f_setup_)();                                       \
        yct_name(&yct_ctx_);                                     \
        if (f_tear_down_ != NULL)                                \
            (*f_tear_down_)();                                   \
        p_yct_ctx_->tests++;                                     \
        p_yct_ctx_->warnings += yct_ctx_.warnings;               \
        p_yct_ctx_->checks += yct_ctx_.checks;                   \
        p_yct_ctx_->messages += yct_ctx_.messages;               \
        if (yct_ctx_.failed > 0) {                               \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags, \
                          VNUT_YCT_FLAGS_LOCKED);                \
            p_yct_ctx_->failed++;                                \
        }                                                        \
    }

#define NO_YCT_TEST_ADD(test_name) (void)(test_name);

#define NO_YCT_SUITE_RUN(suite_name) (void)(suite_name);

#define YCT_SUITE_RUN(yct_name)                              \
    VNUT_YCT_IF_OK {                                         \
        struct yct_context yct_ctx_;                         \
        VNUT_YCT_INIT_DATA(yct_ctx_);                        \
        yct_ctx_.out = p_yct_ctx_->out;                      \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags, \
            VNUT_YCT_FLAGS_BLOCKING_MODE                     \
            | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE);            \
        yct_name(&yct_ctx_);                                 \
        VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags, \
                      VNUT_YCT_FLAGS_LOCKED);                \
        p_yct_ctx_->suites++;                                \
        p_yct_ctx_->tests += yct_ctx_.tests;                 \
        p_yct_ctx_->failed += yct_ctx_.failed;               \
        p_yct_ctx_->warnings += yct_ctx_.warnings;           \
        p_yct_ctx_->checks += yct_ctx_.checks;               \
        p_yct_ctx_->messages += yct_ctx_.messages;           \
    }

#define YCT_END() (void)yct_main_ctx_; }

#define YCT_MAIN(entry) int main(void) {                                \
    YCT_BEGIN(""); entry(&yct_main_ctx_);                               \
    if (VNUT_YCT_GET_BIT(yct_main_ctx_.flags, VNUT_YCT_FLAGS_IS_SUITE)) \
        yct_main_ctx_.suites++; else yct_main_ctx_.tests++;             \
    YCT_DUMP(); YCT_END(); return 0; }

#define VNUT_YCT_FPUTC(c) ((void)fputc((c), p_yct_ctx_->out))
#define VNUT_YCT_FPUTS(str) ((void)fprintf(p_yct_ctx_->out, str))
#define VNUT_YCT_PRINT_STR(str)                                        \
    { if (*(str) == '\"') (void)fputs((void*)(#str), p_yct_ctx_->out); \
    else (void)fputs((void*)(str), p_yct_ctx_->out); }

#define YCT_DUMP_SHORT()                                         \
    if (yct_main_ctx_.out != NULL) {                             \
        if (yct_main_ctx_.failed > 0) {                          \
            (void)fprintf(yct_main_ctx_.out, "%d FAILED\n",      \
                          yct_main_ctx_.failed);                 \
        }                                                        \
        else {                                                   \
            if (yct_main_ctx_.warnings > 0) {                    \
                (void)fprintf(yct_main_ctx_.out, "%d WARNING\n", \
                              yct_main_ctx_.warnings);           \
            }                                                    \
            else                                                 \
                VNUT_YCT_FPUTS("ALL OK\n");                      \
        }                                                        \
    }

#define YCT_DUMP()                                                           \
if (p_yct_ctx_->out != NULL) {                                               \
    const int yct_has_label_ = *p_yct_ctx_->msg != 0;                        \
    const int tests = p_yct_ctx_->tests > 0 ? p_yct_ctx_->tests : 1;         \
    (void)fprintf(p_yct_ctx_->out, "\n------ %s v%d.%d Summary -----"        \
    "%s%s\nTests:      %d\nPassed:     %d\nFailed:     %d\nSuites:     %d\n" \
    "Messages:   %d\nWarnings:   %d\nAssertions: %d\nSuccess:    %.0f%%",    \
    YCT_GET_NAME(), YCT_VERSION_MAJOR(), YCT_VERSION_MINOR(),                \
    yct_has_label_ ? "\nExecuting battery: " : "", yct_has_label_ ?          \
    p_yct_ctx_->msg : "", tests, tests - p_yct_ctx_->failed,                 \
    p_yct_ctx_->failed, p_yct_ctx_->suites, p_yct_ctx_->messages,            \
    p_yct_ctx_->warnings, p_yct_ctx_->checks - p_yct_ctx_->warnings,         \
    (float)((tests - p_yct_ctx_->failed) * 100) / (float)tests);             \
    if (p_yct_ctx_->failed > 0)                                              \
        VNUT_YCT_FPUTS("\n------> SOMETHING FAILED <-----\n");               \
    else {                                                                   \
        if (p_yct_ctx_->warnings > 0)                                        \
            VNUT_YCT_FPUTS("\n-------> SOME WARNING <--------\n");           \
        else                                                                 \
            VNUT_YCT_FPUTS("\n----------> ALL OK <-----------\n");           \
    }                                                                        \
}

/* Internal prints */
#ifdef YCT_DISABLE_INT64

#define VNUT_YCT_PRINT_VAR(var) {                                        \
    const size_t yct_pv_len_ = sizeof(var);                              \
    switch(yct_pv_len_) {                                                \
        case 1: (void)fprintf(p_yct_ctx_->out, "%c", (char)var); break;  \
        case 2: (void)fprintf(p_yct_ctx_->out, "%d", (int)var); break;   \
        case 4: (void)fprintf(p_yct_ctx_->out, "%ld", (long)var); break; \
        default: break;                                                  \
    }                                                                    \
}

#else

#ifdef __BORLANDC__
#define VNUT_YCT_INT64 __int64
#else
#define VNUT_YCT_INT64 long long
#endif

#define VNUT_YCT_PRINT_VAR(var) {                                           \
    const size_t yct_pv_len_ = sizeof(var);                                 \
    switch(yct_pv_len_) {                                                   \
        case 1: (void)fprintf(p_yct_ctx_->out, "%c", (char)var); break;     \
        case 2: (void)fprintf(p_yct_ctx_->out, "%hd", (short)var); break;   \
        case 4: (void)fprintf(p_yct_ctx_->out, "%d", (int)var); break;      \
        case 8: (void)fprintf(p_yct_ctx_->out, "%I64d",                     \
                                               (VNUT_YCT_INT64)var); break; \
        default: break;                                                     \
    }                                                                       \
}

#endif

#define VNUT_YCT_PRINT_MAIN(main_msg)                       \
    if (p_yct_ctx_->out != NULL) {                          \
        VNUT_YCT_PRINT_STR(__FILE__);                       \
        (void)fprintf(p_yct_ctx_->out, ":%d: [", __LINE__); \
        VNUT_YCT_PRINT_STR(YCT_FUNC_NAME);                  \
        VNUT_YCT_FPUTS("]: ");                              \
        VNUT_YCT_PRINT_STR(main_msg);                       \
        VNUT_YCT_FPUTS(": ");                               \
    }

#define VNUT_YCT_PRINT(main_msg, cond) { \
    VNUT_YCT_PRINT_MAIN(main_msg);       \
    if (p_yct_ctx_->out != NULL) {       \
        VNUT_YCT_FPUTC('(');             \
        VNUT_YCT_PRINT_STR(cond);        \
        VNUT_YCT_FPUTC(')');             \
    }                                    \
}

#define VNUT_YCT_PRINT_MSG(main_msg, cond, msg) { \
    VNUT_YCT_PRINT(main_msg, cond);               \
    if (p_yct_ctx_->out != NULL) {                \
        VNUT_YCT_FPUTS(" { \"");                  \
        VNUT_YCT_PRINT_STR(msg);                  \
        VNUT_YCT_FPUTS("\" }\n");                 \
    }                                             \
}

/* Messages, Warnings, Assertions */
#define YCT_MESSAGE(msg) {          \
    p_yct_ctx_->messages++;         \
    VNUT_YCT_PRINT_MAIN("MESSAGE"); \
    if (p_yct_ctx_->out != NULL) {  \
        VNUT_YCT_FPUTS("{ \"");     \
        VNUT_YCT_PRINT_STR(msg);    \
        VNUT_YCT_FPUTS("\" }\n");   \
    }                               \
}

#ifndef VNUT_YCT_RETURN
#define VNUT_YCT_RETURN() return
#endif

#define YCT_WARNING(cond)                                                   \
    VNUT_YCT_IF_OK {                                                        \
        p_yct_ctx_->checks++;                                               \
        if (!(cond)) {                                                      \
            p_yct_ctx_->warnings++;                                         \
            VNUT_YCT_PRINT("WARNING", #cond);                               \
            if (p_yct_ctx_->out != NULL)                                    \
                VNUT_YCT_FPUTC('\n');                                       \
            if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,                         \
                                 VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) != 0)   \
            {                                                               \
                VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED); \
                p_yct_ctx_->failed = 1;                                     \
                VNUT_YCT_RETURN();                                          \
            }                                                               \
        }                                                                   \
    }

#define YCT_WARNING_MSG(cond, msg)                                          \
    VNUT_YCT_IF_OK {                                                        \
        p_yct_ctx_->checks++;                                               \
        if (!(cond)) {                                                      \
            p_yct_ctx_->warnings++;                                         \
            VNUT_YCT_PRINT_MSG("WARNING", #cond, msg);                      \
            if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,                         \
                                 VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) != 0)   \
            {                                                               \
                VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED); \
                p_yct_ctx_->failed = 1;                                     \
                VNUT_YCT_RETURN();                                          \
            }                                                               \
        }                                                                   \
    }

#define YCT_FAIL(msg)                                 \
    VNUT_YCT_IF_OK {                                  \
        VNUT_YCT_PRINT_MSG("FAILED", "FAILURE", msg); \
        p_yct_ctx_->failed = 1;                       \
        VNUT_YCT_IF_SET_BLOCKED();                    \
        VNUT_YCT_RETURN();                            \
    }

#define YCT_ASSERT(cond)                     \
    VNUT_YCT_IF_OK {                         \
        p_yct_ctx_->checks++;                \
        if (!(cond)) {                       \
            VNUT_YCT_PRINT("FAILED", #cond); \
            p_yct_ctx_->failed = 1;          \
            if (p_yct_ctx_->out != NULL)     \
                VNUT_YCT_FPUTC('\n');        \
            VNUT_YCT_IF_SET_BLOCKED();       \
            VNUT_YCT_RETURN();               \
        }                                    \
    }

#define YCT_ASSERT_MSG(cond, msg)                     \
    VNUT_YCT_IF_OK {                                  \
        p_yct_ctx_->checks++;                         \
        if (!(cond)) {                                \
            VNUT_YCT_PRINT_MSG("FAILED", #cond, msg); \
            p_yct_ctx_->failed = 1;                   \
            VNUT_YCT_IF_SET_BLOCKED();                \
            VNUT_YCT_RETURN();                        \
        }                                             \
    }

#define YCT_ASSERT_EQUAL(expected, actual)                      \
    VNUT_YCT_IF_OK {                                            \
        p_yct_ctx_->checks++;                                   \
        if (!((expected) == (actual))) {                        \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Expected: <");                \
                VNUT_YCT_PRINT_VAR(expected);                   \
                VNUT_YCT_FPUTS(">, Actual: <");                 \
                VNUT_YCT_PRINT_VAR(actual);                     \
                VNUT_YCT_FPUTS(">\n");                          \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    }

#define YCT_ASSERT_EQUAL_MSG(expected, actual, msg)             \
    VNUT_YCT_IF_OK {                                            \
        p_yct_ctx_->checks++;                                   \
        if (!((expected) == (actual))) {                        \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Expected: <");                \
                VNUT_YCT_PRINT_VAR(expected);                   \
                VNUT_YCT_FPUTS(">, Actual: <");                 \
                VNUT_YCT_PRINT_VAR(actual);                     \
                VNUT_YCT_FPUTS("> { \"");                       \
                VNUT_YCT_PRINT_STR(msg);                        \
                VNUT_YCT_FPUTS("\" }\n");                       \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    }

#define YCT_ASSERT_NOT_EQUAL(expected, actual)                  \
    VNUT_YCT_IF_OK {                                            \
        p_yct_ctx_->checks++;                                   \
        if ((expected) == (actual)) {                           \
            VNUT_YCT_PRINT("FAILED", #expected " != " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Both values were: <");        \
                VNUT_YCT_PRINT_VAR(expected);                   \
                VNUT_YCT_FPUTC('>');                            \
                VNUT_YCT_FPUTC('\n');                           \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    }

#define YCT_ASSERT_NOT_EQUAL_MSG(expected, actual, msg)         \
    VNUT_YCT_IF_OK {                                            \
        p_yct_ctx_->checks++;                                   \
        if ((expected) == (actual)) {                           \
            VNUT_YCT_PRINT("FAILED", #expected " != " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Both values were: <");        \
                VNUT_YCT_PRINT_VAR(expected);                   \
                VNUT_YCT_FPUTC('>');                            \
                VNUT_YCT_FPUTS(" { \"");                        \
                VNUT_YCT_PRINT_STR(msg);                        \
                VNUT_YCT_FPUTS("\" }\n");                       \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    }

#define YCT_ASSERT_NULL(var)                   \
    VNUT_YCT_IF_OK {                           \
        p_yct_ctx_->checks++;                  \
        if ((var) != NULL) {                   \
            VNUT_YCT_PRINT_MAIN("FAILED");     \
            if (p_yct_ctx_->out != NULL) {     \
                VNUT_YCT_FPUTC('(');           \
                VNUT_YCT_PRINT_STR(#var);      \
                VNUT_YCT_FPUTS(" == NULL)\n"); \
            }                                  \
            p_yct_ctx_->failed = 1;            \
            VNUT_YCT_IF_SET_BLOCKED();         \
            VNUT_YCT_RETURN();                 \
        }                                      \
    }

#define YCT_ASSERT_NULL_MSG(var, msg)             \
    VNUT_YCT_IF_OK {                              \
        p_yct_ctx_->checks++;                     \
        if ((var) != NULL) {                      \
            VNUT_YCT_PRINT_MAIN("FAILED");        \
            if (p_yct_ctx_->out != NULL) {        \
                VNUT_YCT_FPUTC('(');              \
                VNUT_YCT_PRINT_STR(#var);         \
                VNUT_YCT_FPUTS(" == NULL) { \""); \
                VNUT_YCT_PRINT_STR(msg);          \
                VNUT_YCT_FPUTS("\" }\n");         \
            }                                     \
            p_yct_ctx_->failed = 1;               \
            VNUT_YCT_IF_SET_BLOCKED();            \
            VNUT_YCT_RETURN();                    \
        }                                         \
    }

#define YCT_ASSERT_NOT_NULL(var)               \
    VNUT_YCT_IF_OK {                           \
        p_yct_ctx_->checks++;                  \
        if ((var) == NULL) {                   \
            VNUT_YCT_PRINT_MAIN("FAILED");     \
            if (p_yct_ctx_->out != NULL) {     \
                VNUT_YCT_FPUTC('(');           \
                VNUT_YCT_PRINT_STR(#var);      \
                VNUT_YCT_FPUTS(" != NULL)\n"); \
            }                                  \
            p_yct_ctx_->failed = 1;            \
            VNUT_YCT_IF_SET_BLOCKED();         \
            VNUT_YCT_RETURN();                 \
        }                                      \
    }

#define YCT_ASSERT_NOT_NULL_MSG(var, msg)   \
    VNUT_YCT_IF_OK {                        \
        p_yct_ctx_->checks++;               \
        if ((var) == NULL) {                \
            VNUT_YCT_PRINT_MAIN("FAILED");  \
            if (p_yct_ctx_->out != NULL) {  \
                VNUT_YCT_FPUTC('(');        \
                VNUT_YCT_PRINT_STR(#var);   \
                VNUT_YCT_FPUTS(" != NULL"); \
                VNUT_YCT_FPUTS(") { \"");   \
                VNUT_YCT_PRINT_STR(msg);    \
                VNUT_YCT_FPUTS("\" }\n");   \
            }                               \
            p_yct_ctx_->failed = 1;         \
            VNUT_YCT_IF_SET_BLOCKED();      \
            VNUT_YCT_RETURN();              \
        }                                   \
    }

#define VNUT_YCT_COMPARE_STR(str1, str2, out) {                           \
    unsigned int yct_i_; out = 0;                                         \
    for (yct_i_ = 0; str1[yct_i_] != 0 && str1[yct_i_] == str2[yct_i_];   \
         yct_i_++) ;                                                      \
    out = (int)((unsigned int)str1[yct_i_] - (unsigned int)str2[yct_i_]); \
}


#define YCT_ASSERT_EQUAL_STR(expected, actual)                  \
    VNUT_YCT_IF_OK {                                            \
        int yct_out_;                                           \
        p_yct_ctx_->checks++;                                   \
        VNUT_YCT_COMPARE_STR((expected), (actual), yct_out_);   \
        if (yct_out_ != 0) {                                    \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Expected: <");                \
                VNUT_YCT_PRINT_STR(expected);                   \
                VNUT_YCT_FPUTS(">, Actual: <");                 \
                VNUT_YCT_PRINT_STR(actual);                     \
                VNUT_YCT_FPUTS(">\n");                          \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    }

#define YCT_ASSERT_EQUAL_STR_MSG(expected, actual, msg)        \
    VNUT_YCT_IF_OK {                                           \
        int yct_out_;                                          \
        p_yct_ctx_->checks++;                                  \
        VNUT_YCT_COMPARE_STR((expected), (actual), yct_out_);  \
        if (yct_out_ != 0) {                                   \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual) \
            if (p_yct_ctx_->out != NULL) {                     \
                VNUT_YCT_FPUTS(": Expected: <");               \
                VNUT_YCT_PRINT_STR(expected);                  \
                VNUT_YCT_FPUTS(">, Actual: <");                \
                VNUT_YCT_PRINT_STR(actual);                    \
                VNUT_YCT_FPUTS("> { \"");                      \
                VNUT_YCT_PRINT_STR(msg);                       \
                VNUT_YCT_FPUTS("\" }\n");                      \
            }                                                  \
            p_yct_ctx_->failed++;                              \
            VNUT_YCT_IF_SET_BLOCKED();                         \
            VNUT_YCT_RETURN();                                 \
        }                                                      \
    }

#define YCT_ASSERT_NOT_EQUAL_STR(str1, str2)               \
    VNUT_YCT_IF_OK {                                       \
        int yct_out_;                                      \
        p_yct_ctx_->checks++;                              \
        VNUT_YCT_COMPARE_STR((str1), (str2), yct_out_);    \
        if (yct_out_ == 0) {                               \
            VNUT_YCT_PRINT("FAILED", #str1 " != " #str2)   \
            if (p_yct_ctx_->out != NULL) {                 \
                VNUT_YCT_FPUTS(": 'Both strings were: <"); \
                VNUT_YCT_PRINT_STR(str1);                  \
                VNUT_YCT_FPUTS(">'\n");                    \
            }                                              \
            p_yct_ctx_->failed = 1;                        \
            VNUT_YCT_IF_SET_BLOCKED();                     \
            VNUT_YCT_RETURN();                             \
        }                                                  \
    }

#define YCT_ASSERT_NOT_EQUAL_STR_MSG(str1, str2, msg)      \
    VNUT_YCT_IF_OK {                                       \
        int yct_out_;                                      \
        p_yct_ctx_->checks++;                              \
        VNUT_YCT_COMPARE_STR((str1), (str2), yct_out_);    \
        if (yct_out_ == 0) {                               \
            VNUT_YCT_PRINT("FAILED", #str1 " != " #str2);  \
            if (p_yct_ctx_->out != NULL) {                 \
                VNUT_YCT_FPUTS(": 'Both strings were: <"); \
                VNUT_YCT_PRINT_STR(str1);                  \
                VNUT_YCT_FPUTS(">' { \"");                 \
                VNUT_YCT_PRINT_STR(msg);                   \
                VNUT_YCT_FPUTS("\" }\n");                  \
            }                                              \
            p_yct_ctx_->failed = 1;                        \
            VNUT_YCT_IF_SET_BLOCKED();                     \
            VNUT_YCT_RETURN();                             \
        }                                                  \
    }

#define VNUT_YCT_COMPARE_ARRAY(arr1, arr2, len, out) \
    for (out = 0; out < (unsigned int)(len) && arr1[out] == arr2[out]; out++) ;

#define YCT_ASSERT_EQUAL_ARRAY(arr1, arr2, len)                              \
    VNUT_YCT_IF_OK {                                                         \
        unsigned int yct_out_;                                               \
        p_yct_ctx_->checks++;                                                \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), yct_out_);             \
        if (yct_out_ < (unsigned int)(len)) {                                \
            VNUT_YCT_PRINT("FAILED", #arr1 " == " #arr2);                    \
            if (p_yct_ctx_->out != NULL) {                                   \
                (void)fprintf(p_yct_ctx_->out,                               \
                              ": 'Element [%u] was different'\n", yct_out_); \
            }                                                                \
            p_yct_ctx_->failed = 1;                                          \
            VNUT_YCT_IF_SET_BLOCKED();                                       \
            VNUT_YCT_RETURN();                                               \
        }                                                                    \
    }

#define YCT_ASSERT_EQUAL_ARRAY_MSG(arr1, arr2, len, msg)                     \
    VNUT_YCT_IF_OK {                                                         \
        unsigned int yct_out_;                                               \
        p_yct_ctx_->checks++;                                                \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), yct_out_);             \
        if (yct_out_ < (unsigned int)(len)) {                                \
            VNUT_YCT_PRINT("FAILED", #arr1 " == " #arr2);                    \
            if (p_yct_ctx_->out != NULL) {                                   \
                (void)fprintf(p_yct_ctx_->out,                               \
                           ": 'Element [%u] was different' { \"", yct_out_); \
                VNUT_YCT_PRINT_STR(msg);                                     \
                VNUT_YCT_FPUTS("\" }\n");                                    \
            }                                                                \
            p_yct_ctx_->failed = 1;                                          \
            VNUT_YCT_IF_SET_BLOCKED();                                       \
            VNUT_YCT_RETURN();                                               \
        }                                                                    \
    }

#define YCT_ASSERT_NOT_EQUAL_ARRAY(arr1, arr2, len)              \
    VNUT_YCT_IF_OK {                                             \
        unsigned int yct_out_;                                   \
        p_yct_ctx_->checks++;                                    \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), yct_out_); \
        if (yct_out_ == (unsigned int)(len)) {                   \
            VNUT_YCT_PRINT("FAILED", #arr1 " != " #arr2);        \
            if (p_yct_ctx_->out != NULL)                         \
                VNUT_YCT_FPUTC('\n');                            \
            p_yct_ctx_->failed = 1;                              \
            VNUT_YCT_IF_SET_BLOCKED();                           \
            VNUT_YCT_RETURN();                                   \
        }                                                        \
    }

#define YCT_ASSERT_NOT_EQUAL_ARRAY_MSG(arr1, arr2, len, msg)       \
    VNUT_YCT_IF_OK {                                               \
        unsigned int yct_out_;                                     \
        p_yct_ctx_->checks++;                                      \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), yct_out_);   \
        if (yct_out_ == (unsigned int)(len)) {                     \
            VNUT_YCT_PRINT_MSG("FAILED", #arr1 " != " #arr2, msg); \
            p_yct_ctx_->failed = 1;                                \
            VNUT_YCT_IF_SET_BLOCKED();                             \
            VNUT_YCT_RETURN();                                     \
        }                                                          \
    }

#define YCT_ASSERT_EQUAL_OBJ(o1, o2, fcmp) \
    VNUT_YCT_IF_OK {                       \
        p_yct_ctx_->checks++;              \
        if (fcmp(&o1, &o2) != 0) {         \
            VNUT_YCT_PRINT_MAIN("FAILED"); \
            if (p_yct_ctx_->out != NULL) { \
                VNUT_YCT_FPUTC('(');       \
                VNUT_YCT_PRINT_STR(#o1);   \
                VNUT_YCT_FPUTS(" == ");    \
                VNUT_YCT_PRINT_STR(#o2);   \
                VNUT_YCT_FPUTS(")\n");     \
            }                              \
            p_yct_ctx_->failed = 1;        \
            VNUT_YCT_IF_SET_BLOCKED();     \
            VNUT_YCT_RETURN();             \
        }                                  \
    }

#define YCT_ASSERT_EQUAL_OBJ_MSG(o1, o2, fcmp, msg) \
    VNUT_YCT_IF_OK {                                \
        p_yct_ctx_->checks++;                       \
        if (fcmp(&o1, &o2) != 0) {                  \
            VNUT_YCT_PRINT_MAIN("FAILED");          \
            if (p_yct_ctx_->out != NULL) {          \
                VNUT_YCT_FPUTC('(');                \
                VNUT_YCT_PRINT_STR(#o1);            \
                VNUT_YCT_FPUTS(" == ");             \
                VNUT_YCT_PRINT_STR(#o2);            \
                VNUT_YCT_FPUTS(") { \"");           \
                VNUT_YCT_PRINT_STR(msg);            \
                VNUT_YCT_FPUTS("\" }\n");           \
            }                                       \
            p_yct_ctx_->failed = 1;                 \
            VNUT_YCT_IF_SET_BLOCKED();              \
            VNUT_YCT_RETURN();                      \
        }                                           \
    }

#define YCT_ASSERT_NOT_EQUAL_OBJ(o1, o2, fcmp) \
    VNUT_YCT_IF_OK {                           \
        p_yct_ctx_->checks++;                  \
        if (fcmp(&o1, &o2) == 0) {             \
            VNUT_YCT_PRINT_MAIN("FAILED");     \
            if (p_yct_ctx_->out != NULL) {     \
                VNUT_YCT_FPUTC('(');           \
                VNUT_YCT_PRINT_STR(#o1);       \
                VNUT_YCT_FPUTS(" != ");        \
                VNUT_YCT_PRINT_STR(#o2);       \
                VNUT_YCT_FPUTS(")\n");         \
            }                                  \
            p_yct_ctx_->failed = 1;            \
            VNUT_YCT_IF_SET_BLOCKED();         \
            VNUT_YCT_RETURN();                 \
        }                                      \
    }

#define YCT_ASSERT_NOT_EQUAL_OBJ_MSG(o1, o2, fcmp, msg) \
    VNUT_YCT_IF_OK {                                    \
        p_yct_ctx_->checks++;                           \
        if (fcmp(&o1, &o2) == 0) {                      \
            VNUT_YCT_PRINT_MAIN("FAILED");              \
            if (p_yct_ctx_->out != NULL) {              \
                VNUT_YCT_FPUTC('(');                    \
                VNUT_YCT_PRINT_STR(#o1);                \
                VNUT_YCT_FPUTS(" != ");                 \
                VNUT_YCT_PRINT_STR(#o2);                \
                VNUT_YCT_FPUTS(") { \"");               \
                VNUT_YCT_PRINT_STR(msg);                \
                VNUT_YCT_FPUTS("\" }\n");               \
            }                                           \
            p_yct_ctx_->failed = 1;                     \
            VNUT_YCT_IF_SET_BLOCKED();                  \
            VNUT_YCT_RETURN();                          \
        }                                               \
    }

#define YCT_ASSERT_EQUAL_FLOAT(a, b, t)             \
    VNUT_YCT_IF_OK {                                \
        p_yct_ctx_->checks++;                       \
        if (VNUT_YCT_ABS((a) - (b)) > t) {          \
            VNUT_YCT_PRINT("FAILED", #a " == " #b); \
            if (p_yct_ctx_->out != NULL)            \
                VNUT_YCT_FPUTC('\n');               \
            p_yct_ctx_->failed = 1;                 \
            VNUT_YCT_IF_SET_BLOCKED();              \
            VNUT_YCT_RETURN();                      \
        }                                           \
    }

#define YCT_ASSERT_EQUAL_FLOAT_MSG(a, b, t, msg)             \
    VNUT_YCT_IF_OK {                                         \
        p_yct_ctx_->checks++;                                \
        if (VNUT_YCT_ABS((a) - (b)) > t) {                   \
            VNUT_YCT_PRINT_MSG("FAILED", #a " == " #b, msg); \
            p_yct_ctx_->failed = 1;                          \
            VNUT_YCT_IF_SET_BLOCKED();                       \
            VNUT_YCT_RETURN();                               \
        }                                                    \
    }

#define YCT_ASSERT_NOT_EQUAL_FLOAT(a, b, t)         \
    VNUT_YCT_IF_OK {                                \
        p_yct_ctx_->checks++;                       \
        if (VNUT_YCT_ABS((a) - (b)) > t) { } else { \
            VNUT_YCT_PRINT("FAILED", #a " != " #b); \
            if (p_yct_ctx_->out != NULL)            \
                VNUT_YCT_FPUTC('\n');               \
            p_yct_ctx_->failed = 1;                 \
            VNUT_YCT_IF_SET_BLOCKED();              \
            VNUT_YCT_RETURN();                      \
        }                                           \
    }

#define YCT_ASSERT_NOT_EQUAL_FLOAT_MSG(a, b, t, msg)         \
    VNUT_YCT_IF_OK {                                         \
        p_yct_ctx_->checks++;                                \
        if (VNUT_YCT_ABS((a) - (b)) > t) { } else {          \
            VNUT_YCT_PRINT_MSG("FAILED", #a " != " #b, msg); \
            p_yct_ctx_->failed = 1;                          \
            VNUT_YCT_IF_SET_BLOCKED();                       \
            VNUT_YCT_RETURN();                               \
        }                                                    \
    }

#endif /* YACUT_H_ */
