/*
 *  yaCut v2.0 - Yet Another C Unit Test
 *  Copyright (c) 2016 - Michele Pes
 *
 *  Distributed under the BSD License
 *  See accompanying file LICENSE.txt or copy at
 *  http://opensource.org/licenses/BSD-3-Clause
 */

#ifndef YACUT_H_
#define YACUT_H_

#include <stdio.h>
#ifndef YCT_OPT_DISABLE_TIMING
#include <time.h>
#endif

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

#define VNUT_YCT_FLAGS_BLOCKING_MODE        0x1
#define VNUT_YCT_FLAGS_FULL_BLOCKING_MODE   0x2
#define VNUT_YCT_FLAGS_LOCKED               0x4
#define VNUT_YCT_FLAGS_DISABLED             0x8
#define VNUT_YCT_FLAGS_IS_SUITE             0x10
#define VNUT_YCT_FLAGS_LOG                  0x20
#define VNUT_YCT_FLAGS_DISABLED_TIMING      0x40
#define VNUT_YCT_FLAGS_LAST_FAILED          0x80

#define YCT_GET_NAME()      "yaCut"
#define YCT_VERSION_MAJOR() 2
#define YCT_VERSION_MINOR() 0

struct yct_context {
    const char* msg;
    FILE* out;
#ifndef YCT_OPT_DISABLE_TIMING
    clock_t clocks;
#endif
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
#define VNUT_YCT_SET_BIT(var, mask) do { VNUT_YCT_CLEAR_BIT((var), (mask)); \
                                         (var) |= (mask); } while (0)
#define VNUT_YCT_COPY_BIT(src, dest, mask) \
    VNUT_YCT_SET_BIT((dest), VNUT_YCT_GET_BIT((src), (mask)));

#define VNUT_YCT_ABS(a) ((a) < (-(a)) ? -(a) : (a))

#define YCT_DEFAULT_OUTPUT (stdout)

#define YCT_SET_OUTPUT(handle) (yct_main_ctx_.out = (handle))

#define YCT_CURRENT_CONTEXT() (*p_yct_ctx_)

#define YCT_MERGE_CONTEXT(ctx_dst, ctx_src) do {                            \
    ctx_dst.suites += ctx_src.suites; ctx_dst.messages += ctx_src.messages; \
    ctx_dst.tests += ctx_src.tests; ctx_dst.checks += ctx_src.checks;       \
    ctx_dst.warnings += ctx_src.warnings; ctx_dst.failed += ctx_src.failed; \
} while (0)

#define YCT_GET_RETURN_VALUE(val) do { \
    if (p_yct_ctx_->failed > 0)        \
        val = 2;                       \
    else {                             \
        if (p_yct_ctx_->warnings > 0)  \
            val = 1;                   \
        else                           \
            val = 0;                   \
    }                                  \
} while (0)

#define YCT_LAST_FAILED() VNUT_YCT_GET_BIT(p_yct_ctx_->flags, \
                                           VNUT_YCT_FLAGS_LAST_FAILED)

// Enable/Disable
#define YCT_ENABLE() \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED)

#define YCT_DISABLE() \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED)

#define YCT_IS_ENABLED() \
    (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED) == 0)

// Blocking
#define YCT_SET_BLOCKED() do {                                         \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                              \
                       VNUT_YCT_FLAGS_FULL_BLOCKING_MODE);             \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE); \
} while (0)

#define YCT_SET_FULL_BLOCKED() do {                                         \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE);    \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); \
} while (0)

#define YCT_DISABLE_BLOCKING() do {                                           \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE |      \
                                          VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); \
} while (0)

#define YCT_IS_LOCKED() \
    VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED)

#define VNUT_YCT_IF_OK if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, \
    VNUT_YCT_FLAGS_DISABLED | VNUT_YCT_FLAGS_LOCKED) == 0)

#define VNUT_YCT_IF_SET_BLOCKED() if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,   \
    VNUT_YCT_FLAGS_BLOCKING_MODE | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) != 0) \
    { VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED); }


// Timing
#ifdef YCT_OPT_DISABLE_TIMING
#pragma message("yaCut: TIMING DISABLED")
#define YCT_DISABLE_TIMING()
#define VNUT_YCT_GET_START_TIME()
#define VNUT_YCT_PRINT_ELAPSED_TIME()
#else
#define YCT_DISABLE_TIMING() \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED_TIMING)
#define VNUT_YCT_GET_START_TIME() (yct_main_ctx_.clocks = clock())
#define VNUT_YCT_PRINT_ELAPSED_TIME()                                        \
    if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED_TIMING)  \
        == 0) { (void)fprintf(yct_main_ctx_.out, " (%.2f seconds)", (double) \
                (clock() - yct_main_ctx_.clocks) / (double)CLOCKS_PER_SEC); }
#endif

// Log
#ifdef YCT_OPT_DISABLE_LOG
#pragma message("yaCut: LOG DISABLED")
#define YCT_ENABLE_LOG()
#define YCT_DISABLE_LOG()
#define YCT_IS_LOG_ENABLED() 0
#else
#define YCT_ENABLE_LOG() \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOG)

#define YCT_DISABLE_LOG() \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOG)

#define YCT_IS_LOG_ENABLED() \
    VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOG)
#endif

// General
#define YCT_BEGIN(name) {                                  \
    struct yct_context yct_main_ctx_;                      \
    struct yct_context* const p_yct_ctx_ = &yct_main_ctx_; \
    VNUT_YCT_INIT_DATA(yct_main_ctx_);                     \
    yct_main_ctx_.out = YCT_DEFAULT_OUTPUT;                \
    yct_main_ctx_.msg = (const char*)(name);               \
    VNUT_YCT_GET_START_TIME();

#define YCT_TEST(test_name) \
    static void test_name(struct yct_context* const p_yct_ctx_)

#define DISABLE_YCT_TEST(test_name) YCT_TEST(test_name) {  \
    if (p_yct_ctx_->checks == p_yct_ctx_->flags)           \
        p_yct_ctx_->checks = p_yct_ctx_->flags; return; }  \
    void test_name##_YCT_DISABLED_TEST_(struct yct_context* const p_yct_ctx_)

#define YCT_SUITE(suite_name, setup, teardown)                     \
    static void suite_name(struct yct_context* const p_yct_ctx_) { \
        void (*f_setup_)(void) = setup;                            \
        void (*f_tear_down_)(void) = teardown;                     \
        if (f_setup_ == f_tear_down_)                              \
            VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_IS_SUITE);

#define YCT_SUITE_END \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_IS_SUITE); return; }

#define DISABLE_YCT_SUITE(suite_name, setup, teardown)                      \
    YCT_SUITE(suite_name, setup, teardown) YCT_SUITE_END                    \
    void                                                                    \
    test_name##_YCT_DISABLED_SUITE_(struct yct_context* const p_yct_ctx_) { \
        void (*f_setup_)(void) = setup; void (*f_tear_down_)(void) = teardown;

#define YCT_TEST_RUN(test_name)                                              \
    VNUT_YCT_IF_OK {                                                         \
        struct yct_context yct_ctx_;                                         \
        VNUT_YCT_INIT_DATA(yct_ctx_);                                        \
        yct_ctx_.out = p_yct_ctx_->out;                                      \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,                 \
                          VNUT_YCT_FLAGS_BLOCKING_MODE                       \
                          | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE                \
                          | VNUT_YCT_FLAGS_LOG);                             \
        VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED);   \
        test_name(&yct_ctx_);                                                \
        p_yct_ctx_->tests++;                                                 \
        p_yct_ctx_->warnings += yct_ctx_.warnings;                           \
        p_yct_ctx_->checks += yct_ctx_.checks;                               \
        p_yct_ctx_->messages += yct_ctx_.messages;                           \
        if (yct_ctx_.failed > 0) {                                           \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,             \
                              VNUT_YCT_FLAGS_LOCKED);                        \
            p_yct_ctx_->failed++;                                            \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED); \
        }                                                                    \
    }

#define NO_YCT_TEST_RUN(test_name) (void)(test_name)

#define YCT_TEST_ADD(yct_name)                                               \
    VNUT_YCT_IF_OK {                                                         \
        struct yct_context yct_ctx_;                                         \
        VNUT_YCT_INIT_DATA(yct_ctx_);                                        \
        yct_ctx_.out = p_yct_ctx_->out;                                      \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,                 \
                          VNUT_YCT_FLAGS_BLOCKING_MODE                       \
                          | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE                \
                          | VNUT_YCT_FLAGS_LOG);                             \
        VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED);   \
        if (f_setup_ != NULL)                                                \
            (*f_setup_)();                                                   \
        yct_name(&yct_ctx_);                                                 \
        if (f_tear_down_ != NULL)                                            \
            (*f_tear_down_)();                                               \
        p_yct_ctx_->tests++;                                                 \
        p_yct_ctx_->warnings += yct_ctx_.warnings;                           \
        p_yct_ctx_->checks += yct_ctx_.checks;                               \
        p_yct_ctx_->messages += yct_ctx_.messages;                           \
        if (yct_ctx_.failed > 0) {                                           \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,             \
                              VNUT_YCT_FLAGS_LOCKED);                        \
            p_yct_ctx_->failed++;                                            \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED); \
        }                                                                    \
    }

#define NO_YCT_TEST_ADD(test_name)

#define NO_YCT_SUITE_RUN(suite_name) (void)(suite_name)

#define YCT_SUITE_RUN(yct_name)                               \
    VNUT_YCT_IF_OK {                                          \
        struct yct_context yct_ctx_;                          \
        VNUT_YCT_INIT_DATA(yct_ctx_);                         \
        yct_ctx_.out = p_yct_ctx_->out;                       \
        VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                 \
                           VNUT_YCT_FLAGS_LAST_FAILED);       \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,  \
                          VNUT_YCT_FLAGS_BLOCKING_MODE        \
                          | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE \
                          | VNUT_YCT_FLAGS_LOG);              \
        yct_name(&yct_ctx_);                                  \
        VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,  \
                          VNUT_YCT_FLAGS_LOCKED);             \
        if (yct_ctx_.failed > 0) {                            \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags,               \
                             VNUT_YCT_FLAGS_LAST_FAILED); }   \
        p_yct_ctx_->suites++;                                 \
        p_yct_ctx_->tests += yct_ctx_.tests;                  \
        p_yct_ctx_->failed += yct_ctx_.failed;                \
        p_yct_ctx_->warnings += yct_ctx_.warnings;            \
        p_yct_ctx_->checks += yct_ctx_.checks;                \
        p_yct_ctx_->messages += yct_ctx_.messages;            \
    }

#define YCT_END() }

#define YCT_MAIN(epf) int main(void) { YCT_BEGIN(""); epf(&yct_main_ctx_); \
    if (VNUT_YCT_GET_BIT(yct_main_ctx_.flags, VNUT_YCT_FLAGS_IS_SUITE))    \
        yct_main_ctx_.suites++; else yct_main_ctx_.tests++;                \
    YCT_DUMP(); YCT_END(); return 0; }

// Parallel
#ifndef YCT_OPT_ENABLE_PARALLEL
#define YCT_PARALLEL()
#define YCT_SCHEDULE_TEST()
#define YCT_PARALLEL_TEST(yct_name) YCT_TEST_RUN(yct_name)
#define YCT_JOIN_TEST()
#define YCT_COLLECT_TEST_RESULTS()
#else
#pragma message("yaCut: PARALLEL ENABLED")
#ifdef _MSC_VER
#define YCT_PARALLEL() __pragma(omp parallel sections)
#define YCT_SCHEDULE_TEST() __pragma(omp section)
#define YCT_JOIN_TEST() __pragma(omp critical)
#else
#define YCT_PARALLEL() _Pragma("omp parallel sections")
#define YCT_SCHEDULE_TEST() _Pragma("omp section")
#define YCT_JOIN_TEST() _Pragma("omp critical")
#endif

#define YCT_PARALLEL_TEST(test_name) { VNUT_YCT_IF_OK {    \
    struct yct_context yct_ctx_;                           \
    VNUT_YCT_INIT_DATA(yct_ctx_);                          \
    yct_ctx_.out = p_yct_ctx_->out;                        \
    VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,   \
                      VNUT_YCT_FLAGS_BLOCKING_MODE         \
                      | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE  \
                      | VNUT_YCT_FLAGS_LOG);               \
    test_name(&yct_ctx_);

#define YCT_COLLECT_TEST_RESULTS() {                  \
        p_yct_ctx_->tests++;                          \
        p_yct_ctx_->warnings += yct_ctx_.warnings;    \
        p_yct_ctx_->checks += yct_ctx_.checks;        \
        p_yct_ctx_->messages += yct_ctx_.messages;    \
        if (yct_ctx_.failed > 0) {                    \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags,         \
                              p_yct_ctx_->flags,      \
                              VNUT_YCT_FLAGS_LOCKED); \
            p_yct_ctx_->failed++;                     \
        } } } }
#endif

#define VNUT_YCT_FPUTC(c) ((void)fputc((c), p_yct_ctx_->out))
#define VNUT_YCT_FPUTS(str) ((void)fprintf(p_yct_ctx_->out, str))
#define VNUT_YCT_PRINT_STR(str) \
    do { (void)fputs((void*)(str), p_yct_ctx_->out); } while (0)

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
    const char* yct_title = p_yct_ctx_->msg;                                 \
    const int yct_has_label_ = yct_title != NULL && *yct_title != '\0';      \
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
    VNUT_YCT_PRINT_ELAPSED_TIME(); if (p_yct_ctx_->failed > 0)               \
        VNUT_YCT_FPUTS("\n------> SOMETHING FAILED <-----\n");               \
    else {                                                                   \
        if (p_yct_ctx_->warnings > 0)                                        \
            VNUT_YCT_FPUTS("\n-------> SOME WARNING <--------\n");           \
        else                                                                 \
            VNUT_YCT_FPUTS("\n----------> ALL OK <-----------\n");           \
    }                                                                        \
}

// Internal prints
#ifdef YCT_OPT_DISABLE_INT64

#pragma message("yaCut: INT64 DISABLED")

#define VNUT_YCT_PRINT_VAR(var) do {                                     \
    const size_t yct_pv_len_ = sizeof(var);                              \
    switch(yct_pv_len_) {                                                \
        case 1: (void)fprintf(p_yct_ctx_->out, "%c",  (char)var); break; \
        case 2: (void)fprintf(p_yct_ctx_->out, "%d",  (int) var); break; \
        case 4: (void)fprintf(p_yct_ctx_->out, "%ld", (long)var); break; \
        default: break;                                                  \
    }                                                                    \
} while (0)

#else

#ifdef __BORLANDC__
#define VNUT_YCT_INT64 __int64
#else
#define VNUT_YCT_INT64 long long
#endif

#define VNUT_YCT_PRINT_VAR(var) do {                                        \
    const size_t yct_pv_len_ = sizeof(var);                                 \
    switch(yct_pv_len_) {                                                   \
        case 1: (void)fprintf(p_yct_ctx_->out, "%c",  (char) var); break;   \
        case 2: (void)fprintf(p_yct_ctx_->out, "%hd", (short)var); break;   \
        case 4: (void)fprintf(p_yct_ctx_->out, "%d",  (int)  var); break;   \
        case 8: (void)fprintf(p_yct_ctx_->out, "%I64d",                     \
                                               (VNUT_YCT_INT64)var); break; \
        default: break;                                                     \
    }                                                                       \
} while (0)

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

#define VNUT_YCT_PRINT(main_msg, cond) do { \
    VNUT_YCT_PRINT_MAIN(main_msg);          \
    if (p_yct_ctx_->out != NULL) {          \
        VNUT_YCT_FPUTC('(');                \
        VNUT_YCT_PRINT_STR(cond);           \
        VNUT_YCT_FPUTC(')');                \
    }                                       \
} while (0)

#define VNUT_YCT_PRINT_MSG(main_msg, cond, msg) do { \
    VNUT_YCT_PRINT(main_msg, cond);                  \
    if (p_yct_ctx_->out != NULL) {                   \
        VNUT_YCT_FPUTS(" { \"");                     \
        VNUT_YCT_PRINT_STR(msg);                     \
        VNUT_YCT_FPUTS("\" }\n");                    \
    }                                                \
} while (0)

#ifdef YCT_OPT_DISABLE_LOG
#define VNUT_YCT_LOG(cond)
#else
#define VNUT_YCT_LOG(cond)                                         \
    if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOG)) { \
        VNUT_YCT_PRINT("LOG", #cond);                              \
        if (p_yct_ctx_->out != NULL)                               \
            VNUT_YCT_FPUTC('\n');                                  \
    }
#endif

// Messages, Warnings, Assertions
#define YCT_MESSAGE(msg) do {       \
    p_yct_ctx_->messages++;         \
    VNUT_YCT_PRINT_MAIN("MESSAGE"); \
    if (p_yct_ctx_->out != NULL) {  \
        VNUT_YCT_FPUTS("{ \"");     \
        VNUT_YCT_PRINT_STR(msg);    \
        VNUT_YCT_FPUTS("\" }\n");   \
    }                               \
} while (0)

#ifndef VNUT_YCT_RETURN
#define VNUT_YCT_RETURN() return
#endif

#define YCT_WARNING(cond)                                                   \
    VNUT_YCT_IF_OK {                                                        \
        p_yct_ctx_->checks++;                                               \
        VNUT_YCT_LOG(cond);                                                 \
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
        VNUT_YCT_LOG(cond);                                                 \
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
        VNUT_YCT_LOG(cond);                  \
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
        VNUT_YCT_LOG(cond);                           \
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
        VNUT_YCT_LOG(expected == actual);                       \
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
        VNUT_YCT_LOG(expected == actual);                       \
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

#define YCT_ASSERT_NOT_EQUAL(v1, v2)                     \
    VNUT_YCT_IF_OK {                                     \
        p_yct_ctx_->checks++;                            \
        VNUT_YCT_LOG(v1 != v2);                          \
        if ((v1) == (v2)) {                              \
            VNUT_YCT_PRINT("FAILED", #v1 " != " #v2);    \
            if (p_yct_ctx_->out != NULL) {               \
                VNUT_YCT_FPUTS(": Both values were: <"); \
                VNUT_YCT_PRINT_VAR(v1);                  \
                VNUT_YCT_FPUTS(">\n");                   \
            }                                            \
            p_yct_ctx_->failed = 1;                      \
            VNUT_YCT_IF_SET_BLOCKED();                   \
            VNUT_YCT_RETURN();                           \
        }                                                \
    }

#define YCT_ASSERT_NOT_EQUAL_MSG(v1, v2, msg)            \
    VNUT_YCT_IF_OK {                                     \
        p_yct_ctx_->checks++;                            \
        VNUT_YCT_LOG(v1 != v2);                          \
        if ((v1) == (v2)) {                              \
            VNUT_YCT_PRINT("FAILED", #v1 " != " #v2);    \
            if (p_yct_ctx_->out != NULL) {               \
                VNUT_YCT_FPUTS(": Both values were: <"); \
                VNUT_YCT_PRINT_VAR(v1);                  \
                VNUT_YCT_FPUTS("> { \"");                \
                VNUT_YCT_PRINT_STR(msg);                 \
                VNUT_YCT_FPUTS("\" }\n");                \
            }                                            \
            p_yct_ctx_->failed = 1;                      \
            VNUT_YCT_IF_SET_BLOCKED();                   \
            VNUT_YCT_RETURN();                           \
        }                                                \
    }

#define YCT_ASSERT_NULL(var)                   \
    VNUT_YCT_IF_OK {                           \
        p_yct_ctx_->checks++;                  \
        VNUT_YCT_LOG(var == NULL);             \
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
        VNUT_YCT_LOG(var == NULL);                \
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
        VNUT_YCT_LOG(var != NULL);             \
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
        VNUT_YCT_LOG(var != NULL);          \
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

#define VNUT_YCT_COMPARE_STR(str1, str2, out) do {                        \
    unsigned int yct_i_; out = 0;                                         \
    for (yct_i_ = 0; str1[yct_i_] != 0 && str1[yct_i_] == str2[yct_i_];   \
         yct_i_++) { }                                                    \
    out = (int)((unsigned int)str1[yct_i_] - (unsigned int)str2[yct_i_]); \
} while (0)

#define YCT_ASSERT_EQUAL_STR(expected, actual)                  \
    VNUT_YCT_IF_OK {                                            \
        int yct_out_;                                           \
        p_yct_ctx_->checks++;                                   \
        VNUT_YCT_LOG(expected == actual);                       \
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

#define YCT_ASSERT_EQUAL_STR_MSG(expected, actual, msg)         \
    VNUT_YCT_IF_OK {                                            \
        int yct_out_;                                           \
        p_yct_ctx_->checks++;                                   \
        VNUT_YCT_LOG(expected == actual);                       \
        VNUT_YCT_COMPARE_STR((expected), (actual), yct_out_);   \
        if (yct_out_ != 0) {                                    \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Expected: <");                \
                VNUT_YCT_PRINT_STR(expected);                   \
                VNUT_YCT_FPUTS(">, Actual: <");                 \
                VNUT_YCT_PRINT_STR(actual);                     \
                VNUT_YCT_FPUTS("> { \"");                       \
                VNUT_YCT_PRINT_STR(msg);                        \
                VNUT_YCT_FPUTS("\" }\n");                       \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    }

#define YCT_ASSERT_NOT_EQUAL_STR(str1, str2)               \
    VNUT_YCT_IF_OK {                                       \
        int yct_out_;                                      \
        p_yct_ctx_->checks++;                              \
        VNUT_YCT_LOG(str1 != str2);                        \
        VNUT_YCT_COMPARE_STR((str1), (str2), yct_out_);    \
        if (yct_out_ == 0) {                               \
            VNUT_YCT_PRINT("FAILED", #str1 " != " #str2);  \
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
        VNUT_YCT_LOG(str1 != str2);                        \
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

#define VNUT_YCT_COMPARE_ARRAY(arr1, arr2, len, out) do { for (out = 0; \
    out < (unsigned int)(len) && arr1[out] == arr2[out]; out++) ; } while (0)

#define YCT_ASSERT_EQUAL_ARRAY(expected, actual, len)                  \
    VNUT_YCT_IF_OK {                                                   \
        unsigned int yct_out_;                                         \
        p_yct_ctx_->checks++;                                          \
        VNUT_YCT_LOG(expected == actual);                              \
        VNUT_YCT_COMPARE_ARRAY((expected), (actual), (len), yct_out_); \
        if (yct_out_ < (unsigned int)(len)) {                          \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual);        \
            if (p_yct_ctx_->out != NULL) {                             \
                (void)fprintf(p_yct_ctx_->out,                         \
                      ": 'Element [%u] was different'\n", yct_out_);   \
            }                                                          \
            p_yct_ctx_->failed = 1;                                    \
            VNUT_YCT_IF_SET_BLOCKED();                                 \
            VNUT_YCT_RETURN();                                         \
        }                                                              \
    }

#define YCT_ASSERT_EQUAL_ARRAY_MSG(expected, actual, len, msg)          \
    VNUT_YCT_IF_OK {                                                    \
        unsigned int yct_out_;                                          \
        p_yct_ctx_->checks++;                                           \
        VNUT_YCT_LOG(expected == actual);                               \
        VNUT_YCT_COMPARE_ARRAY((expected), (actual), (len), yct_out_);  \
        if (yct_out_ < (unsigned int)(len)) {                           \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual);         \
            if (p_yct_ctx_->out != NULL) {                              \
                (void)fprintf(p_yct_ctx_->out,                          \
                      ": 'Element [%u] was different' { \"", yct_out_); \
                VNUT_YCT_PRINT_STR(msg);                                \
                VNUT_YCT_FPUTS("\" }\n");                               \
            }                                                           \
            p_yct_ctx_->failed = 1;                                     \
            VNUT_YCT_IF_SET_BLOCKED();                                  \
            VNUT_YCT_RETURN();                                          \
        }                                                               \
    }

#define YCT_ASSERT_NOT_EQUAL_ARRAY(arr1, arr2, len)              \
    VNUT_YCT_IF_OK {                                             \
        unsigned int yct_out_;                                   \
        p_yct_ctx_->checks++;                                    \
        VNUT_YCT_LOG(arr1 != arr2);                              \
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
        VNUT_YCT_LOG(arr1 != arr2);                                \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), yct_out_);   \
        if (yct_out_ == (unsigned int)(len)) {                     \
            VNUT_YCT_PRINT_MSG("FAILED", #arr1 " != " #arr2, msg); \
            p_yct_ctx_->failed = 1;                                \
            VNUT_YCT_IF_SET_BLOCKED();                             \
            VNUT_YCT_RETURN();                                     \
        }                                                          \
    }

#define YCT_ASSERT_EQUAL_OBJ(expected, actual, fcmp) \
    VNUT_YCT_IF_OK {                                 \
        p_yct_ctx_->checks++;                        \
        VNUT_YCT_LOG(expected == actual);            \
        if (fcmp(&expected, &actual) != 0) {         \
            VNUT_YCT_PRINT_MAIN("FAILED");           \
            if (p_yct_ctx_->out != NULL) {           \
                VNUT_YCT_FPUTC('(');                 \
                VNUT_YCT_PRINT_STR(#expected);       \
                VNUT_YCT_FPUTS(" == ");              \
                VNUT_YCT_PRINT_STR(#actual);         \
                VNUT_YCT_FPUTS(")\n");               \
            }                                        \
            p_yct_ctx_->failed = 1;                  \
            VNUT_YCT_IF_SET_BLOCKED();               \
            VNUT_YCT_RETURN();                       \
        }                                            \
    }

#define YCT_ASSERT_EQUAL_OBJ_MSG(expected, actual, fcmp, msg) \
    VNUT_YCT_IF_OK {                                          \
        p_yct_ctx_->checks++;                                 \
        VNUT_YCT_LOG(expected == actual);                     \
        if (fcmp(&expected, &actual) != 0) {                  \
            VNUT_YCT_PRINT_MAIN("FAILED");                    \
            if (p_yct_ctx_->out != NULL) {                    \
                VNUT_YCT_FPUTC('(');                          \
                VNUT_YCT_PRINT_STR(#expected);                \
                VNUT_YCT_FPUTS(" == ");                       \
                VNUT_YCT_PRINT_STR(#actual);                  \
                VNUT_YCT_FPUTS(") { \"");                     \
                VNUT_YCT_PRINT_STR(msg);                      \
                VNUT_YCT_FPUTS("\" }\n");                     \
            }                                                 \
            p_yct_ctx_->failed = 1;                           \
            VNUT_YCT_IF_SET_BLOCKED();                        \
            VNUT_YCT_RETURN();                                \
        }                                                     \
    }

#define YCT_ASSERT_NOT_EQUAL_OBJ(obj1, obj2, fcmp) \
    VNUT_YCT_IF_OK {                               \
        p_yct_ctx_->checks++;                      \
        VNUT_YCT_LOG(obj1 != obj2);                \
        if (fcmp(&obj1, &obj2) == 0) {             \
            VNUT_YCT_PRINT_MAIN("FAILED");         \
            if (p_yct_ctx_->out != NULL) {         \
                VNUT_YCT_FPUTC('(');               \
                VNUT_YCT_PRINT_STR(#obj1);         \
                VNUT_YCT_FPUTS(" != ");            \
                VNUT_YCT_PRINT_STR(#obj2);         \
                VNUT_YCT_FPUTS(")\n");             \
            }                                      \
            p_yct_ctx_->failed = 1;                \
            VNUT_YCT_IF_SET_BLOCKED();             \
            VNUT_YCT_RETURN();                     \
        }                                          \
    }

#define YCT_ASSERT_NOT_EQUAL_OBJ_MSG(obj1, obj2, fcmp, msg) \
    VNUT_YCT_IF_OK {                                        \
        p_yct_ctx_->checks++;                               \
        VNUT_YCT_LOG(obj1 != obj2);                         \
        if (fcmp(&obj1, &obj2) == 0) {                      \
            VNUT_YCT_PRINT_MAIN("FAILED");                  \
            if (p_yct_ctx_->out != NULL) {                  \
                VNUT_YCT_FPUTC('(');                        \
                VNUT_YCT_PRINT_STR(#obj1);                  \
                VNUT_YCT_FPUTS(" != ");                     \
                VNUT_YCT_PRINT_STR(#obj2);                  \
                VNUT_YCT_FPUTS(") { \"");                   \
                VNUT_YCT_PRINT_STR(msg);                    \
                VNUT_YCT_FPUTS("\" }\n");                   \
            }                                               \
            p_yct_ctx_->failed = 1;                         \
            VNUT_YCT_IF_SET_BLOCKED();                      \
            VNUT_YCT_RETURN();                              \
        }                                                   \
    }

#define YCT_ASSERT_EQUAL_FLOAT(expected_float, actual_float, tolerance)     \
    VNUT_YCT_IF_OK {                                                        \
        p_yct_ctx_->checks++;                                               \
        VNUT_YCT_LOG(expected_float == actual_float);                       \
        if (VNUT_YCT_ABS((expected_float) - (actual_float)) > tolerance) {  \
            VNUT_YCT_PRINT("FAILED", #expected_float " == " #actual_float); \
            if (p_yct_ctx_->out != NULL)                                    \
                VNUT_YCT_FPUTC('\n');                                       \
            p_yct_ctx_->failed = 1;                                         \
            VNUT_YCT_IF_SET_BLOCKED();                                      \
            VNUT_YCT_RETURN();                                              \
        }                                                                   \
    }

#define YCT_ASSERT_EQUAL_FLOAT_MSG(exp_float, act_float, tol, msg)           \
    VNUT_YCT_IF_OK {                                                         \
        p_yct_ctx_->checks++;                                                \
        VNUT_YCT_LOG(exp_float == act_float);                                \
        if (VNUT_YCT_ABS((exp_float) - (act_float)) > tol) {                 \
            VNUT_YCT_PRINT_MSG("FAILED", #exp_float " == " #act_float, msg); \
            p_yct_ctx_->failed = 1;                                          \
            VNUT_YCT_IF_SET_BLOCKED();                                       \
            VNUT_YCT_RETURN();                                               \
        }                                                                    \
    }

#define YCT_ASSERT_NOT_EQUAL_FLOAT(a, b, t)         \
    VNUT_YCT_IF_OK {                                \
        p_yct_ctx_->checks++;                       \
        VNUT_YCT_LOG(a != b);                       \
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
        VNUT_YCT_LOG(a != b);                                \
        if (VNUT_YCT_ABS((a) - (b)) > t) { } else {          \
            VNUT_YCT_PRINT_MSG("FAILED", #a " != " #b, msg); \
            p_yct_ctx_->failed = 1;                          \
            VNUT_YCT_IF_SET_BLOCKED();                       \
            VNUT_YCT_RETURN();                               \
        }                                                    \
    }

#endif // YACUT_H_
