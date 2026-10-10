#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nx_api.h"

/* Mock state definitions */
int g_mock_mutex_get_calls = 0;
int g_mock_mutex_put_calls = 0;
int g_mock_semaphore_get_calls = 0;
int g_mock_semaphore_put_calls = 0;
ULONG g_mock_semaphore_last_wait_option = 0;
UINT g_mock_thread_sleep_calls = 0;
ULONG g_mock_thread_sleep_last_ticks = 0;

UINT g_mock_hub_connect_return_val = NX_SUCCESS;
int  g_mock_hub_connect_calls = 0;
int  g_mock_hub_disconnect_calls = 0;
int  g_mock_hub_deinit_calls = 0;
UINT g_mock_gw_address_get_return_val = NX_SUCCESS;
int  g_mock_gw_address_get_calls = 0;

/* Include target file directly to inspect/test static functions and variables */
#include "../src/app/sample_azure_iot_embedded_sdk_connect.c"

static UINT dummy_iothub_init(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr)
{
    (void)hub_client_ptr;
    return NX_SUCCESS;
}

void setUp(void)
{
    exponential_backoff_reset();
    g_mock_thread_sleep_calls = 0;
    g_mock_thread_sleep_last_ticks = 0;
    g_mock_hub_connect_calls = 0;
    g_mock_hub_disconnect_calls = 0;
    g_mock_hub_deinit_calls = 0;
    g_mock_gw_address_get_calls = 0;
    g_mock_hub_connect_return_val = NX_SUCCESS;
    g_mock_gw_address_get_return_val = NX_SUCCESS;
}

void tearDown(void)
{
}

/* Test 1: Normal backoff scaling with retries */
void test_exponential_backoff_normal_scaling(void)
{
    /* Retry 0: base is 3 sec (300 ticks), with 0-60% jitter -> 300..480 ticks */
    UINT delay0 = exponential_backoff_with_jitter();
    TEST_ASSERT_TRUE(delay0 >= (3 * NX_IP_PERIODIC_RATE));
    TEST_ASSERT_TRUE(delay0 <= (UINT)(3 * 1.6 * NX_IP_PERIODIC_RATE + 1));
    TEST_ASSERT_EQUAL_UINT(1, exponential_retry_count);

    /* Retry 1: base is 6 sec (600 ticks), with 0-60% jitter -> 600..960 ticks */
    UINT delay1 = exponential_backoff_with_jitter();
    TEST_ASSERT_TRUE(delay1 >= (6 * NX_IP_PERIODIC_RATE));
    TEST_ASSERT_TRUE(delay1 <= (UINT)(6 * 1.6 * NX_IP_PERIODIC_RATE + 1));
    TEST_ASSERT_EQUAL_UINT(2, exponential_retry_count);

    /* Retry 2: base is 12 sec (1200 ticks), with 0-60% jitter -> 1200..1920 ticks */
    UINT delay2 = exponential_backoff_with_jitter();
    TEST_ASSERT_TRUE(delay2 >= (12 * NX_IP_PERIODIC_RATE));
    TEST_ASSERT_TRUE(delay2 <= (UINT)(12 * 1.6 * NX_IP_PERIODIC_RATE + 1));
    TEST_ASSERT_EQUAL_UINT(3, exponential_retry_count);
}

/* Test 2 (UT-06): Prevention of 32-bit shift overflow when retry_count >= 31 */
void test_exponential_backoff_shift_overflow_prevention(void)
{
    UINT delay;

    /* Simulate high retry count: 31 */
    exponential_retry_count = 31;
    delay = exponential_backoff_with_jitter();
    /* Should clip retry count to 8 and cap delay to MAX_DELAY (600s = 60000 ticks) + jitter */
    TEST_ASSERT_TRUE(delay >= (SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC * NX_IP_PERIODIC_RATE));
    TEST_ASSERT_TRUE(delay <= (UINT)(SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC * 1.6 * NX_IP_PERIODIC_RATE + 1));

    /* Simulate high retry count: 32 */
    exponential_retry_count = 32;
    delay = exponential_backoff_with_jitter();
    TEST_ASSERT_TRUE(delay >= (SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC * NX_IP_PERIODIC_RATE));
    TEST_ASSERT_TRUE(delay <= (UINT)(SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC * 1.6 * NX_IP_PERIODIC_RATE + 1));

    /* Simulate extreme retry count: 64 */
    exponential_retry_count = 64;
    delay = exponential_backoff_with_jitter();
    TEST_ASSERT_TRUE(delay >= (SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC * NX_IP_PERIODIC_RATE));
    TEST_ASSERT_TRUE(delay <= (UINT)(SAMPLE_MAX_EXPONENTIAL_BACKOFF_IN_SEC * 1.6 * NX_IP_PERIODIC_RATE + 1));
}

/* Test 3 (UT-07): Jitter distribution stays within 0% to 60% of base */
void test_exponential_backoff_jitter_distribution(void)
{
    const UINT min_expected = 3 * NX_IP_PERIODIC_RATE;
    const UINT max_expected = (UINT)(3 * 1.6 * NX_IP_PERIODIC_RATE);

    for (int i = 0; i < 50; i++)
    {
        exponential_retry_count = 0;
        UINT delay = exponential_backoff_with_jitter();
        TEST_ASSERT_TRUE(delay >= min_expected);
        TEST_ASSERT_TRUE(delay <= (max_expected + 1));
    }
}

/* Test 4 (UT-08): Connection monitor propagates status to caller */
void test_connection_monitor_propagate_status(void)
{
    NX_IP ip;
    NX_AZURE_IOT_HUB_CLIENT hub_client;
    UINT status = NX_AZURE_IOT_NOT_INITIALIZED;

    /* When connection attempt succeeds (NX_SUCCESS) */
    g_mock_hub_connect_return_val = NX_SUCCESS;

    sample_connection_monitor(&ip, &hub_client, &status, dummy_iothub_init);

    /* Verify that *connection_status_ptr was updated to NX_SUCCESS */
    TEST_ASSERT_EQUAL_UINT(NX_SUCCESS, status);
    TEST_ASSERT_TRUE(g_mock_hub_connect_calls >= 1);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_exponential_backoff_normal_scaling);
    RUN_TEST(test_exponential_backoff_shift_overflow_prevention);
    RUN_TEST(test_exponential_backoff_jitter_distribution);
    RUN_TEST(test_connection_monitor_propagate_status);
    return UNITY_END();
}
