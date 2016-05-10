# - Try to find tritium
# Once done, this will define
#
#  TRITIUM_FOUND - system has tritium
#  TRITIUM_INCLUDE_DIRS - the tritium include directories
#  TRITIUM_LIBRARIES - link these to use tritium

# Include dir
find_path(TRITIUM_INCLUDE_DIR tritium/tritium.h)

# Library
find_library(TRITIUM_LIBRARY tritium)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tritium DEFAULT_MSG TRITIUM_LIBRARY TRITIUM_INCLUDE_DIR)

if(TRITIUM_FOUND)
    set(TRITIUM_LIBRARIES ${TRITIUM_LIBRARY})
    set(TRITIUM_INCLUDE_DIRS ${TRITIUM_INCLUDE_DIR})
else(TRITIUM_FOUND)
    set(TRITIUM_LIBRARIES)
endif(TRITIUM_FOUND)

mark_as_advanced(TRITIUM_LIBRARY TRITIUM_LIBRARIES TRITIUM_INCLUDE_DIR TRITIUM_INCLUDE_DIRS)