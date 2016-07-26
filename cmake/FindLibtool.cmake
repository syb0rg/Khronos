# - Try to find Libtool
# Once done, this will define
#
#  LIBTOOL_FOUND - system has sphinxbase
#  LIBTOOL_INCLUDE_DIRS - the sphinxbase include directories
#  LIBTOOL_LIBRARIES - link these to use sphinxbase

# Include dir
find_path(LIBTOOL_INCLUDE_DIR Libtool/config.h)

# Library
find_library(LIBTOOL_LIBRARY Libtool)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libtool DEFAULT_MSG LIBTOOL_LIBRARY LIBTOOL_INCLUDE_DIR)

if (LIBTOOL_FOUND)
    set(LIBTOOL_LIBRARIES ${LIBTOOL_LIBRARY})
    set(LIBTOOL_INCLUDE_DIRS ${LIBTOOL_INCLUDE_DIR})
else ()
    set(LIBTOOL_LIBRARIES)
endif ()

mark_as_advanced(LIBTOOL_LIBRARY LIBTOOL_LIBRARIES LIBTOOL_INCLUDE_DIR LIBTOOL_INCLUDE_DIRS)