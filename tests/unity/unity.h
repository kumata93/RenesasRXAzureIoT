#ifndef UNITY_H
#define UNITY_H

#include "unity_internals.h"

#ifdef __cplusplus
extern "C" {
#endif

void setUp(void);
void tearDown(void);

#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END()   UnityEnd()

#define RUN_TEST(func) UnityDefaultTestRun(func, #func, __LINE__)

#define TEST_ASSERT_TRUE(condition) \
    UnityAssertTrue((condition) ? 1 : 0, #condition, __LINE__)

#define TEST_ASSERT_FALSE(condition) \
    UnityAssertTrue((condition) ? 0 : 1, #condition, __LINE__)

#define TEST_ASSERT_EQUAL(expected, actual) \
    UnityAssertEqualNumber((UNITY_INT32)(expected), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UnityAssertEqualNumber((UNITY_INT32)(expected), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_EQUAL_UINT(expected, actual) \
    UnityAssertEqualNumber((UNITY_INT32)(expected), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_UINT)

#define TEST_ASSERT_EQUAL_HEX32(expected, actual) \
    UnityAssertEqualNumber((UNITY_INT32)(expected), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_HEX32)

#define TEST_ASSERT_EQUAL_HEX8(expected, actual) \
    UnityAssertEqualNumber((UNITY_INT32)(expected), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_HEX8)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    UnityAssertEqualString((const char*)(expected), (const char*)(actual), NULL, __LINE__)

#define TEST_ASSERT_GREATER_THAN(threshold, actual) \
    UnityAssertNumbersGreater((UNITY_INT32)(threshold), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_LESS_OR_EQUAL(threshold, actual) \
    UnityAssertNumbersLessOrEqual((UNITY_INT32)(threshold), (UNITY_INT32)(actual), NULL, __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_UINT_WITHIN(delta, expected, actual) \
    UnityAssertTrue(((actual) >= ((expected) - (delta))) && ((actual) <= ((expected) + (delta))), "Value out of range", __LINE__)

#define TEST_FAIL_MESSAGE(message) \
    do { UnityAssertTrue(0, message, __LINE__); } while (0)

#ifdef __cplusplus
}
#endif

#endif /* UNITY_H */
