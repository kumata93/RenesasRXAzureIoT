#include "unity.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "tx_api.h"
#include "r_sci_rx_if.h"

/* Mock state definitions */
int g_mock_mutex_get_calls = 0;
int g_mock_mutex_put_calls = 0;
int g_mock_semaphore_get_calls = 0;
int g_mock_semaphore_put_calls = 0;
ULONG g_mock_semaphore_last_wait_option = 0;
UINT g_mock_thread_sleep_calls = 0;
ULONG g_mock_thread_sleep_last_ticks = 0;

sci_hdl_t Console = (sci_hdl_t)0x12345678;
sci_err_t g_mock_sci_send_return_val = SCI_SUCCESS;
uint8_t   g_mock_sci_last_sent_buf[512];
uint16_t  g_mock_sci_last_sent_len = 0;
int       g_mock_sci_send_calls = 0;

sci_err_t R_SCI_Send(sci_hdl_t const hdl, uint8_t *p_src, uint16_t const length)
{
    (void)hdl;
    g_mock_sci_send_calls++;
    g_mock_sci_last_sent_len = length;
    if (length <= sizeof(g_mock_sci_last_sent_buf))
    {
        memcpy(g_mock_sci_last_sent_buf, p_src, length);
    }
    return g_mock_sci_send_return_val;
}

/* Include target file directly */
#include "../src/driver/demo_printf.c"

void setUp(void)
{
    demo_printf_init();
    g_mock_mutex_get_calls = 0;
    g_mock_mutex_put_calls = 0;
    g_mock_semaphore_get_calls = 0;
    g_mock_semaphore_put_calls = 0;
    g_mock_semaphore_last_wait_option = 0;
    g_mock_sci_send_calls = 0;
    g_mock_sci_last_sent_len = 0;
    memset(g_mock_sci_last_sent_buf, 0, sizeof(g_mock_sci_last_sent_buf));
    g_mock_sci_send_return_val = SCI_SUCCESS;
}

void tearDown(void)
{
}

/* Test 1: Normal string output within buffer size */
void test_demo_printf_normal_output(void)
{
    const char *msg = "System Booting... Code = %d";
    demo_printf((char *)msg, 100);

    TEST_ASSERT_EQUAL_INT(1, g_mock_mutex_get_calls);
    TEST_ASSERT_EQUAL_INT(1, g_mock_mutex_put_calls);
    TEST_ASSERT_EQUAL_INT(1, g_mock_sci_send_calls);
    TEST_ASSERT_EQUAL_INT(1, g_mock_semaphore_get_calls);

    /* Verify sent length and payload */
    char expected[64];
    snprintf(expected, sizeof(expected), msg, 100);
    TEST_ASSERT_EQUAL_UINT(strlen(expected), g_mock_sci_last_sent_len);
    TEST_ASSERT_EQUAL_STRING(expected, (char *)g_mock_sci_last_sent_buf);
}

/* Test 2 (UT-01): 256+ bytes string clipping and null termination */
void test_demo_printf_clip_over_256_bytes(void)
{
    /* Create a long test string (300 characters) */
    char long_input[320];
    memset(long_input, 'X', 300);
    long_input[300] = '\0';

    demo_printf("%s", long_input);

    /* Must clip to sizeof(demo_printf_buf) - 1 = 256 */
    TEST_ASSERT_EQUAL_UINT(256, g_mock_sci_last_sent_len);

    /* Buffer must be null-terminated at index 256 */
    TEST_ASSERT_EQUAL_HEX8('\0', demo_printf_buf[256]);

    /* First and last sent bytes must be 'X' */
    TEST_ASSERT_EQUAL_HEX8('X', g_mock_sci_last_sent_buf[0]);
    TEST_ASSERT_EQUAL_HEX8('X', g_mock_sci_last_sent_buf[255]);
}

/* Test 3 (UT-02): SCI send error skips semaphore to avoid deadlock */
void test_demo_printf_sci_send_error_skips_semaphore(void)
{
    /* Simulate SCI transmission error (e.g. BUSY) */
    g_mock_sci_send_return_val = SCI_ERR_BUSY;

    demo_printf("Testing error handling\n");

    /* R_SCI_Send must have been called */
    TEST_ASSERT_EQUAL_INT(1, g_mock_sci_send_calls);

    /* Semaphore wait MUST be skipped to prevent deadlock */
    TEST_ASSERT_EQUAL_INT(0, g_mock_semaphore_get_calls);

    /* Mutex MUST be released */
    TEST_ASSERT_EQUAL_INT(1, g_mock_mutex_get_calls);
    TEST_ASSERT_EQUAL_INT(1, g_mock_mutex_put_calls);
}

/* Test 4 (UT-03): Semaphore wait option has 1 second timeout (not TX_WAIT_FOREVER) */
void test_demo_printf_semaphore_timeout_setting(void)
{
    g_mock_sci_send_return_val = SCI_SUCCESS;

    demo_printf("Checking timeout parameter\n");

    TEST_ASSERT_EQUAL_INT(1, g_mock_semaphore_get_calls);
    /* Should use TX_TIMER_TICKS_PER_SECOND (100 ticks = 1 sec), not TX_WAIT_FOREVER */
    TEST_ASSERT_EQUAL_HEX32(TX_TIMER_TICKS_PER_SECOND, g_mock_semaphore_last_wait_option);
}

/* Test 5: Single character output function */
void test_my_sw_charput_function(void)
{
    my_sw_charput_function('Z');

    TEST_ASSERT_EQUAL_INT(1, g_mock_sci_send_calls);
    TEST_ASSERT_EQUAL_UINT(1, g_mock_sci_last_sent_len);
    TEST_ASSERT_EQUAL_HEX8('Z', g_mock_sci_last_sent_buf[0]);
    TEST_ASSERT_EQUAL_INT(1, g_mock_semaphore_get_calls);
    TEST_ASSERT_EQUAL_INT(1, g_mock_mutex_put_calls);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_demo_printf_normal_output);
    RUN_TEST(test_demo_printf_clip_over_256_bytes);
    RUN_TEST(test_demo_printf_sci_send_error_skips_semaphore);
    RUN_TEST(test_demo_printf_semaphore_timeout_setting);
    RUN_TEST(test_my_sw_charput_function);
    return UNITY_END();
}
