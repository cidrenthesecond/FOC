#include "FakeConfig.h"

FakeConfigStruct_t fakeConfig;

const uint32_t FAKE_UINT_UPPER_BOUND  = 125u; 
const uint32_t FAKE_UINT_LOWER_BOUND  = 25u;  
const int32_t  FAKE_INT_UPPER_BOUND   = 66u;  
const int32_t  FAKE_INT_LOWER_BOUND   = -67;
const float    FAKE_FLOAT_UPPER_BOUND = 1.25f;
const float    FAKE_FLOAT_LOWER_BOUND = -0.75f;

static const module_param_t fake_param_descriptor[] =
{
    {
        .id        = FAKE_BOOL_ID,
        .type      = PARAM_TYPE_BOOL,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeBooleanConfig,
        .paramSize = sizeof(fakeConfig.fakeBooleanConfig)
    },
    {
        .id        = FAKE_READONLY_BOOL_ID,
        .type      = PARAM_TYPE_BOOL,
        .flags     = PARAM_FLAG_READ,
        .address   = &fakeConfig.fakeBooleanConfig,
        .paramSize = sizeof(fakeConfig.fakeBooleanConfig)
    },
    {
        .id        = FAKE_WRITEONLY_BOOL_ID,
        .type      = PARAM_TYPE_BOOL,
        .flags     = PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeBooleanConfig,
        .paramSize = sizeof(fakeConfig.fakeBooleanConfig)
    },
    {
        .id        = FAKE_UNBOUNDED_UINT8_ID,
        .type      = PARAM_TYPE_UINT_8,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeUint8Config,
        .paramSize = sizeof(&fakeConfig.fakeUint8Config)
    },
    {
        .id        = FAKE_BOUNDED_UINT8_ID,
        .type      = PARAM_TYPE_UINT_8,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeUint8Config,
        .paramSize = sizeof(&fakeConfig.fakeUint16Config),
        .maxVal    = &FAKE_UINT_UPPER_BOUND,
        .minVal    = &FAKE_UINT_LOWER_BOUND
    },
    {
        .id        = FAKE_UNBOUNDED_UINT16_ID,
        .type      = PARAM_TYPE_UINT_16,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeUint16Config,
        .paramSize = sizeof(&fakeConfig.fakeUint16Config)
    },
    {
        .id        = FAKE_BOUNDED_UINT16_ID,
        .type      = PARAM_TYPE_UINT_16,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeUint16Config,
        .paramSize = sizeof(&fakeConfig.fakeUint16Config),
        .maxVal    = &FAKE_UINT_UPPER_BOUND,
        .minVal    = &FAKE_UINT_LOWER_BOUND
    },
        {
        .id        = FAKE_UNBOUNDED_UINT32_ID,
        .type      = PARAM_TYPE_UINT_32,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeUint32Config,
        .paramSize = sizeof(&fakeConfig.fakeUint32Config)
    },
    {
        .id        = FAKE_BOUNDED_UINT32_ID,
        .type      = PARAM_TYPE_UINT_32,
        .flags     = PARAM_FLAG_READ | PARAM_FLAG_WRITE,
        .address   = &fakeConfig.fakeUint32Config,
        .paramSize = sizeof(&fakeConfig.fakeUint32Config),
        .maxVal    = &FAKE_UINT_UPPER_BOUND,
        .minVal    = &FAKE_UINT_LOWER_BOUND
    }
};

const module_t fakeModule = {
    .id           = FAKE_MODULE_ID,
    .context      = &fakeConfig,
    .params       = fake_param_descriptor,
    .params_count = 9
};

const module_t *modules[] = 
{
    &fakeModule
};

void FakeConfig_Init()
{
    fakeConfig.fakeBooleanConfig = false;
    fakeConfig.fakeUint8Config   = 0;
    fakeConfig.fakeUint16Config  = 0;
    fakeConfig.fakeUint32Config  = 0;
    fakeConfig.fakeInt8Config    = 0;
    fakeConfig.fakeInt16Config   = 0;
    fakeConfig.fakeInt32Config   = 0;
    fakeConfig.fakeFloatConfig   = 0.0f;
}


//________________STUBS__________________________
FakeConfigStruct_t stubConfig;

static const module_param_t descriptorWithNoParamId = {
    .id        = 0,
    .flags     = PARAM_FLAG_READ,
    .address   = &stubConfig.fakeBooleanConfig,
    .type      = PARAM_TYPE_BOOL,
    .paramSize = sizeof(stubConfig.fakeBooleanConfig),
};

const module_t moduleWithNoParamId = {
    .id           = 1,
    .context      = &stubConfig,
    .params       = &descriptorWithNoParamId,
    .params_count = 1
};

static const module_param_t descriptorWithNoParamType = {
    .id        = 1,
    .flags     = PARAM_FLAG_READ,
    .address   = &stubConfig.fakeBooleanConfig,
    .paramSize = sizeof(stubConfig.fakeBooleanConfig),
};

const module_t moduleWithNoParamType = {
    .id           = 1,
    .context      = &stubConfig,
    .params       = &descriptorWithNoParamType,
    .params_count = 1   
};

static const module_param_t descriptorWithNotSupportedType = {
    .id        = 1,
    .flags     = PARAM_FLAG_READ,
    .type      = PARAM_NUM_HANDLED,
    .address   = &stubConfig.fakeBooleanConfig,
    .paramSize = sizeof(stubConfig.fakeBooleanConfig),
};

const module_t moduleWithNotSupportedType = {
    .id           = 1,
    .context      = &stubConfig,
    .params       = &descriptorWithNotSupportedType,
    .params_count = 1   
};

static const module_param_t descriptorWithNoFlags = {
    .id        = 1,
    .type      = PARAM_TYPE_BOOL,
    .address   = &stubConfig.fakeBooleanConfig,
    .paramSize = sizeof(stubConfig.fakeBooleanConfig),
};

const module_t moduleWithNoFlags = {
    .id           = 1,
    .context      = &stubConfig,
    .params       = &descriptorWithNoFlags,
    .params_count = 1   
};

static const module_param_t descriptorWithNoSize = {
    .id        = 1,
    .type      = PARAM_TYPE_BOOL,
    .flags     = PARAM_FLAG_READ,
    .address   = &stubConfig.fakeBooleanConfig,
};

const module_t moduleWithNoSize = {
    .id           = 1,
    .context      = &stubConfig,
    .params       = &descriptorWithNoSize,
    .params_count = 1   
};

static const module_param_t descriptorWithNoAddress = {
    .id        = 1,
    .type      = PARAM_TYPE_BOOL,
    .flags     = PARAM_FLAG_READ,
    .address   = NULL,
    .paramSize = 1
};

const module_t moduleWithNoParamAddress = {
    .id           = 1,
    .context      = &stubConfig,
    .params       = &descriptorWithNoAddress,
    .params_count = 1   
};

static const module_param_t descriptorStub = {
    .id        = 1,
    .type      = PARAM_TYPE_BOOL,
    .flags     = PARAM_FLAG_READ,
    .address   = &stubConfig.fakeBooleanConfig,
    .paramSize = 1
};

const module_t moduleWithNoId = {
    .context      = &stubConfig,
    .id           = 0,
    .params       = &descriptorStub,
    .params_count = 1   
};

const module_t moduleWithNoContext = {
    .id      = 1,
    .context = NULL,
    .params  = &descriptorStub,
    .params_count = 1
};

const module_t moduleWithNoParameterPtr = {
    .id      = 1,
    .context = &stubConfig,
    .params  = NULL,
    .params_count = 1
};

const module_t moduleWithNoParameterCount = {
    .id      = 1,
    .context = &stubConfig,
    .params  = &descriptorStub,
    .params_count = 0
};
