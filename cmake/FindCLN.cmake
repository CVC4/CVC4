# Find CLN
# CLN_FOUND - system has CLN lib
# CLN_INCLUDE_DIR - the CLN include directory
# CLN_LIBRARIES - Libraries needed to use CLN

find_path(CLN_INCLUDE_DIR NAMES cln/cln.h)
find_library(CLN_LIBRARIES NAMES cln)


if(CLN_INCLUDE_DIR)
  file(STRINGS
        "${CLN_INCLUDE_DIR}/cln/version.h" version_info
        REGEX "^#define[ \t]+CL_VERSION_.*")
  string(REGEX REPLACE
         "^.*_MAJOR[ \t]+([0-9]+).*" "\\1" version_major "${version_info}")
  string(REGEX REPLACE
         "^.*_MINOR[ \t]+([0-9]+).*" "\\1" version_minor "${version_info}")
  string(REGEX REPLACE
         "^.*_PATCHLEVEL[ \t]+([0-9]+).*" "\\1" version_patch "${version_info}")
  set(CLN_VERSION ${version_major}.${version_minor}.${version_patch})

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(CLN
    REQUIRED_VARS CLN_INCLUDE_DIR CLN_LIBRARIES
    VERSION_VAR CLN_VERSION)
  mark_as_advanced(CLN_INCLUDE_DIR CLN_LIBRARIES)
endif()
