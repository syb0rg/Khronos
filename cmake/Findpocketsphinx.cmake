# - Try to find pocketsphinx
# Once done, this will define
#
#  POCKETSPHINX_FOUND - system has pocketsphinx
#  POCKETSPHINX_INCLUDE_DIRS - the pocketsphinx include directories
#  POCKETSPHINX_LIBRARIES - link these to use pocketsphinx

# Include dir
find_path(POCKETSPHINX_INCLUDE_DIR pocketsphinx/pocketsphinx.h)

# Library
find_library(POCKETSPHINX_LIBRARY libpocketsphinx.a)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pocketsphinx DEFAULT_MSG POCKETSPHINX_LIBRARY POCKETSPHINX_INCLUDE_DIR)

if(POCKETSPHINX_FOUND)
    set(POCKETSPHINX_LIBRARIES ${POCKETSPHINX_LIBRARY})
    set(POCKETSPHINX_INCLUDE_DIRS ${POCKETSPHINX_INCLUDE_DIR})
else(POCKETSPHINX_FOUND)
    set(POCKETSPHINX_LIBRARIES)
endif(POCKETSPHINX_FOUND)

mark_as_advanced(POCKETSPHINX_LIBRARY POCKETSPHINX_LIBRARIES POCKETSPHINX_INCLUDE_DIR POCKETSPHINX_INCLUDE_DIRS)