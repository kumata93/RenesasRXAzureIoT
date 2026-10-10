#ifndef NX_API_H
#define NX_API_H

#include "tx_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NX_SUCCESS                              (0x00)
#define NX_TRUE                                 (1)
#define NX_FALSE                                (0)
#define NX_NULL                                 ((void*)0)
#define NX_WAIT_FOREVER                         (0xFFFFFFFFUL)
#define NX_IP_PERIODIC_RATE                     (100)

#define NX_AZURE_IOT_NOT_INITIALIZED            (0x00000001)
#define NX_AZURE_IOT_SAS_TOKEN_EXPIRED          (0x00000002)
#define NX_DNS_QUERY_FAILED                     (0x000000A0)
#define NXD_MQTT_ERROR_BAD_USERNAME_PASSWORD    (0x00000005)
#define NXD_MQTT_ERROR_NOT_AUTHORIZED           (0x00000006)

typedef struct NX_IP_STRUCT {
    ULONG dummy;
} NX_IP;

typedef struct NX_AZURE_IOT_HUB_CLIENT_STRUCT {
    ULONG dummy;
} NX_AZURE_IOT_HUB_CLIENT;

/* Mock verification hooks */
extern UINT g_mock_hub_connect_return_val;
extern int  g_mock_hub_connect_calls;
extern int  g_mock_hub_disconnect_calls;
extern int  g_mock_hub_deinit_calls;
extern UINT g_mock_gw_address_get_return_val;
extern int  g_mock_gw_address_get_calls;

static inline UINT nx_azure_iot_hub_client_connect(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr, UINT clean_session, ULONG wait_option) {
    (void)hub_client_ptr; (void)clean_session; (void)wait_option;
    g_mock_hub_connect_calls++;
    return g_mock_hub_connect_return_val;
}

static inline UINT nx_azure_iot_hub_client_disconnect(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr) {
    (void)hub_client_ptr;
    g_mock_hub_disconnect_calls++;
    return NX_SUCCESS;
}

static inline UINT nx_azure_iot_hub_client_deinitialize(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr) {
    (void)hub_client_ptr;
    g_mock_hub_deinit_calls++;
    return NX_SUCCESS;
}

static inline UINT nx_ip_gateway_address_get(NX_IP *ip_ptr, ULONG *gateway_address) {
    (void)ip_ptr;
    if (gateway_address) *gateway_address = 0xC0A80101; /* 192.168.1.1 */
    g_mock_gw_address_get_calls++;
    return g_mock_gw_address_get_return_val;
}

#ifdef __cplusplus
}
#endif

#endif /* NX_API_H */
