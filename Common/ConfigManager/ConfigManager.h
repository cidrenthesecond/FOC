#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "ConfigInterface.h"

config_status_t config_get(uint16_t module_id, uint16_t param_id, void * value);


#endif