#ifndef R_SCI_RX_IF_H
#define R_SCI_RX_IF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SCI_SUCCESS = 0,
    SCI_ERR_XMIT_EOT,
    SCI_ERR_BUSY,
    SCI_ERR_BAD_MODE,
    SCI_ERR_INSUFFICIENT_SPACE
} sci_err_t;

typedef void* sci_hdl_t;

typedef enum {
    SCI_CLK_INT = 0
} sci_clk_t;

typedef enum {
    SCI_DATA_8BIT = 0
} sci_data_size_t;

typedef enum {
    SCI_PARITY_OFF = 0
} sci_parity_en_t;

typedef enum {
    SCI_EVEN_PARITY = 0
} sci_parity_type_t;

typedef enum {
    SCI_STOPBITS_1 = 0
} sci_stopbits_t;

typedef enum {
    SCI_CH5 = 5
} sci_ch_t;

typedef enum {
    SCI_MODE_ASYNC = 0
} sci_mode_t;

typedef enum {
    SCI_EVT_TEI = 0
} sci_cb_evt_t;

typedef struct {
    sci_cb_evt_t event;
} sci_cb_args_t;

typedef struct {
    struct {
        uint32_t baud_rate;
        sci_clk_t clk_src;
        sci_data_size_t data_size;
        sci_parity_en_t parity_en;
        sci_parity_type_t parity_type;
        sci_stopbits_t stop_bits;
        uint8_t int_priority;
    } async;
} sci_cfg_t;

extern sci_hdl_t Console;
extern sci_err_t g_mock_sci_send_return_val;
extern uint8_t   g_mock_sci_last_sent_buf[512];
extern uint16_t  g_mock_sci_last_sent_len;
extern int       g_mock_sci_send_calls;

sci_err_t R_SCI_Send(sci_hdl_t const hdl, uint8_t *p_src, uint16_t const length);

static inline sci_err_t R_SCI_Open(sci_ch_t const ch, sci_mode_t const mode, sci_cfg_t * const p_cfg,
                                   void (* const p_cb)(void *p_args), sci_hdl_t * const p_hdl)
{
    (void)ch; (void)mode; (void)p_cfg; (void)p_cb;
    if (p_hdl) *p_hdl = Console;
    return SCI_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* R_SCI_RX_IF_H */
