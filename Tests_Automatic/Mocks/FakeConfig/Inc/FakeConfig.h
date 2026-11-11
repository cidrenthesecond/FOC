#ifndef FAKE_CONFIG_H
#define FAKE_CONFIG_H

#include "stdbool.h"
#include "stdint.h"
#include "ConfigInterface.h"

#define FAKE_MODULE_ID           (1u)
#define FAKE_MODULE_PARAM_COUNT  (17u)

#define FAKE_BOOL_ID             (1u)
#define FAKE_READONLY_BOOL_ID    (2u)
#define FAKE_WRITEONLY_BOOL_ID   (3u)
#define FAKE_UNBOUNDED_UINT8_ID  (4u)
#define FAKE_BOUNDED_UINT8_ID    (5u)
#define FAKE_UNBOUNDED_UINT16_ID (6u)
#define FAKE_BOUNDED_UINT16_ID   (7u)
#define FAKE_UNBOUNDED_UINT32_ID (8u)
#define FAKE_BOUNDED_UINT32_ID   (9u)
#define FAKE_UNBOUNDED_INT8_ID   (10u)
#define FAKE_BOUNDED_INT8_ID     (11u)
#define FAKE_UNBOUNDED_INT16_ID  (12u)
#define FAKE_BOUNDED_INT16_ID    (13u)
#define FAKE_UNBOUNDED_INT32_ID  (14u)
#define FAKE_BOUNDED_INT32_ID    (15u)
#define FAKE_UNBOUNDED_FLOAT_ID  (16u)
#define FAKE_BOUNDED_FLOAT_ID    (17u)

extern const uint32_t FAKE_UINT_UPPER_BOUND;
extern const uint32_t FAKE_UINT_LOWER_BOUND;
extern const int32_t  FAKE_INT_UPPER_BOUND; 
extern const int32_t  FAKE_INT_LOWER_BOUND; 
extern const float    FAKE_FLOAT_UPPER_BOUND;
extern const float    FAKE_FLOAT_LOWER_BOUND;

typedef struct {
    bool     fakeBooleanConfig;
    uint8_t  fakeUint8Config;
    uint16_t fakeUint16Config;
    uint32_t fakeUint32Config;
    int8_t   fakeInt8Config;
    int16_t  fakeInt16Config;
    int32_t  fakeInt32Config;
    float    fakeFloatConfig;
}FakeConfigStruct_t;

extern FakeConfigStruct_t fakeConfig;
extern const module_t *modules[];

//stubs
extern const module_t moduleWithNoParamId;
extern const module_t moduleWithNoParamType;
extern const module_t moduleWithNotSupportedType;
extern const module_t moduleWithNoFlags;
extern const module_t moduleWithNoSize;
extern const module_t moduleWithNoParamAddress;
extern const module_t moduleWithNoId;
extern const module_t moduleWithNoContext;
extern const module_t moduleWithNoParameterPtr;
extern const module_t moduleWithNoParameterCount;

void FakeConfig_Init();

#endif