/* basic variables, T13.714-T13.895 $DVS:time$ */

#ifndef QT_XDAGMAIN_H
#define QT_XDAGMAIN_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_XDAG_ADDRESS_LEN 32
#define MAX_XDAG_BANLANCE_LEN 32
#define MAX_XDAG_STATE_LEN 1024
#define MAX_XDAG_ERR_MSG_LEN 1024
#define MAX_XDAG_LOG_BUF_SIZE 4096

typedef enum {
    en_procedure_init_wallet    = 0,
    en_procedure_xfer_coin      = 1
} en_xdag_procedure_type;

typedef enum {
    en_event_type_pwd               = 0,
    en_event_set_pwd                = 1,
    en_event_retype_pwd             = 2,
    en_event_set_rdm                = 3,
    en_event_pwd_not_same           = 4,
    en_event_pwd_error              = 5,
    en_event_update_progress        = 6,
    en_event_make_block_error       = 7,
    en_event_update_state           = 8,
    en_event_open_dnetfile_error    = 9,
    en_event_open_walletfile_error  = 10,
    en_event_load_storage_error     = 11,

    en_event_nothing_transfer       = 12,
    en_event_balance_too_small      = 13,
    en_event_incorrect_address      = 14,
    en_event_xdag_transfered        = 15,

    // invoke print log function
    en_event_xdag_log_print         = 99,
    en_event_unkown                 = 100,
} en_xdag_event_type;

typedef enum tag_en_xdag_app_log_level{
    en_xdag_no_error,
    en_xdag_fatal,
    en_xdag_critical,
    en_xdag_internal,
    en_xdag_error,
    en_xdag_warning,
    en_xdag_message,
    en_xdag_info,
    en_xdag_debug,
    en_xdag_trace
}en_xdag_app_log_level;

typedef struct {
    en_xdag_procedure_type  procedure_type;
    en_xdag_event_type      event_type;
    en_xdag_app_log_level   log_level;
    char error_msg[MAX_XDAG_ERR_MSG_LEN + 1];
    char address[MAX_XDAG_ADDRESS_LEN + 1];
    char balance[MAX_XDAG_BANLANCE_LEN + 1];
    char state[MAX_XDAG_STATE_LEN + 1];
    char *app_log_msg;                  //always point to the xdag log buffer g_xdag_app_log_buff
} st_xdag_event;

typedef struct {
    char    *xdag_pwd;
    char    *xdag_retype_pwd;
    char    *xdag_rdm;
    char    *xdag_xfer_account;
    char    *xdag_xfer_num;
    char    *xdag_state;
    char    *xdag_balance;
}st_xdag_app_msg;

/* callback object and callback func for app ui */
extern st_xdag_app_msg* (*g_app_callback_func)(const void* callback_object,st_xdag_event *event);
extern void* g_callback_object;

extern st_xdag_app_msg* xdag_malloc_app_msg();
extern void xdag_free_app_msg(st_xdag_app_msg* msg);

extern void xdag_wrapper_log_init();

extern const char* xdag_get_version();

extern void xdag_wrapper_init(const void* callback_object,
                              st_xdag_app_msg* (*callback_func)(const void* callback_object,st_xdag_event *event));

extern void xdag_send_coin(const char* amount,const char* address);

extern void xdag_wrapper_uninit();

#ifdef __cplusplus
}
#endif

#endif
