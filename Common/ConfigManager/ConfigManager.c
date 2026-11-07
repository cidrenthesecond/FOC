#include "ConfigManager.h"
#include "string.h"
#include "stdbool.h"

static const module_t *const *moduleList;
static uint8_t modules_count;

static const module_t       * FindModule(uint16_t module_id);
static const module_param_t * FindParameter(const module_t *searchedModule,uint16_t param_id);
static config_status_t        ValidateList(const module_t *const *SetModuleList, uint16_t modulesCount);

static config_status_t Bool_ValidationStrategy    (const module_param_t *parameter,void * value);
static config_status_t uint8t_ValidationStrategy  (const module_param_t *parameter,void * value);
static config_status_t uint16t_ValidationStrategy (const module_param_t *parameter,void * value);
static config_status_t uint32t_ValidationStrategy (const module_param_t *parameter,void * value);
static config_status_t int8t_ValidationStrategy   (const module_param_t *parameter,void * value);
static config_status_t int16t_ValidationStrategy  (const module_param_t *parameter,void * value);
static config_status_t int32t_ValidationStrategy  (const module_param_t *parameter,void * value);
static config_status_t float_ValidationStrategy   (const module_param_t *parameter,void * value);

static config_status_t (*ParameterValidationStrategies[PARAM_NUM_HANDLED])(const module_param_t *parameter,void * value) =
{
    [PARAM_TYPE_BOOL]    = Bool_ValidationStrategy,
    [PARAM_TYPE_UINT_8]  = uint8t_ValidationStrategy,
    [PARAM_TYPE_UINT_16] = uint16t_ValidationStrategy,
    [PARAM_TYPE_UINT_32] = uint32t_ValidationStrategy,
    [PARAM_TYPE_INT_8]   = int8t_ValidationStrategy,
    [PARAM_TYPE_INT_16]  = int16t_ValidationStrategy,
    [PARAM_TYPE_INT_32]  = int32t_ValidationStrategy,
    [PARAM_TYPE_FLOAT]   = float_ValidationStrategy
};

config_status_t ConfigManager_Setup(const module_t *const *SetModuleList, uint8_t count)
{
    config_status_t validationStatus = ValidateList(SetModuleList,count);
    if(validationStatus != CONFIG_OK) return validationStatus;

    moduleList    = SetModuleList;
    modules_count = count;

    return CONFIG_OK;
}  

config_status_t Config_get(uint16_t module_id, uint16_t param_id, void * value)
{
    const module_t       * foundModule    = NULL;
    const module_param_t * foundParameter = NULL;

    foundModule = FindModule(module_id);
    if(foundModule == NULL) return CONFIG_ERR_MODULE_NOT_FOUND;

    foundParameter = FindParameter(foundModule,param_id);
    if(foundParameter == NULL) return CONFIG_ERR_PARAM_NOT_FOUND;

    if(!(foundParameter->flags & PARAM_FLAG_READ))
        return CONFIG_ERR_READ_NOT_ALLOWED;

    memcpy(value,foundParameter->address,foundParameter->paramSize);

    return CONFIG_OK;
}

config_status_t Config_set(uint16_t module_id, uint16_t param_id, void * value)
{
    const module_t       * foundModule    = NULL;
    const module_param_t * foundParameter = NULL;

    foundModule = FindModule(module_id);
    if(foundModule == NULL) return CONFIG_ERR_MODULE_NOT_FOUND;

    foundParameter = FindParameter(foundModule, param_id);
    if(foundParameter == NULL) return CONFIG_ERR_PARAM_NOT_FOUND;

    if(!(foundParameter->flags & PARAM_FLAG_WRITE)) 
        return CONFIG_ERR_WRITE_NOT_ALLOWED;

    if(ParameterValidationStrategies[foundParameter->type](foundParameter,value) == CONFIG_ERR_INVALID_VALUE)
        return CONFIG_ERR_INVALID_VALUE;

    memcpy(foundParameter->address,value,foundParameter->paramSize);

    return CONFIG_OK;
}

static config_status_t ValidateList(const module_t *const *SetModuleList, uint16_t modulesCount)
{
    for(uint8_t i = 0; i < modulesCount;i++)
    {
        const module_t * searchedModule = SetModuleList[i];

        if(searchedModule->id           == 0)    return CONFIG_ERR_INIT_MODULE_ID_NOT_SET;
        if(searchedModule->context      == NULL) return CONFIG_ERR_INIT_MODULE_CONTEXT_NOT_SET;
        if(searchedModule->params       == NULL) return CONFIG_ERR_INIT_MODULE_PARAM_PTR_NOT_SET;
        if(searchedModule->params_count == 0)    return CONFIG_ERR_INIT_MODULE_PARAM_COUNT_NOT_SET;

        for(uint16_t j = 0; j < searchedModule->params_count;j++)
        {
            const module_param_t * searchedParameter = &searchedModule->params[j];

            if(searchedParameter->id        == 0)                  return CONFIG_ERR_INIT_PARAM_ID_NOT_SET;
            if(searchedParameter->type      == PARAM_TYPE_NOT_SET) return CONFIG_ERR_INIT_PARAM_TYPE_NOT_SET;
            if(searchedParameter->type      >= PARAM_NUM_HANDLED)  return CONFIG_ERR_INIT_PARAM_TYPE_NOT_SUPPORTED;
            if(searchedParameter->flags     == PARAM_FLAG_NONE)    return CONFIG_ERR_INIT_PARAM_NO_FLAGS;
            if(searchedParameter->paramSize == 0)                  return CONFIG_ERR_INIT_PARAM_SIZE_NOT_SET;
            if(searchedParameter->address   == NULL)               return CONFIG_ERR_INIT_PARAM_ADDR_NOT_SET;
        }
    }
    return CONFIG_OK;
}

static const module_t * FindModule(uint16_t module_id)
{
    const module_t * foundModule = NULL;

    for(uint8_t index = 0; index < modules_count; index++)
    {
        if(moduleList[index]->id == module_id)
        {
            foundModule = moduleList[index];
            break;
        }
    }

    return foundModule;
}

static const module_param_t * FindParameter(const module_t *searchedModule,uint16_t param_id)
{
    const module_param_t * foundParameter = NULL;

    for(uint8_t index = 0; index < searchedModule->params_count; index++)
    {
        const module_param_t *param = &searchedModule->params[index];
        if(param->id == param_id)
        {
            foundParameter = &searchedModule->params[index];
            break;
        }
    }

    return foundParameter;
}

void ConfigManager_Destroy()
{
    moduleList    = NULL;
    modules_count = 0;
}

static config_status_t Bool_ValidationStrategy(const module_param_t *parameter,void * value)
{
    (void)parameter;
    uint8_t val = *(uint8_t *)value;

    if (val != true && val != false) 
        return CONFIG_ERR_INVALID_VALUE;

    return CONFIG_OK;
}

static config_status_t uint8t_ValidationStrategy(const module_param_t *parameter,void * value)
{
    uint8_t val = *(uint8_t *)value;

    if(parameter->maxVal != NULL)
    {
        uint8_t castedMaxVal = *(uint8_t *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        uint8_t castedMinVal = *(uint8_t *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}

static config_status_t uint16t_ValidationStrategy(const module_param_t *parameter,void * value)
{
    uint16_t val = *(uint16_t *)value;

    if(parameter->maxVal != NULL)
    {
        uint16_t castedMaxVal = *(uint16_t *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        uint16_t castedMinVal = *(uint16_t *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}

static config_status_t uint32t_ValidationStrategy(const module_param_t *parameter,void * value)
{
    uint32_t val = *(uint32_t *)value;

    if(parameter->maxVal != NULL)
    {
        uint32_t castedMaxVal = *(uint32_t *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        uint32_t castedMinVal = *(uint32_t *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}

static config_status_t int8t_ValidationStrategy(const module_param_t *parameter,void * value)
{
    int8_t val = *(int8_t *)value;

    if(parameter->maxVal != NULL)
    {
        int8_t castedMaxVal = *(int8_t *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        int8_t castedMinVal = *(int8_t *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}

static config_status_t int16t_ValidationStrategy(const module_param_t *parameter,void * value)
{
    int16_t val = *(int16_t *)value;

    if(parameter->maxVal != NULL)
    {
        int16_t castedMaxVal = *(int16_t *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        int16_t castedMinVal = *(int16_t *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}

static config_status_t int32t_ValidationStrategy(const module_param_t *parameter,void * value)
{
    int32_t val = *(int32_t *)value;

    if(parameter->maxVal != NULL)
    {
        int32_t castedMaxVal = *(int32_t *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        int32_t castedMinVal = *(int32_t *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}

static config_status_t float_ValidationStrategy(const module_param_t *parameter,void * value)
{
    float val = *(float *)value;

    if(parameter->maxVal != NULL)
    {
        float castedMaxVal = *(float *)(parameter->maxVal);
        if(val > castedMaxVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    if(parameter->minVal != NULL)
    {
        float castedMinVal = *(float *)(parameter->minVal);
        if(val < castedMinVal)
            return CONFIG_ERR_INVALID_VALUE;
    }

    return CONFIG_OK;
}



