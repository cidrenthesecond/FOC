#include "unity_fixture.h"

TEST_GROUP_RUNNER(ConfigManager)
{
    RUN_TEST_CASE(ConfigManager, Getting_WrongModuleId_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Getting_WrongParamId_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Getting_CorrectModuleAndParamId_ReturnsSuccess);
    RUN_TEST_CASE(ConfigManager, Getting_CorrectModuleAndParamId_ReturnsValue);
    RUN_TEST_CASE(ConfigManager, Getting_WriteOnly_ThrowsError);

    RUN_TEST_CASE(ConfigManager, Setting_WrongModuleId_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_WrongParameterId_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_ReadonlyParameter_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_CorrectModuleIdAndParamId_SetsValue);

    RUN_TEST_CASE(ConfigManager, Setting_BooleanHigherThanTrue_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_uint8OutOfBounds_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_uint8InBounds_ReturnsSuccessAndChangesValue);
    RUN_TEST_CASE(ConfigManager, Setting_uint16OutOfBounds_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_uint16InBounds_ReturnsSuccessAndChangesValue);
    RUN_TEST_CASE(ConfigManager, Setting_uint32OutOfBounds_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Setting_uint32InBounds_ReturnsSuccessAndChangesValue);

    RUN_TEST_CASE(ConfigManager, Init_WithNoParamID_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithNoParamType_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithParamTypeNotSupported_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithNoParamFlags_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithNoParamSize_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithNoParamAddress_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithNoModuleId_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_WithNoModuleContext_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_ModuleWithNoDescriptorPointer_ThrowsError);
    RUN_TEST_CASE(ConfigManager, Init_ModuleWithNoParamCount_ThrowsError);
}