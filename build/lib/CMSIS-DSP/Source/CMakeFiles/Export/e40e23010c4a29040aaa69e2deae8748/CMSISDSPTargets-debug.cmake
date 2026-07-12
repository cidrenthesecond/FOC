#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CMSISDSP::CMSISDSP" for configuration "Debug"
set_property(TARGET CMSISDSP::CMSISDSP APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(CMSISDSP::CMSISDSP PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libCMSISDSP.a"
  )

list(APPEND _cmake_import_check_targets CMSISDSP::CMSISDSP )
list(APPEND _cmake_import_check_files_for_CMSISDSP::CMSISDSP "${_IMPORT_PREFIX}/lib/libCMSISDSP.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
