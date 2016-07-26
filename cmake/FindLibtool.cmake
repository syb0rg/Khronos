#
# Check for the presence of LIBTOOL
#
# LIBTOOL_FOUND will be true if all components have been found.
include(FindPackageHandleStandardArgs)

IF (NOT LIBTOOL_FOUND)
    IF (NOT LIBTOOL_ROOT_DIR)
        SET(LIBTOOL_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
    ENDIF ()

    # Determine if the libtool executables exist
    FOREACH (APPLICATION libtool libtoolize)
        # Find the executables
        STRING(TOUPPER ${APPLICATION} APPLICATION_NAME)
        FIND_PROGRAM(${APPLICATION_NAME}_EXECUTABLE ${APPLICATION} HINTS ${LIBTOOL_ROOT_DIR} ${CMAKE_INSTALL_PREFIX} PATH_SUFFIXES bin)
    ENDFOREACH()

    # Finalize results
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBTOOL DEFAULT_MSG LIBTOOL_EXECUTABLE)
    IF (NOT LIBTOOL_FOUND)
        IF (LIBTOOL_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "Could not find LIBTOOL!")
        ENDIF ()
    ENDIF ()
    MARK_AS_ADVANCED(LIBTOOL_ROOT_DIR LIBTOOL_EXECUTABLE LIBTOOLIZE_EXECUTABLE)
endif ()