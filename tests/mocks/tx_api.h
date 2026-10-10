#ifndef TX_API_H
#define TX_API_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t    ULONG;
typedef uint16_t    USHORT;
typedef uint8_t     UCHAR;
typedef uint32_t    UINT;
typedef int32_t     INT;
typedef char        CHAR;
typedef void        VOID;

#define TX_SUCCESS                  (0x00)
#define TX_WAIT_FOREVER             (0xFFFFFFFFUL)
#define TX_NO_WAIT                  (0x00000000UL)
#define TX_INHERIT                  (1)
#define TX_TIMER_TICKS_PER_SECOND   (100)

typedef struct TX_MUTEX_STRUCT {
    ULONG tx_mutex_count;
} TX_MUTEX;

typedef struct TX_SEMAPHORE_STRUCT {
    ULONG tx_semaphore_count;
} TX_SEMAPHORE;

/* Mock function pointers & counters for verification */
extern int g_mock_mutex_get_calls;
extern int g_mock_mutex_put_calls;
extern int g_mock_semaphore_get_calls;
extern int g_mock_semaphore_put_calls;
extern ULONG g_mock_semaphore_last_wait_option;
extern UINT g_mock_thread_sleep_calls;
extern ULONG g_mock_thread_sleep_last_ticks;

static inline UINT tx_mutex_create(TX_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit) {
    (void)mutex_ptr; (void)name_ptr; (void)inherit;
    return TX_SUCCESS;
}

static inline UINT tx_mutex_get(TX_MUTEX *mutex_ptr, ULONG wait_option) {
    (void)mutex_ptr; (void)wait_option;
    g_mock_mutex_get_calls++;
    return TX_SUCCESS;
}

static inline UINT tx_mutex_put(TX_MUTEX *mutex_ptr) {
    (void)mutex_ptr;
    g_mock_mutex_put_calls++;
    return TX_SUCCESS;
}

static inline UINT tx_semaphore_create(TX_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count) {
    (void)semaphore_ptr; (void)name_ptr; (void)initial_count;
    return TX_SUCCESS;
}

static inline UINT tx_semaphore_get(TX_SEMAPHORE *semaphore_ptr, ULONG wait_option) {
    (void)semaphore_ptr;
    g_mock_semaphore_get_calls++;
    g_mock_semaphore_last_wait_option = wait_option;
    return TX_SUCCESS;
}

static inline UINT tx_semaphore_put(TX_SEMAPHORE *semaphore_ptr) {
    (void)semaphore_ptr;
    g_mock_semaphore_put_calls++;
    return TX_SUCCESS;
}

static inline UINT tx_thread_sleep(ULONG timer_ticks) {
    g_mock_thread_sleep_calls++;
    g_mock_thread_sleep_last_ticks = timer_ticks;
    return TX_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* TX_API_H */
