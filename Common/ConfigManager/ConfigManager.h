#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "ConfigInterface.h"

typedef enum {
    CONFIG_OK,
    CONFIG_ERR_INIT_PARAM_ID_NOT_SET,
    CONFIG_ERR_INIT_PARAM_TYPE_NOT_SET,
    CONFIG_ERR_INIT_PARAM_TYPE_NOT_SUPPORTED,
    CONFIG_ERR_INIT_PARAM_NO_FLAGS,
    CONFIG_ERR_INIT_PARAM_SIZE_NOT_SET,
    CONFIG_ERR_INIT_PARAM_ADDR_NOT_SET,
    CONFIG_ERR_INIT_MODULE_ID_NOT_SET,
    CONFIG_ERR_INIT_MODULE_CONTEXT_NOT_SET,
    CONFIG_ERR_INIT_MODULE_PARAM_PTR_NOT_SET,
    CONFIG_ERR_INIT_MODULE_PARAM_COUNT_NOT_SET,
    CONFIG_ERR_MODULE_NOT_FOUND,
    CONFIG_ERR_PARAM_NOT_FOUND,
    CONFIG_ERR_READ_NOT_ALLOWED,
    CONFIG_ERR_WRITE_NOT_ALLOWED,
    CONFIG_ERR_INVALID_VALUE
} config_status_t;

config_status_t ConfigManager_Setup(const module_t *const *SetModuleList, uint8_t count);     
config_status_t Config_get(uint16_t module_id, uint16_t param_id, void * value);
config_status_t Config_set(uint16_t module_id, uint16_t param_id, void * value);
void            ConfigManager_Destroy();


#endif