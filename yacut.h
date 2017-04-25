/*
 *  yaCut v2.0.2 - Yet Another C Unit Test
 *  Copyright (c) 2017 - Michele Pes
 *
 *  Distributed under the BSD License
 *  See accompanying file LICENSE.txt or copy at
 *  http://opensource.org/licenses/BSD-3-Clause
 */


#ifndef YACUT_H_
#define YACUT_H_

#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else
#include <stdio.h>
#endif

#ifndef YCT_OPT_DISABLE_TIMING
#include <time.h>
#define YCT_TIMER clock_t
#else
#define YCT_TIMER int
#endif

#ifdef YCT_OPT_FPRINTF
#define VNUT_YCT_FPRINTF YCT_OPT_FPRINTF
#else
#ifdef YCT_OPT_JAILHOUSE
#include <inmate.h>
#define VNUT_YCT_FPRINTF(s, f, ...) printk((f), __VA_ARGS__)
#else
#define VNUT_YCT_FPRINTF fprintf
#endif
#endif

#ifdef YCT_OPT_FWPRINTF
#define VNUT_YCT_FWPRINTF YCT_OPT_FWPRINTF
#else
#ifdef YCT_OPT_JAILHOUSE
#include <inmate.h>
#define VNUT_YCT_FWPRINTF(s, f, ...) printk("???")
#else
#define VNUT_YCT_FWPRINTF fwprintf
#endif
#endif

#define VNUT_YCT_FPUTS(str) \
    ((void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "%s", (const char*)(str)))

#ifndef YCT_OPT_DISABLE_WCHAR
#include <wchar.h>
#define VNUT_YCT_FPUTWS(str) \
    ((void)VNUT_YCT_FWPRINTF(p_yct_ctx_->out, L"%s", (const wchar_t*)(str)))
#else
#define VNUT_YCT_FPUTWS(str) ((void)VNUT_YCT_FPUTS("???"))
#pragma message("yaCut: WCHAR DISABLED")
#endif

#ifndef YCT_FUNC_NAME
#if ( ( defined(_MSC_VER) && (_MSC_VER >= 1310) ) || defined(__WATCOMC__) )
#define YCT_FUNC_NAME __FUNCTION__
#elif ( defined(__GNUC__) || defined(__ICC) || defined(__IBMC__) )
#define YCT_FUNC_NAME __FUNCTION__
#elif ( defined(__BORLANDC__) && (__BORLANDC__ > 0x520) )
#define YCT_FUNC_NAME __FUNC__
#elif defined(__FUNCSIG__)
#define _FUNCTION YCT_FUNC_NAME __FUNCSIG__
#elif ( defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901) )
#define YCT_FUNC_NAME __func__
#elif ( defined(__cplusplus) && (__cplusplus >= 201103) )
#define YCT_FUNC_NAME __func__
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
#define VNUT_YCT_FLAGS_LAST_WARNED          0x100

#define YCT_GET_NAME()          "yaCut"
#define YCT_VERSION_MAJOR()     2
#define YCT_VERSION_MINOR()     0
#define YCT_VERSION_RELEASE     2

struct yct_context {
    void* arg;
    const char* msg;
    FILE* out;
#ifndef YCT_OPT_DISABLE_TIMING
    YCT_TIMER clocks;
#endif
    int suites;
    int messages;
    int tests;
    int assertions;
    int total_warnings;
    int warnings;
    int failed;
    int flags;
};

#define VNUT_YCT_INIT_DATA(ctx) \
    ctx.suites             = 0; \
    ctx.messages           = 0; \
    ctx.tests              = 0; \
    ctx.assertions         = 0; \
    ctx.total_warnings     = 0; \
    ctx.warnings           = 0; \
    ctx.failed             = 0; \
    ctx.flags              = 0

#define VNUT_YCT_GET_BIT(var, mask) ((var) & (mask))
#define VNUT_YCT_CLEAR_BIT(var, mask) ((var) &= ~(mask))
#define VNUT_YCT_SET_BIT(var, mask) do { VNUT_YCT_CLEAR_BIT((var), (mask)); \
                                         (var) |= (mask); } while (0)
#define VNUT_YCT_COPY_BIT(src, dest, mask) \
    VNUT_YCT_SET_BIT((dest), VNUT_YCT_GET_BIT((src), (mask)))

#define VNUT_YCT_ABS(a) ((a) < (-(a)) ? -(a) : (a))

#define YCT_DEFAULT_OUTPUT (stdout)

#define YCT_SET_OUTPUT(handle) (yct_main_ctx_.out = (handle))

#define YCT_STATUS_OK      0
#define YCT_STATUS_WARNING 1
#define YCT_STATUS_FAILED  2

#define YCT_GET_RETURN_VALUE(val) do { \
    if (p_yct_ctx_->failed > 0)        \
        val = YCT_STATUS_FAILED;       \
    else {                             \
        if (p_yct_ctx_->warnings > 0)  \
            val = YCT_STATUS_WARNING;  \
        else                           \
            val = YCT_STATUS_OK;       \
    }                                  \
} while (0)

#define YCT_LAST_FAILED() VNUT_YCT_GET_BIT(p_yct_ctx_->flags, \
                                           VNUT_YCT_FLAGS_LAST_FAILED)

#define YCT_GET_MESSAGES() (p_yct_ctx_->messages)
#define YCT_GET_TOTAL_WARNINGS() (p_yct_ctx_->total_warnings)
#define YCT_GET_WARNINGS() (p_yct_ctx_->warnings)

/* Enable/Disable */
#define YCT_ENABLE() \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED)

#define YCT_DISABLE() \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED)

#define YCT_IS_ENABLED() \
    (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED) == 0)

/* Blocking */
#define YCT_SET_BLOCKED() do {                                         \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                              \
                       VNUT_YCT_FLAGS_FULL_BLOCKING_MODE);             \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE); \
} while (0)

#define YCT_SET_FULL_BLOCKED() do {                                         \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_BLOCKING_MODE);    \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); \
} while (0)

#define YCT_DISABLE_BLOCKING() do {                 \
    VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,           \
                       VNUT_YCT_FLAGS_BLOCKING_MODE \
                       | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE); } while (0)

#define YCT_IS_LOCKED() \
    VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED)

#define VNUT_YCT_IF_OK if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, \
    VNUT_YCT_FLAGS_DISABLED | VNUT_YCT_FLAGS_LOCKED) == 0)

#define VNUT_YCT_IF_SET_BLOCKED() do {                                    \
    if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,                               \
        VNUT_YCT_FLAGS_BLOCKING_MODE | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) \
    != 0) { VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED); } \
} while (0)

/* Timing */
#ifdef YCT_OPT_DISABLE_TIMING
#pragma message("yaCut: TIMING DISABLED")
#define YCT_START_TIMER(t)
#define YCT_STOP_TIMER(t)
#define YCT_GET_TIME(t, f)
#define YCT_DISABLE_TIMING()
#define VNUT_YCT_GET_START_TIME()
#define VNUT_YCT_PRINT_ELAPSED_TIME()
#else
#define YCT_START_TIMER(t) ((t) = clock())
#define YCT_STOP_TIMER(t)  ((t) = clock() - (t))
#define YCT_GET_TIME(t, f) ((f) = (float)(t) / (float)CLOCKS_PER_SEC)
#define YCT_DISABLE_TIMING() \
    VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED_TIMING)
#define VNUT_YCT_GET_START_TIME() YCT_START_TIMER(yct_main_ctx_.clocks)
#define VNUT_YCT_PRINT_ELAPSED_TIME() do {                                  \
    if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_DISABLED_TIMING) \
        == 0) (void)VNUT_YCT_FPRINTF(yct_main_ctx_.out, " (%.2f seconds)",  \
        (float)(clock() - yct_main_ctx_.clocks) / (float)CLOCKS_PER_SEC);   \
} while (0)
#endif

/* Log */
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

/* General */
#define YCT_BEGIN(name) {                                  \
    struct yct_context yct_main_ctx_;                      \
    struct yct_context* const p_yct_ctx_ = &yct_main_ctx_; \
    VNUT_YCT_INIT_DATA(yct_main_ctx_);                     \
    YCT_SET_OUTPUT(YCT_DEFAULT_OUTPUT);                    \
    yct_main_ctx_.arg = NULL;                              \
    yct_main_ctx_.msg = (const char*)(name);               \
    VNUT_YCT_GET_START_TIME();

#define YCT_TEST(test_name) \
    static void test_name(struct yct_context* const p_yct_ctx_)

#define DISABLE_YCT_TEST(test_name) YCT_TEST(test_name) {     \
    if (p_yct_ctx_->assertions == p_yct_ctx_->flags)          \
        p_yct_ctx_->assertions = p_yct_ctx_->flags; return; } \
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

#define YCT_SET_PARAMETER(par) (p_yct_ctx_->arg = (void*)(par))
#define YCT_PARAMETER (p_yct_ctx_->arg)

#define VNUT_YCT_PRINT_STR_VAR(str) do {                 \
    if ((str) == NULL) { (void)VNUT_YCT_FPUTS("NULL"); } \
    else {                                               \
        if (sizeof(*(str)) < 2) VNUT_YCT_FPUTS(str);     \
        else VNUT_YCT_FPUTWS(str); } } while (0)

#define VNUT_YCT_PRINT_END_NAME() do {                      \
    if ((yct_ctx_.failed > 0 || yct_ctx_.warnings > 0       \
         || yct_ctx_.messages > 0) && yct_ctx_.out != NULL) \
    { VNUT_YCT_FPUTS("\n"); } } while (0)

#define YCT_TEST_RUN(test_name)                                              \
    do { VNUT_YCT_IF_OK {                                                    \
        struct yct_context yct_ctx_;                                         \
        VNUT_YCT_INIT_DATA(yct_ctx_);                                        \
        yct_ctx_.out = p_yct_ctx_->out;                                      \
        yct_ctx_.arg = p_yct_ctx_->arg;                                      \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,                 \
                          VNUT_YCT_FLAGS_BLOCKING_MODE                       \
                          | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE                \
                          | VNUT_YCT_FLAGS_LOG);                             \
        VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED);   \
        if (yct_ctx_.out != NULL) {                                          \
            VNUT_YCT_FPUTS(#test_name); VNUT_YCT_FPUTS("\n"); }              \
        test_name(&yct_ctx_);                                                \
        p_yct_ctx_->tests++;                                                 \
        p_yct_ctx_->total_warnings += yct_ctx_.total_warnings;               \
        p_yct_ctx_->warnings += yct_ctx_.warnings;                           \
        p_yct_ctx_->assertions += yct_ctx_.assertions;                       \
        p_yct_ctx_->messages += yct_ctx_.messages;                           \
        if (yct_ctx_.failed > 0) {                                           \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,             \
                              VNUT_YCT_FLAGS_LOCKED);                        \
            p_yct_ctx_->failed++;                                            \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED); \
        }                                                                    \
        VNUT_YCT_PRINT_END_NAME();                                           \
    } } while (0)

#define NO_YCT_TEST_RUN(test_name) (void)(test_name)

#define YCT_TEST_ADD(test_name)                                              \
    do { VNUT_YCT_IF_OK {                                                    \
        struct yct_context yct_ctx_;                                         \
        VNUT_YCT_INIT_DATA(yct_ctx_);                                        \
        yct_ctx_.out = p_yct_ctx_->out;                                      \
        yct_ctx_.arg = p_yct_ctx_->arg;                                      \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,                 \
                          VNUT_YCT_FLAGS_BLOCKING_MODE                       \
                          | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE                \
                          | VNUT_YCT_FLAGS_LOG);                             \
        VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED);   \
        if (yct_ctx_.out != NULL) { VNUT_YCT_FPUTS("  ");                    \
            VNUT_YCT_FPUTS(#test_name); VNUT_YCT_FPUTS("\n"); }              \
        if (f_setup_ != NULL)                                                \
            (*f_setup_)();                                                   \
        test_name(&yct_ctx_);                                                \
        if (f_tear_down_ != NULL)                                            \
            (*f_tear_down_)();                                               \
        p_yct_ctx_->tests++;                                                 \
        p_yct_ctx_->total_warnings += yct_ctx_.total_warnings;               \
        p_yct_ctx_->warnings += yct_ctx_.warnings;                           \
        p_yct_ctx_->assertions += yct_ctx_.assertions;                       \
        p_yct_ctx_->messages += yct_ctx_.messages;                           \
        if (yct_ctx_.failed > 0) {                                           \
            VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,             \
                              VNUT_YCT_FLAGS_LOCKED);                        \
            p_yct_ctx_->failed++;                                            \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_FAILED); \
        }                                                                    \
        VNUT_YCT_PRINT_END_NAME();                                           \
    } } while (0)

#define NO_YCT_TEST_ADD(test_name)

#define NO_YCT_SUITE_RUN(suite_name) (void)(suite_name)

#define YCT_SUITE_RUN(suite_name)                                        \
    do { VNUT_YCT_IF_OK {                                                \
        struct yct_context yct_ctx_;                                     \
        VNUT_YCT_INIT_DATA(yct_ctx_);                                    \
        yct_ctx_.out = p_yct_ctx_->out;                                  \
        yct_ctx_.arg = p_yct_ctx_->arg;                                  \
        VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                            \
                           VNUT_YCT_FLAGS_LAST_FAILED);                  \
        VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,             \
                          VNUT_YCT_FLAGS_BLOCKING_MODE                   \
                          | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE            \
                          | VNUT_YCT_FLAGS_LOG);                         \
        if (yct_ctx_.out != NULL) { VNUT_YCT_FPUTS("\n");                \
            VNUT_YCT_FPUTS(#suite_name); VNUT_YCT_FPUTS(" (suite)\n"); } \
        suite_name(&yct_ctx_);                                           \
        VNUT_YCT_COPY_BIT(yct_ctx_.flags, p_yct_ctx_->flags,             \
                          VNUT_YCT_FLAGS_LOCKED);                        \
        if (yct_ctx_.failed > 0) {                                       \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags,                          \
                             VNUT_YCT_FLAGS_LAST_FAILED);                \
        }                                                                \
        VNUT_YCT_PRINT_END_NAME();                                       \
        p_yct_ctx_->suites++;                                            \
        p_yct_ctx_->tests += yct_ctx_.tests;                             \
        p_yct_ctx_->failed += yct_ctx_.failed;                           \
        p_yct_ctx_->total_warnings += yct_ctx_.total_warnings;           \
        p_yct_ctx_->warnings += yct_ctx_.warnings;                       \
        p_yct_ctx_->assertions += yct_ctx_.assertions;                   \
        p_yct_ctx_->messages += yct_ctx_.messages;                       \
    } } while (0)

#define YCT_END() }

#define YCT_MAIN(epf) int main(void) { YCT_BEGIN(""); epf(&yct_main_ctx_); \
    if (VNUT_YCT_GET_BIT(yct_main_ctx_.flags, VNUT_YCT_FLAGS_IS_SUITE))    \
        yct_main_ctx_.suites++; else yct_main_ctx_.tests++;                \
    YCT_DUMP(); YCT_END(); return 0; }

/* Parallel */
#ifndef YCT_OPT_ENABLE_PARALLEL

#define YCT_PARALLEL()
#define YCT_SCHEDULE_TEST()
#define YCT_PARALLEL_TEST(yct_name) YCT_TEST_RUN(yct_name);
#define YCT_JOIN_TEST()
#define YCT_COLLECT_TEST_RESULTS()

#define YCT_GO()
#define YCT_SYNCHRONIZED()
#define YCT_ATOMIC()

#else
#pragma message("yaCut: PARALLEL ENABLED")

#ifdef _MSC_VER
#define YCT_PARALLEL()      __pragma(omp parallel sections)
#define YCT_SCHEDULE_TEST() __pragma(omp section)
#define YCT_JOIN_TEST()     __pragma(omp critical)
#define YCT_ATOMIC()        __pragma(omp atomic)
#else
#define YCT_PARALLEL()      _Pragma("omp parallel sections")
#define YCT_SCHEDULE_TEST() _Pragma("omp section")
#define YCT_JOIN_TEST()     _Pragma("omp critical")
#define YCT_ATOMIC()        _Pragma("omp atomic")
#endif

#define YCT_GO()           YCT_SCHEDULE_TEST()
#define YCT_SYNCHRONIZED() YCT_JOIN_TEST()

#define YCT_PARALLEL_TEST(test_name) { VNUT_YCT_IF_OK {   \
    struct yct_context yct_ctx_;                          \
    VNUT_YCT_INIT_DATA(yct_ctx_);                         \
    yct_ctx_.out = p_yct_ctx_->out;                       \
    yct_ctx_.arg = p_yct_ctx_->arg;                       \
    VNUT_YCT_COPY_BIT(p_yct_ctx_->flags, yct_ctx_.flags,  \
                      VNUT_YCT_FLAGS_BLOCKING_MODE        \
                      | VNUT_YCT_FLAGS_FULL_BLOCKING_MODE \
                      | VNUT_YCT_FLAGS_LOG);              \
    test_name(&yct_ctx_);

#define YCT_COLLECT_TEST_RESULTS() {                       \
    p_yct_ctx_->tests++;                                   \
    p_yct_ctx_->total_warnings += yct_ctx_.total_warnings; \
    p_yct_ctx_->warnings += yct_ctx_.warnings;             \
    p_yct_ctx_->assertions += yct_ctx_.assertions;         \
    p_yct_ctx_->messages += yct_ctx_.messages;             \
    if (yct_ctx_.failed > 0) {                             \
        VNUT_YCT_COPY_BIT(yct_ctx_.flags,                  \
                          p_yct_ctx_->flags,               \
                          VNUT_YCT_FLAGS_LOCKED);          \
        p_yct_ctx_->failed++;                              \
    } } } }
#endif

#define YCT_DUMP_SHORT()                                                  \
    do { if (yct_main_ctx_.out != NULL) {                                 \
        if (yct_main_ctx_.failed > 0) {                                   \
            (void)VNUT_YCT_FPRINTF(yct_main_ctx_.out, "%d FAILED\n",      \
                                   yct_main_ctx_.failed);                 \
        }                                                                 \
        else {                                                            \
            if (yct_main_ctx_.warnings > 0) {                             \
                (void)VNUT_YCT_FPRINTF(yct_main_ctx_.out, "%d WARNING\n", \
                                       yct_main_ctx_.warnings);           \
            }                                                             \
            else                                                          \
                VNUT_YCT_FPUTS("ALL OK\n");                               \
        }                                                                 \
    } } while (0)

#define YCT_DUMP()                                                           \
do { if (p_yct_ctx_->out != NULL) {                                          \
    const int vnut_yct_tests_ = p_yct_ctx_->tests > 0 ?                      \
                                p_yct_ctx_->tests : 1;                       \
    const int vnut_yct_passed_ = vnut_yct_tests_ - p_yct_ctx_->failed;       \
    if (p_yct_ctx_->msg != NULL && *p_yct_ctx_->msg == 0)                    \
        p_yct_ctx_->msg = NULL;                                              \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "\n------- %s v%d.%d Summary",   \
    YCT_GET_NAME(), YCT_VERSION_MAJOR(), YCT_VERSION_MINOR());               \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, " ------%s%s\nTests:        %d", \
    p_yct_ctx_->msg != NULL ? "\nExecuting battery: " : "",                  \
    p_yct_ctx_->msg != NULL ? p_yct_ctx_->msg : "", vnut_yct_tests_);        \
    VNUT_YCT_FPUTS("\n");                                                    \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Passed:       %d\n",            \
    vnut_yct_passed_);                                                       \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Failed:       %d\n",            \
    p_yct_ctx_->failed);                                                     \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Suites:       %d\n",            \
    p_yct_ctx_->suites);                                                     \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Assertions:   %d\n",            \
    p_yct_ctx_->assertions);                                                 \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Warnings:     %d\n",            \
    p_yct_ctx_->total_warnings);                                             \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Messages:     %d\n",            \
    p_yct_ctx_->messages);                                                   \
    (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "Success:      %d",              \
    (100 * vnut_yct_passed_) / vnut_yct_tests_); VNUT_YCT_FPUTS("%");        \
    VNUT_YCT_PRINT_ELAPSED_TIME();                                           \
    if (p_yct_ctx_->failed > 0)                                              \
        VNUT_YCT_FPUTS("\n-------> SOMETHING FAILED <------\n");             \
    else {                                                                   \
        if (p_yct_ctx_->warnings > 0)                                        \
            VNUT_YCT_FPUTS("\n---------> SOME WARNING <--------\n");         \
        else                                                                 \
            VNUT_YCT_FPUTS("\n------------> ALL OK <-----------\n");         \
    }                                                                        \
} } while (0)

#define VNUT_YCT_PRINT_VAR(var) do {                                         \
static const char* vnut_yct_hexes_ = "0123456789ABCDEF";                     \
const int vnut_yct_nibbles_ = (int)(sizeof(var) << 1);                       \
int vnut_yct_i_, vnut_yct_started_ = 0;                                      \
char vnut_yct_c_; (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "0x");             \
for (vnut_yct_i_ = vnut_yct_nibbles_ - 1; vnut_yct_i_ >= 0; vnut_yct_i_--) { \
    vnut_yct_c_ = vnut_yct_hexes_[(((size_t)(var)) >> (vnut_yct_i_ << 2))    \
                                      & 0xF];                                \
    if (vnut_yct_started_ != 0 || vnut_yct_c_ != '0' || vnut_yct_i_ == 0) {  \
        (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, "%c", vnut_yct_c_);          \
        vnut_yct_started_ = 1; } } } while(0)

#define VNUT_YCT_PRINT_MAIN(main_msg)                                \
    do { if (p_yct_ctx_->out != NULL) {                              \
        VNUT_YCT_FPUTS(__FILE__);                                    \
        (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out, ":%d: [", __LINE__); \
        VNUT_YCT_FPUTS(YCT_FUNC_NAME);                               \
        VNUT_YCT_FPUTS("]: ");                                       \
        VNUT_YCT_FPUTS(main_msg);                                    \
        VNUT_YCT_FPUTS(": ");                                        \
    } } while (0)

#define VNUT_YCT_PRINT(main_msg, cond) do { \
    VNUT_YCT_PRINT_MAIN(main_msg);          \
    if (p_yct_ctx_->out != NULL) {          \
        VNUT_YCT_FPUTS("(");                \
        VNUT_YCT_FPUTS(cond);               \
        VNUT_YCT_FPUTS(")");                \
    }                                       \
} while (0)

#define VNUT_YCT_PRINT_MSG(main_msg, cond, msg) do { \
    VNUT_YCT_PRINT(main_msg, cond);                  \
    if (p_yct_ctx_->out != NULL) {                   \
        VNUT_YCT_FPUTS(" { \"");                     \
        VNUT_YCT_FPUTS(msg);                         \
        VNUT_YCT_FPUTS("\" }\n");                    \
    }                                                \
} while (0)

#ifdef YCT_OPT_DISABLE_LOG
#define VNUT_YCT_LOG(cond)
#else
#define VNUT_YCT_LOG(cond)                                              \
    do { if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOG)) { \
        VNUT_YCT_PRINT("LOG", #cond);                                   \
        if (p_yct_ctx_->out != NULL)                                    \
            VNUT_YCT_FPUTS("\n");                                       \
    } } while (0)
#endif

/* Messages, Warnings, Assertions */
#define YCT_MESSAGE(msg) do {       \
    p_yct_ctx_->messages++;         \
    VNUT_YCT_PRINT_MAIN("MESSAGE"); \
    if (p_yct_ctx_->out != NULL) {  \
        VNUT_YCT_FPUTS("{ \"");     \
        VNUT_YCT_FPUTS(msg);        \
        VNUT_YCT_FPUTS("\" }\n");   \
    }                               \
} while (0)

#ifndef VNUT_YCT_RETURN
#define VNUT_YCT_RETURN() return
#endif

#define YCT_WARNING(cond)                                                    \
    do { VNUT_YCT_IF_OK {                                                    \
        p_yct_ctx_->total_warnings++;                                        \
        VNUT_YCT_LOG(cond);                                                  \
        if (!(cond)) {                                                       \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_WARNED); \
            p_yct_ctx_->warnings++;                                          \
            VNUT_YCT_PRINT("WARNING", #cond);                                \
            if (p_yct_ctx_->out != NULL)                                     \
                VNUT_YCT_FPUTS("\n");                                        \
            if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,                          \
                                 VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) != 0)    \
            {                                                                \
                VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED);  \
                p_yct_ctx_->failed = 1;                                      \
                VNUT_YCT_RETURN();                                           \
            }                                                                \
        }                                                                    \
        else {                                                               \
            VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                            \
                               VNUT_YCT_FLAGS_LAST_WARNED); }                \
    } } while (0)

#define YCT_WARNING_MSG(cond, msg)                                           \
    do { VNUT_YCT_IF_OK {                                                    \
        p_yct_ctx_->total_warnings++;                                        \
        VNUT_YCT_LOG(cond);                                                  \
        if (!(cond)) {                                                       \
            VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LAST_WARNED); \
            p_yct_ctx_->warnings++;                                          \
            VNUT_YCT_PRINT_MSG("WARNING", #cond, msg);                       \
            if (VNUT_YCT_GET_BIT(p_yct_ctx_->flags,                          \
                                 VNUT_YCT_FLAGS_FULL_BLOCKING_MODE) != 0)    \
            {                                                                \
                VNUT_YCT_SET_BIT(p_yct_ctx_->flags, VNUT_YCT_FLAGS_LOCKED);  \
                p_yct_ctx_->failed = 1;                                      \
                VNUT_YCT_RETURN();                                           \
            }                                                                \
        }                                                                    \
        else {                                                               \
            VNUT_YCT_CLEAR_BIT(p_yct_ctx_->flags,                            \
                               VNUT_YCT_FLAGS_LAST_WARNED); }                \
    } } while (0)

#define YCT_FAIL(msg)                                 \
    do { VNUT_YCT_IF_OK {                             \
        VNUT_YCT_PRINT_MSG("FAILED", "FAILURE", msg); \
        p_yct_ctx_->failed = 1;                       \
        VNUT_YCT_IF_SET_BLOCKED();                    \
        VNUT_YCT_RETURN();                            \
    } } while (0)

#define YCT_ASSERT(cond)                     \
    do { VNUT_YCT_IF_OK {                    \
        p_yct_ctx_->assertions++;            \
        VNUT_YCT_LOG(cond);                  \
        if (!(cond)) {                       \
            VNUT_YCT_PRINT("FAILED", #cond); \
            p_yct_ctx_->failed = 1;          \
            if (p_yct_ctx_->out != NULL)     \
                VNUT_YCT_FPUTS("\n");        \
            VNUT_YCT_IF_SET_BLOCKED();       \
            VNUT_YCT_RETURN();               \
        }                                    \
    } } while (0)

#define YCT_ASSERT_MSG(cond, msg)                     \
    do { VNUT_YCT_IF_OK {                             \
        p_yct_ctx_->assertions++;                     \
        VNUT_YCT_LOG(cond);                           \
        if (!(cond)) {                                \
            VNUT_YCT_PRINT_MSG("FAILED", #cond, msg); \
            p_yct_ctx_->failed = 1;                   \
            VNUT_YCT_IF_SET_BLOCKED();                \
            VNUT_YCT_RETURN();                        \
        }                                             \
    } } while (0)

#define YCT_ASSERT_EQUAL(expected, actual)                      \
    do { VNUT_YCT_IF_OK {                                       \
        p_yct_ctx_->assertions++;                               \
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
    } } while (0)

#define YCT_ASSERT_EQUAL_MSG(expected, actual, msg)             \
    do { VNUT_YCT_IF_OK {                                       \
        p_yct_ctx_->assertions++;                               \
        VNUT_YCT_LOG(expected == actual);                       \
        if (!((expected) == (actual))) {                        \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual); \
            if (p_yct_ctx_->out != NULL) {                      \
                VNUT_YCT_FPUTS(": Expected: <");                \
                VNUT_YCT_PRINT_VAR(expected);                   \
                VNUT_YCT_FPUTS(">, Actual: <");                 \
                VNUT_YCT_PRINT_VAR(actual);                     \
                VNUT_YCT_FPUTS("> { \"");                       \
                VNUT_YCT_FPUTS(msg);                            \
                VNUT_YCT_FPUTS("\" }\n");                       \
            }                                                   \
            p_yct_ctx_->failed = 1;                             \
            VNUT_YCT_IF_SET_BLOCKED();                          \
            VNUT_YCT_RETURN();                                  \
        }                                                       \
    } } while (0)

#define YCT_ASSERT_ZERO(cond) YCT_ASSERT_EQUAL(0, (cond))
#define YCT_ASSERT_ZERO_MSG(cond, msg) YCT_ASSERT_EQUAL_MSG(0, (cond), msg)
#define YCT_ASSERT_ONE(cond) YCT_ASSERT_EQUAL(1, (cond))
#define YCT_ASSERT_ONE_MSG(cond, msg) YCT_ASSERT_EQUAL_MSG(1, (cond), msg)

#define YCT_ASSERT_NOT_EQUAL(v1, v2)                     \
    do { VNUT_YCT_IF_OK {                                \
        p_yct_ctx_->assertions++;                        \
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
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_MSG(v1, v2, msg)            \
    do { VNUT_YCT_IF_OK {                                \
        p_yct_ctx_->assertions++;                        \
        VNUT_YCT_LOG(v1 != v2);                          \
        if ((v1) == (v2)) {                              \
            VNUT_YCT_PRINT("FAILED", #v1 " != " #v2);    \
            if (p_yct_ctx_->out != NULL) {               \
                VNUT_YCT_FPUTS(": Both values were: <"); \
                VNUT_YCT_PRINT_VAR(v1);                  \
                VNUT_YCT_FPUTS("> { \"");                \
                VNUT_YCT_FPUTS(msg);                     \
                VNUT_YCT_FPUTS("\" }\n");                \
            }                                            \
            p_yct_ctx_->failed = 1;                      \
            VNUT_YCT_IF_SET_BLOCKED();                   \
            VNUT_YCT_RETURN();                           \
        }                                                \
    } } while (0)

#define YCT_ASSERT_NULL(var)                   \
    do { VNUT_YCT_IF_OK {                      \
        p_yct_ctx_->assertions++;              \
        VNUT_YCT_LOG(var == NULL);             \
        if ((var) != NULL) {                   \
            VNUT_YCT_PRINT_MAIN("FAILED");     \
            if (p_yct_ctx_->out != NULL) {     \
                VNUT_YCT_FPUTS("(");           \
                VNUT_YCT_FPUTS(#var);          \
                VNUT_YCT_FPUTS(" == NULL)\n"); \
            }                                  \
            p_yct_ctx_->failed = 1;            \
            VNUT_YCT_IF_SET_BLOCKED();         \
            VNUT_YCT_RETURN();                 \
        }                                      \
    } } while (0)

#define YCT_ASSERT_NULL_MSG(var, msg)             \
    do { VNUT_YCT_IF_OK {                         \
        p_yct_ctx_->assertions++;                 \
        VNUT_YCT_LOG(var == NULL);                \
        if ((var) != NULL) {                      \
            VNUT_YCT_PRINT_MAIN("FAILED");        \
            if (p_yct_ctx_->out != NULL) {        \
                VNUT_YCT_FPUTS("(");              \
                VNUT_YCT_FPUTS(#var);             \
                VNUT_YCT_FPUTS(" == NULL) { \""); \
                VNUT_YCT_FPUTS(msg);              \
                VNUT_YCT_FPUTS("\" }\n");         \
            }                                     \
            p_yct_ctx_->failed = 1;               \
            VNUT_YCT_IF_SET_BLOCKED();            \
            VNUT_YCT_RETURN();                    \
        }                                         \
    } } while (0)

#define YCT_ASSERT_NOT_NULL(var)               \
    do { VNUT_YCT_IF_OK {                      \
        p_yct_ctx_->assertions++;              \
        VNUT_YCT_LOG(var != NULL);             \
        if ((var) == NULL) {                   \
            VNUT_YCT_PRINT_MAIN("FAILED");     \
            if (p_yct_ctx_->out != NULL) {     \
                VNUT_YCT_FPUTS("(");           \
                VNUT_YCT_FPUTS(#var);          \
                VNUT_YCT_FPUTS(" != NULL)\n"); \
            }                                  \
            p_yct_ctx_->failed = 1;            \
            VNUT_YCT_IF_SET_BLOCKED();         \
            VNUT_YCT_RETURN();                 \
        }                                      \
    } } while (0)

#define YCT_ASSERT_NOT_NULL_MSG(var, msg)   \
    do { VNUT_YCT_IF_OK {                   \
        p_yct_ctx_->assertions++;           \
        VNUT_YCT_LOG(var != NULL);          \
        if ((var) == NULL) {                \
            VNUT_YCT_PRINT_MAIN("FAILED");  \
            if (p_yct_ctx_->out != NULL) {  \
                VNUT_YCT_FPUTS("(");        \
                VNUT_YCT_FPUTS(#var);       \
                VNUT_YCT_FPUTS(" != NULL"); \
                VNUT_YCT_FPUTS(") { \"");   \
                VNUT_YCT_FPUTS(msg);        \
                VNUT_YCT_FPUTS("\" }\n");   \
            }                               \
            p_yct_ctx_->failed = 1;         \
            VNUT_YCT_IF_SET_BLOCKED();      \
            VNUT_YCT_RETURN();              \
        }                                   \
    } } while (0)

#define VNUT_YCT_COMPARE_STR(str1, str2, out) do {                       \
    out = 0;                                                             \
    if (((str1) == NULL) && ((str2) != NULL)) out = -1;                  \
    else { if (((str1) != NULL) && ((str2) == NULL)) out = 1; }          \
    if (out == 0) {                                                      \
        unsigned int vnut_yct_i_;                                        \
        for (vnut_yct_i_ = 0; str1[vnut_yct_i_] != 0                     \
                              && str1[vnut_yct_i_] == str2[vnut_yct_i_]; \
             vnut_yct_i_++) ;                                            \
        out = (int)((unsigned int)str1[vnut_yct_i_]                      \
                    - (unsigned int)str2[vnut_yct_i_]);                  \
    }                                                                    \
} while (0)

#define YCT_ASSERT_EQUAL_STR(expected, actual)                  \
    do { VNUT_YCT_IF_OK {                                       \
    int vnut_yct_out_;                                          \
    p_yct_ctx_->assertions++;                                   \
    VNUT_YCT_LOG(expected == actual);                           \
    VNUT_YCT_COMPARE_STR((expected), (actual), vnut_yct_out_);  \
    if (vnut_yct_out_ != 0) {                                   \
        VNUT_YCT_PRINT("FAILED", #expected " == " #actual);     \
        if (p_yct_ctx_->out != NULL) {                          \
            VNUT_YCT_FPUTS(": Expected: <");                    \
            VNUT_YCT_PRINT_STR_VAR(expected);                   \
            VNUT_YCT_FPUTS(">, Actual: <");                     \
            VNUT_YCT_PRINT_STR_VAR(actual);                     \
            VNUT_YCT_FPUTS(">\n");                              \
        }                                                       \
        p_yct_ctx_->failed = 1;                                 \
        VNUT_YCT_IF_SET_BLOCKED();                              \
        VNUT_YCT_RETURN();                                      \
    }                                                           \
} } while (0)

#define YCT_ASSERT_EQUAL_STR_MSG(expected, actual, msg)            \
    do { VNUT_YCT_IF_OK {                                          \
        int vnut_yct_out_;                                         \
        p_yct_ctx_->assertions++;                                  \
        VNUT_YCT_LOG(expected == actual);                          \
        VNUT_YCT_COMPARE_STR((expected), (actual), vnut_yct_out_); \
        if (vnut_yct_out_ != 0) {                                  \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual);    \
            if (p_yct_ctx_->out != NULL) {                         \
                VNUT_YCT_FPUTS(": Expected: <");                   \
                VNUT_YCT_PRINT_STR_VAR(expected);                  \
                VNUT_YCT_FPUTS(">, Actual: <");                    \
                VNUT_YCT_PRINT_STR_VAR(actual);                    \
                VNUT_YCT_FPUTS("> { \"");                          \
                VNUT_YCT_FPUTS(msg);                               \
                VNUT_YCT_FPUTS("\" }\n");                          \
            }                                                      \
            p_yct_ctx_->failed = 1;                                \
            VNUT_YCT_IF_SET_BLOCKED();                             \
            VNUT_YCT_RETURN();                                     \
        }                                                          \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_STR(str1, str2)                 \
    do { VNUT_YCT_IF_OK {                                    \
        int vnut_yct_out_;                                   \
        p_yct_ctx_->assertions++;                            \
        VNUT_YCT_LOG(str1 != str2);                          \
        VNUT_YCT_COMPARE_STR((str1), (str2), vnut_yct_out_); \
        if (vnut_yct_out_ == 0) {                            \
            VNUT_YCT_PRINT("FAILED", #str1 " != " #str2);    \
            if (p_yct_ctx_->out != NULL) {                   \
                VNUT_YCT_FPUTS(": 'Both strings were: <");   \
                VNUT_YCT_PRINT_STR_VAR(str1);                \
                VNUT_YCT_FPUTS(">'\n");                      \
            }                                                \
            p_yct_ctx_->failed = 1;                          \
            VNUT_YCT_IF_SET_BLOCKED();                       \
            VNUT_YCT_RETURN();                               \
        }                                                    \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_STR_MSG(str1, str2, msg)        \
    do { VNUT_YCT_IF_OK {                                    \
        int vnut_yct_out_;                                   \
        p_yct_ctx_->assertions++;                            \
        VNUT_YCT_LOG(str1 != str2);                          \
        VNUT_YCT_COMPARE_STR((str1), (str2), vnut_yct_out_); \
        if (vnut_yct_out_ == 0) {                            \
            VNUT_YCT_PRINT("FAILED", #str1 " != " #str2);    \
            if (p_yct_ctx_->out != NULL) {                   \
                VNUT_YCT_FPUTS(": 'Both strings were: <");   \
                VNUT_YCT_PRINT_STR_VAR(str1);                \
                VNUT_YCT_FPUTS(">' { \"");                   \
                VNUT_YCT_FPUTS(msg);                         \
                VNUT_YCT_FPUTS("\" }\n");                    \
            }                                                \
            p_yct_ctx_->failed = 1;                          \
            VNUT_YCT_IF_SET_BLOCKED();                       \
            VNUT_YCT_RETURN();                               \
        }                                                    \
    } } while (0)

#define VNUT_YCT_COMPARE_ARRAY(arr1, arr2, len, out) do { for (out = 0; \
    out < (unsigned int)(len) && arr1[out] == arr2[out]; out++) ; } while (0)

#define YCT_ASSERT_EQUAL_ARRAY(expected, actual, len)                       \
    do { VNUT_YCT_IF_OK {                                                   \
        unsigned int vnut_yct_out_;                                         \
        p_yct_ctx_->assertions++;                                           \
        VNUT_YCT_LOG(expected == actual);                                   \
        VNUT_YCT_COMPARE_ARRAY((expected), (actual), (len), vnut_yct_out_); \
        if (vnut_yct_out_ < (unsigned int)(len)) {                          \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual);             \
            if (p_yct_ctx_->out != NULL) {                                  \
                (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out,                     \
                      ": 'Element [%u] was different'\n", vnut_yct_out_);   \
            }                                                               \
            p_yct_ctx_->failed = 1;                                         \
            VNUT_YCT_IF_SET_BLOCKED();                                      \
            VNUT_YCT_RETURN();                                              \
        }                                                                   \
    } } while (0)

#define YCT_ASSERT_EQUAL_ARRAY_MSG(expected, actual, len, msg)               \
    do { VNUT_YCT_IF_OK {                                                    \
        unsigned int vnut_yct_out_;                                          \
        p_yct_ctx_->assertions++;                                            \
        VNUT_YCT_LOG(expected == actual);                                    \
        VNUT_YCT_COMPARE_ARRAY((expected), (actual), (len), vnut_yct_out_);  \
        if (vnut_yct_out_ < (unsigned int)(len)) {                           \
            VNUT_YCT_PRINT("FAILED", #expected " == " #actual);              \
            if (p_yct_ctx_->out != NULL) {                                   \
                (void)VNUT_YCT_FPRINTF(p_yct_ctx_->out,                      \
                      ": 'Element [%u] was different' { \"", vnut_yct_out_); \
                VNUT_YCT_FPUTS(msg);                                         \
                VNUT_YCT_FPUTS("\" }\n");                                    \
            }                                                                \
            p_yct_ctx_->failed = 1;                                          \
            VNUT_YCT_IF_SET_BLOCKED();                                       \
            VNUT_YCT_RETURN();                                               \
        }                                                                    \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_ARRAY(arr1, arr2, len)                   \
    do { VNUT_YCT_IF_OK {                                             \
        unsigned int vnut_yct_out_;                                   \
        p_yct_ctx_->assertions++;                                     \
        VNUT_YCT_LOG(arr1 != arr2);                                   \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), vnut_yct_out_); \
        if (vnut_yct_out_ == (unsigned int)(len)) {                   \
            VNUT_YCT_PRINT("FAILED", #arr1 " != " #arr2);             \
            if (p_yct_ctx_->out != NULL)                              \
                VNUT_YCT_FPUTS("\n");                                 \
            p_yct_ctx_->failed = 1;                                   \
            VNUT_YCT_IF_SET_BLOCKED();                                \
            VNUT_YCT_RETURN();                                        \
        }                                                             \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_ARRAY_MSG(arr1, arr2, len, msg)          \
    do { VNUT_YCT_IF_OK {                                             \
        unsigned int vnut_yct_out_;                                   \
        p_yct_ctx_->assertions++;                                     \
        VNUT_YCT_LOG(arr1 != arr2);                                   \
        VNUT_YCT_COMPARE_ARRAY((arr1), (arr2), (len), vnut_yct_out_); \
        if (vnut_yct_out_ == (unsigned int)(len)) {                   \
            VNUT_YCT_PRINT_MSG("FAILED", #arr1 " != " #arr2, msg);    \
            p_yct_ctx_->failed = 1;                                   \
            VNUT_YCT_IF_SET_BLOCKED();                                \
            VNUT_YCT_RETURN();                                        \
        }                                                             \
    } } while (0)

#define YCT_ASSERT_EQUAL_OBJ(expected, actual, fcmp) \
    do { VNUT_YCT_IF_OK {                            \
        p_yct_ctx_->assertions++;                    \
        VNUT_YCT_LOG(expected == actual);            \
        if (fcmp(&expected, &actual) != 0) {         \
            VNUT_YCT_PRINT_MAIN("FAILED");           \
            if (p_yct_ctx_->out != NULL) {           \
                VNUT_YCT_FPUTS("(");                 \
                VNUT_YCT_FPUTS(#expected);           \
                VNUT_YCT_FPUTS(" == ");              \
                VNUT_YCT_FPUTS(#actual);             \
                VNUT_YCT_FPUTS(")\n");               \
            }                                        \
            p_yct_ctx_->failed = 1;                  \
            VNUT_YCT_IF_SET_BLOCKED();               \
            VNUT_YCT_RETURN();                       \
        }                                            \
    } } while (0)

#define YCT_ASSERT_EQUAL_OBJ_MSG(expected, actual, fcmp, msg) \
    do { VNUT_YCT_IF_OK {                                     \
        p_yct_ctx_->assertions++;                             \
        VNUT_YCT_LOG(expected == actual);                     \
        if (fcmp(&expected, &actual) != 0) {                  \
            VNUT_YCT_PRINT_MAIN("FAILED");                    \
            if (p_yct_ctx_->out != NULL) {                    \
                VNUT_YCT_FPUTS("(");                          \
                VNUT_YCT_FPUTS(#expected);                    \
                VNUT_YCT_FPUTS(" == ");                       \
                VNUT_YCT_FPUTS(#actual);                      \
                VNUT_YCT_FPUTS(") { \"");                     \
                VNUT_YCT_FPUTS(msg);                          \
                VNUT_YCT_FPUTS("\" }\n");                     \
            }                                                 \
            p_yct_ctx_->failed = 1;                           \
            VNUT_YCT_IF_SET_BLOCKED();                        \
            VNUT_YCT_RETURN();                                \
        }                                                     \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_OBJ(obj1, obj2, fcmp) \
    do { VNUT_YCT_IF_OK {                          \
        p_yct_ctx_->assertions++;                  \
        VNUT_YCT_LOG(obj1 != obj2);                \
        if (fcmp(&obj1, &obj2) == 0) {             \
            VNUT_YCT_PRINT_MAIN("FAILED");         \
            if (p_yct_ctx_->out != NULL) {         \
                VNUT_YCT_FPUTS("(");               \
                VNUT_YCT_FPUTS(#obj1);             \
                VNUT_YCT_FPUTS(" != ");            \
                VNUT_YCT_FPUTS(#obj2);             \
                VNUT_YCT_FPUTS(")\n");             \
            }                                      \
            p_yct_ctx_->failed = 1;                \
            VNUT_YCT_IF_SET_BLOCKED();             \
            VNUT_YCT_RETURN();                     \
        }                                          \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_OBJ_MSG(obj1, obj2, fcmp, msg) \
    do { VNUT_YCT_IF_OK {                                   \
        p_yct_ctx_->assertions++;                           \
        VNUT_YCT_LOG(obj1 != obj2);                         \
        if (fcmp(&obj1, &obj2) == 0) {                      \
            VNUT_YCT_PRINT_MAIN("FAILED");                  \
            if (p_yct_ctx_->out != NULL) {                  \
                VNUT_YCT_FPUTS("(");                        \
                VNUT_YCT_FPUTS(#obj1);                      \
                VNUT_YCT_FPUTS(" != ");                     \
                VNUT_YCT_FPUTS(#obj2);                      \
                VNUT_YCT_FPUTS(") { \"");                   \
                VNUT_YCT_FPUTS(msg);                        \
                VNUT_YCT_FPUTS("\" }\n");                   \
            }                                               \
            p_yct_ctx_->failed = 1;                         \
            VNUT_YCT_IF_SET_BLOCKED();                      \
            VNUT_YCT_RETURN();                              \
        }                                                   \
    } } while (0)

#define YCT_ASSERT_EQUAL_FLOAT(expected_float, actual_float, tolerance)     \
    do { VNUT_YCT_IF_OK {                                                   \
        p_yct_ctx_->assertions++;                                           \
        VNUT_YCT_LOG(expected_float == actual_float);                       \
        if (VNUT_YCT_ABS((expected_float) - (actual_float)) > tolerance) {  \
            VNUT_YCT_PRINT("FAILED", #expected_float " == " #actual_float); \
            if (p_yct_ctx_->out != NULL)                                    \
                VNUT_YCT_FPUTS("\n");                                       \
            p_yct_ctx_->failed = 1;                                         \
            VNUT_YCT_IF_SET_BLOCKED();                                      \
            VNUT_YCT_RETURN();                                              \
        }                                                                   \
    } } while (0)

#define YCT_ASSERT_EQUAL_FLOAT_MSG(exp_float, act_float, tol, msg)           \
    do { VNUT_YCT_IF_OK {                                                    \
        p_yct_ctx_->assertions++;                                            \
        VNUT_YCT_LOG(exp_float == act_float);                                \
        if (VNUT_YCT_ABS((exp_float) - (act_float)) > tol) {                 \
            VNUT_YCT_PRINT_MSG("FAILED", #exp_float " == " #act_float, msg); \
            p_yct_ctx_->failed = 1;                                          \
            VNUT_YCT_IF_SET_BLOCKED();                                       \
            VNUT_YCT_RETURN();                                               \
        }                                                                    \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_FLOAT(a, b, t)           \
    do { VNUT_YCT_IF_OK {                             \
        p_yct_ctx_->assertions++;                     \
        VNUT_YCT_LOG(a != b);                         \
        if (VNUT_YCT_ABS((a) - (b)) > t) { ; } else { \
            VNUT_YCT_PRINT("FAILED", #a " != " #b);   \
            if (p_yct_ctx_->out != NULL)              \
                VNUT_YCT_FPUTS("\n");                 \
            p_yct_ctx_->failed = 1;                   \
            VNUT_YCT_IF_SET_BLOCKED();                \
            VNUT_YCT_RETURN();                        \
        }                                             \
    } } while (0)

#define YCT_ASSERT_NOT_EQUAL_FLOAT_MSG(a, b, t, msg)         \
    do { VNUT_YCT_IF_OK {                                    \
        p_yct_ctx_->assertions++;                            \
        VNUT_YCT_LOG(a != b);                                \
        if (VNUT_YCT_ABS((a) - (b)) > t) { ; } else {        \
            VNUT_YCT_PRINT_MSG("FAILED", #a " != " #b, msg); \
            p_yct_ctx_->failed = 1;                          \
            VNUT_YCT_IF_SET_BLOCKED();                       \
            VNUT_YCT_RETURN();                               \
        }                                                    \
    } } while (0)

#ifdef __cplusplus
}
#endif

#endif /* YACUT_H_ */
