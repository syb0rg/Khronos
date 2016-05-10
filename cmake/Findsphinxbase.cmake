# - Try to find sphinxbase
# Once done, this will define
#
#  SPHINXBASE_FOUND - system has sphinxbase
#  SPHINXBASE_INCLUDE_DIRS - the sphinxbase include directories
#  SPHINXBASE_LIBRARIES - link these to use sphinxbase

# Include dir
find_path(SPHINXBASE_INCLUDE_DIR sphinxbase/cmd_ln.h)

# Library
find_library(SPHINXBASE_LIBRARY sphinxbase)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sphinxbase DEFAULT_MSG SPHINXBASE_LIBRARY SPHINXBASE_INCLUDE_DIR)

if(SPHINXBASE_FOUND)
    set(SPHINXBASE_LIBRARIES ${SPHINXBASE_LIBRARY})
    set(SPHINXBASE_INCLUDE_DIRS ${SPHINXBASE_INCLUDE_DIR})
else(SPHINXBASE_FOUND)
    set(SPHINXBASE_LIBRARIES)
endif(SPHINXBASE_FOUND)

mark_as_advanced(SPHINXBASE_LIBRARY SPHINXBASE_LIBRARIES SPHINXBASE_INCLUDE_DIR SPHINXBASE_INCLUDE_DIRS)