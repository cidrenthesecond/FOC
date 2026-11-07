#ifndef CONFIG_INTERFACE_H
#define CONFIG_INTERFACE_H

#include "stdint.h"

typedef enum {
    PARAM_TYPE_NOT_SET,
    PARAM_TYPE_BOOL,
    PARAM_TYPE_INT_8,
    PARAM_TYPE_INT_16,
    PARAM_TYPE_INT_32,
    PARAM_TYPE_UINT_8,
    PARAM_TYPE_UINT_16,
    PARAM_TYPE_UINT_32,
    PARAM_TYPE_FLOAT,
    PARAM_NUM_HANDLED
} param_type_t;

typedef enum {
    PARAM_FLAG_NONE  = 0u,
    PARAM_FLAG_READ  = (1u << 0),
    PARAM_FLAG_WRITE = (1u << 1)
} param_flags_t;

typedef struct {
    uint16_t id;
    param_type_t type;
    uint8_t flags;
    uint8_t paramSize;

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