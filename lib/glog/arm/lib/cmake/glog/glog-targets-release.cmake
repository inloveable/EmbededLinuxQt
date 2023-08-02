#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glog::glog" for configuration "Release"
set_property(TARGET glog::glog APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(glog::glog PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libglog.so.0.7.0"
  IMPORTED_SONAME_RELEASE "libglog.so.1"
  )

list(APPEND _cmake_import_check_targets glog::glog )
list(APPEND _cmake_import_check_files_for_glog::glog "${_IMPORT_PREFIX}/lib/libglog.so.0.7.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
