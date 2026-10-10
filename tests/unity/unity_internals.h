#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int UNITY_INT;
typedef unsigned int UNITY_UINT;
typedef int8_t UNITY_INT8;
typedef uint8_t UNITY_UINT8;
typedef int16_t UNITY_INT16;
typedef uint16_t UNITY_UINT16;
typedef int32_t UNITY_INT32;
typedef uint32_t UNITY_UINT32;

typedef enum {
    UNITY_DISPLAY_STYLE_INT = 0,
    UNITY_DISPLAY_STYLE_UINT,
    UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_HEX32
} UNITY_DISPLAY_STYLE_T;

struct UNITY_STORAGE_T {
    const char* TestFile;
    const char* CurrentTestName;
    UNITY_UINT CurrentTestLineNumber;
    UNITY_UINT NumberOfTests;
    UNITY_UINT TestFailures;
    UNITY_UINT TestIgnores;
    UNITY_UINT CurrentTestFailed;
    UNITY_UINT CurrentTestIgnored;
    jmp_buf AbortFrame;
};

extern struct UNITY_STORAGE_T Unity;

#define UNITY_TEST_FAIL() UnityTestFail()
#define UNITY_OUTPUT_CHAR(a) putchar(a)

void UnityBegin(const char* filename);
int  UnityEnd(void);
void UnityTestFail(void);
void UnityConcludeTest(void);
void UnityDefaultTestRun(void (*Func)(void), const char* FuncName, const int FuncLineNum);

void UnityAssertEqualNumber(const UNITY_INT32 expected,
                            const UNITY_INT32 actual,
                            const char* msg,
                            const UNITY_UINT lineNumber,
                            const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_UINT lineNumber);

void UnityAssertNumbersGreater(const UNITY_INT32 threshold,
                               const UNITY_INT32 actual,
                               const char* msg,
                               const UNITY_UINT lineNumber,
                               const UNITY_DISPLAY_STYLE_T style);

void UnityAssertNumbersLessOrEqual(const UNITY_INT32 threshold,
                                   const UNITY_INT32 actual,
                                   const char* msg,
                                   const UNITY_UINT lineNumber,
                                   const UNITY_DISPLAY_STYLE_T style);

void UnityAssertTrue(const int condition,
                     const char* msg,
                     const UNITY_UINT lineNumber);

#ifdef __cplusplus
}
#endif

#endif /* UNITY_INTERNALS_H */
