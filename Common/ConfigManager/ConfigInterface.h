#ifndef CONFIG_INTERFACE_H
#define CONFIG_INTERFACE_H

#include "stdint.h"

typedef enum {
    CONFIG_OK,
    CONFIG_ERR_NULL_POINTER,
    CONFIG_ERR_MODULE_NOT_FOUND,
    CONFIG_ERR_PARAM_NOT_FOUND,
    CONFIG_ERR_READ_NOT_ALLOWED,
    CONFIG_ERR_WRITE_NOT_ALLOWED,
    CONFIG_ERR_INVALID_VALUE
} config_status_t;

typedef enum {
    PARAM_TYPE_BOOL
} param_type_t;

typedef enum {
    PARAM_FLAG_READ  = (1u << 0),
    PARAM_FLAG_WRITE = (1u << 1)
} param_flags_t;

typedef struct {
    uint16_t id;
    param_type_t  type;
    uint8_t flags;

    void * address;
    void * maxVal;
    void * minVal;
} module_param_t;

typedef struct {
    uint16_t id;
    void * context;
    const module_param_t *params;
    uint8_t params_count;
} module_t;

#endif