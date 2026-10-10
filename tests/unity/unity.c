#include "unity.h"
#include <string.h>

struct UNITY_STORAGE_T Unity;

void UnityBegin(const char* filename)
{
    Unity.TestFile = filename;
    Unity.CurrentTestName = NULL;
    Unity.CurrentTestLineNumber = 0;
    Unity.NumberOfTests = 0;
    Unity.TestFailures = 0;
    Unity.TestIgnores = 0;
    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
    printf("---------------------------------------\n");
    printf("Starting Tests in: %s\n", filename);
    printf("---------------------------------------\n");
}

int UnityEnd(void)
{
    printf("---------------------------------------\n");
    printf("%u Tests %u Failures %u Ignored\n",
           Unity.NumberOfTests, Unity.TestFailures, Unity.TestIgnores);
    if (Unity.TestFailures == 0)
    {
        printf("OK\n\n");
        return 0;
    }
    else
    {
        printf("FAIL\n\n");
        return (int)Unity.TestFailures;
    }
}

void UnityTestFail(void)
{
    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;
    longjmp(Unity.AbortFrame, 1);
}

void UnityConcludeTest(void)
{
    if (!Unity.CurrentTestFailed)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

void UnityDefaultTestRun(void (*Func)(void), const char* FuncName, const int FuncLineNum)
{
    Unity.NumberOfTests++;
    Unity.CurrentTestName = FuncName;
    Unity.CurrentTestLineNumber = (UNITY_UINT)FuncLineNum;
    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;

    printf("%s:%d: %s: ", Unity.TestFile, FuncLineNum, FuncName);

    if (setjmp(Unity.AbortFrame) == 0)
    {
        setUp();
        Func();
    }
    tearDown();
    UnityConcludeTest();
}

void UnityAssertTrue(const int condition, const char* msg, const UNITY_UINT lineNumber)
{
    if (!condition)
    {
        printf("\n  Line %u: Assertion Failed. %s\n  ", lineNumber, msg ? msg : "");
        UnityTestFail();
    }
}

void UnityAssertEqualNumber(const UNITY_INT32 expected,
                            const UNITY_INT32 actual,
                            const char* msg,
                            const UNITY_UINT lineNumber,
                            const UNITY_DISPLAY_STYLE_T style)
{
    (void)style;
    if (expected != actual)
    {
        printf("\n  Line %u: Expected %d Was %d. %s\n  ",
               lineNumber, (int)expected, (int)actual, msg ? msg : "");
        UnityTestFail();
    }
}

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_UINT lineNumber)
{
    if (expected == NULL && actual == NULL) return;
    if (expected == NULL || actual == NULL || strcmp(expected, actual) != 0)
    {
        printf("\n  Line %u: Expected \"%s\" Was \"%s\". %s\n  ",
               lineNumber, expected ? expected : "NULL", actual ? actual : "NULL", msg ? msg : "");
        UnityTestFail();
    }
}

void UnityAssertNumbersGreater(const UNITY_INT32 threshold,
                               const UNITY_INT32 actual,
                               const char* msg,
                               const UNITY_UINT lineNumber,
                               const UNITY_DISPLAY_STYLE_T style)
{
    (void)style;
    if (actual <= threshold)
    {
        printf("\n  Line %u: Expected %d > %d. %s\n  ",
               lineNumber, (int)actual, (int)threshold, msg ? msg : "");
        UnityTestFail();
    }
}

void UnityAssertNumbersLessOrEqual(const UNITY_INT32 threshold,
                                   const UNITY_INT32 actual,
                                   const char* msg,
                                   const UNITY_UINT lineNumber,
                                   const UNITY_DISPLAY_STYLE_T style)
{
    (void)style;
    if (actual > threshold)
    {
        printf("\n  Line %u: Expected %d <= %d. %s\n  ",
               lineNumber, (int)actual, (int)threshold, msg ? msg : "");
        UnityTestFail();
    }
}
