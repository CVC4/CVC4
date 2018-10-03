# Find ANTLR
# ANTLR_FOUND - system has ANTLR lib
# ANTLR_BINARY - the ANTLR binary
# ANTLR_INCLUDE_DIR - the ANTLR include directory
# ANTLR_LIBRARIES - Libraries needed to use ANTLR


# Check default location of ANTLR built with contrib/get-antlr-3.4.
# If the user provides a directory we will not search the default paths and
# fail if ANTLR was not found in the specified directory.
if(NOT ANTLR_HOME)
  set(ANTLR_HOME ${PROJECT_SOURCE_DIR}/antlr-3.4)
  set(CHECK_SYSTEM_VERSION TRUE)
endif()

find_program(ANTLR_BINARY
  NAMES antlr3
  PATHS ${ANTLR_HOME}/bin
  NO_DEFAULT_PATH)
find_path(ANTLR_INCLUDE_DIR
  NAMES antlr3.h
  PATHS ${ANTLR_HOME}/include
  NO_DEFAULT_PATH)
find_library(ANTLR_LIBRARIES
  NAMES antlr3c antlr3c-static
  PATHS ${ANTLR_HOME}/lib
  NO_DEFAULT_PATH)

if(CHECK_SYSTEM_VERSION)
  find_program(ANTLR_BINARY NAMES antlr3)
  find_path(ANTLR_INCLUDE_DIR NAMES antlr3.h)
  find_library(ANTLR_LIBRARIES NAMES antlr3c)
endif()

# Check if antlr3FileStreamNew is available. If not we have to
# define CVC4_ANTLR3_OLD_INPUT_STREAM (src/parser/CMakeLists.txt).
if(ANTLR_INCLUDE_DIR)
  include(CheckSymbolExists)
  set(CMAKE_REQUIRED_INCLUDES ${ANTLR_INCLUDE_DIR})
  set(CMAKE_REQUIRED_LIBRARIES ${ANTLR_LIBRARIES})
  check_symbol_exists(
    antlr3FileStreamNew "antlr3defs.h" HAVE_ANTLR3_FILE_STREAM_NEW)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  ANTLR DEFAULT_MSG ANTLR_BINARY ANTLR_INCLUDE_DIR ANTLR_LIBRARIES)

mark_as_advanced(ANTLR_BINARY ANTLR_INCLUDE_DIR ANTLR_LIBRARIES
                 HAVE_ANTLR3_FILE_STREAM_NEW)
if(ANTLR_LIBRARIES)
  message(STATUS "Found ANTLR libs: ${ANTLR_LIBRARIES}")
endif()
