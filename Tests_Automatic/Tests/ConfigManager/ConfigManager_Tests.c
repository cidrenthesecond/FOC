#include "unity_fixture.h"
#include "ConfigManager.h"
#include "FakeConfig.h"
#include "stdbool.h"

//TO DO: finish bounds tests

TEST_GROUP(ConfigManager);

TEST_SETUP(ConfigManager)
{
    ConfigManager_Setup(modules,1);
    FakeConfig_Init();
}

TEST_TEAR_DOWN(ConfigManager)
{
    ConfigManager_Destroy();
}

TEST(ConfigManager,Getting_WrongModuleId_ThrowsError)
{
    bool returnVal;
    TEST_ASSERT_EQUAL(CONFIG_ERR_MODULE_NOT_FOUND,Config_get(0,FAKE_BOOL_ID,&returnVal));
}

TEST(ConfigManager,Getting_WrongParamId_ThrowsError)
{
    bool returnVal;
    TEST_ASSERT_EQUAL(CONFIG_ERR_PARAM_NOT_FOUND,Config_get( FAKE_MODULE_ID,0,&returnVal));
}

TEST(ConfigManager,Getting_CorrectModuleAndParamId_ReturnsSuccess)
{
    bool returnVal;
    TEST_ASSERT_EQUAL(CONFIG_OK,Config_get(FAKE_MODULE_ID,FAKE_BOOL_ID,&returnVal));
}

TEST(ConfigManager,Getting_CorrectModuleAndParamId_ReturnsValue)
{
    fakeConfig.fakeBooleanConfig = false;
    bool returnVal;

    Config_get(FAKE_MODULE_ID,FAKE_BOOL_ID,&returnVal);
    TEST_ASSERT_EQUAL(returnVal, fakeConfig.fakeBooleanConfig);

    fakeConfig.fakeBooleanConfig = true;

    Config_get(FAKE_MODULE_ID,FAKE_BOOL_ID,&returnVal);
    TEST_ASSERT_EQUAL(returnVal, fakeConfig.fakeBooleanConfig);
}

TEST(ConfigManager,Getting_WriteOnly_ThrowsError)
{
    bool returnVal;

    TEST_ASSERT_EQUAL(CONFIG_ERR_READ_NOT_ALLOWED,Config_get(FAKE_MODULE_ID,FAKE_WRITEONLY_BOOL_ID,&returnVal));
}

TEST(ConfigManager,Setting_WrongModuleId_ThrowsError)
{
    bool setVal;

    TEST_ASSERT_EQUAL(CONFIG_ERR_MODULE_NOT_FOUND,Config_set(0,FAKE_BOOL_ID,&setVal));
}

TEST(ConfigManager,Setting_WrongParameterId_ThrowsError)
{
    bool setVal;

    TEST_ASSERT_EQUAL(CONFIG_ERR_PARAM_NOT_FOUND,Config_set(FAKE_MODULE_ID,0,&setVal));
}

TEST(ConfigManager,Setting_ReadonlyParameter_ThrowsError)
{
    bool setVal;

    TEST_ASSERT_EQUAL(CONFIG_ERR_WRITE_NOT_ALLOWED,Config_set(FAKE_MODULE_ID,FAKE_READONLY_BOOL_ID,&setVal));
}

TEST(ConfigManager, Setting_CorrectModuleIdAndParamId_SetsValue)
{
    bool setVal = false;

    Config_set(FAKE_MODULE_ID,FAKE_BOOL_ID,&setVal);
    TEST_ASSERT_EQUAL(setVal, fakeConfig.fakeBooleanConfig);

    setVal = true;

    Config_set(FAKE_MODULE_ID,FAKE_BOOL_ID,&setVal);
    TEST_ASSERT_EQUAL(setVal,fakeConfig.fakeBooleanConfig);
}

TEST(ConfigManager, Setting_BooleanHigherThanTrue_ThrowsError)
{
    uint32_t setVal = 125;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INVALID_VALUE, Config_set(FAKE_MODULE_ID,FAKE_BOOL_ID,&setVal));
}

TEST(ConfigManager, Setting_uint8OutOfBounds_ThrowsError)
{
    uint8_t  startValue = fakeConfig.fakeUint8Config;

    uint32_t setVal     = FAKE_UINT_UPPER_BOUND + 1;
    TEST_ASSERT_EQUAL      (CONFIG_ERR_INVALID_VALUE,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT8_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT8(startValue,fakeConfig.fakeUint8Config);

    setVal = FAKE_UINT_LOWER_BOUND - 1;
    TEST_ASSERT_EQUAL(CONFIG_ERR_INVALID_VALUE,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT8_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT8(startValue,fakeConfig.fakeUint8Config);
}

TEST(ConfigManager, Setting_uint8InBounds_ReturnsSuccessAndChangesValue)
{
    uint32_t setVal = FAKE_UINT_UPPER_BOUND;
    TEST_ASSERT_EQUAL      (CONFIG_OK,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT8_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT8(FAKE_UINT_UPPER_BOUND,fakeConfig.fakeUint8Config);

    setVal = FAKE_UINT_LOWER_BOUND;
    TEST_ASSERT_EQUAL      (CONFIG_OK,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT8_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT8(FAKE_UINT_LOWER_BOUND,fakeConfig.fakeUint8Config);
}

TEST(ConfigManager, Setting_uint16OutOfBounds_ThrowsError)
{
    uint8_t  startValue = fakeConfig.fakeUint16Config;

    uint32_t setVal     = FAKE_UINT_UPPER_BOUND + 1;
    TEST_ASSERT_EQUAL      (CONFIG_ERR_INVALID_VALUE,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT16_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT16(startValue,fakeConfig.fakeUint16Config);

    setVal = FAKE_UINT_LOWER_BOUND - 1;
    TEST_ASSERT_EQUAL(CONFIG_ERR_INVALID_VALUE,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT16_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT16(startValue,fakeConfig.fakeUint16Config);
}

TEST(ConfigManager, Setting_uint16InBounds_ReturnsSuccessAndChangesValue)
{
    uint32_t setVal = FAKE_UINT_UPPER_BOUND;
    TEST_ASSERT_EQUAL (CONFIG_OK,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT16_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT16(setVal,fakeConfig.fakeUint16Config);

    setVal = FAKE_UINT_LOWER_BOUND;
    TEST_ASSERT_EQUAL      (CONFIG_OK,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT16_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT16(setVal,fakeConfig.fakeUint16Config);
}

TEST(ConfigManager, Setting_uint32OutOfBounds_ThrowsError)
{
    uint8_t  startValue = fakeConfig.fakeUint32Config;

    uint32_t setVal     = FAKE_UINT_UPPER_BOUND + 1;
    TEST_ASSERT_EQUAL       (CONFIG_ERR_INVALID_VALUE,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT32_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT32(startValue,fakeConfig.fakeUint32Config);

    setVal = FAKE_UINT_LOWER_BOUND - 1;
    TEST_ASSERT_EQUAL(CONFIG_ERR_INVALID_VALUE,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT32_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT32(startValue,fakeConfig.fakeUint32Config);
}

TEST(ConfigManager, Setting_uint32InBounds_ReturnsSuccessAndChangesValue)
{
    uint32_t setVal = FAKE_UINT_UPPER_BOUND;
    TEST_ASSERT_EQUAL (CONFIG_OK,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT32_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT32(setVal,fakeConfig.fakeUint32Config);

    setVal = FAKE_UINT_LOWER_BOUND;
    TEST_ASSERT_EQUAL      (CONFIG_OK,Config_set(FAKE_MODULE_ID,FAKE_BOUNDED_UINT32_ID,&setVal));
    TEST_ASSERT_EQUAL_UINT8(setVal,fakeConfig.fakeUint32Config);
}


//SPACE FOR BOUND CHECKING



TEST(ConfigManager, Init_WithNoParamID_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoParamId;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_PARAM_ID_NOT_SET,ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithNoParamType_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoParamType;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_PARAM_TYPE_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithParamTypeNotSupported_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNotSupportedType;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_PARAM_TYPE_NOT_SUPPORTED, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithNoParamFlags_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoFlags;
    
    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_PARAM_NO_FLAGS, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithNoParamSize_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoSize;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_PARAM_SIZE_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithNoParamAddress_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoParamAddress;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_PARAM_ADDR_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithNoModuleId_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoId;

    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_MODULE_ID_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_WithNoModuleContext_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoContext;
    
    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_MODULE_CONTEXT_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_ModuleWithNoDescriptorPointer_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoParameterPtr;
    
    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_MODULE_PARAM_PTR_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}

TEST(ConfigManager, Init_ModuleWithNoParamCount_ThrowsError)
{
    ConfigManager_Destroy();
    const module_t *module_ptr = &moduleWithNoParameterCount;
    
    TEST_ASSERT_EQUAL(CONFIG_ERR_INIT_MODULE_PARAM_COUNT_NOT_SET, ConfigManager_Setup(&module_ptr,1));
}
