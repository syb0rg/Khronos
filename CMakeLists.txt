#
# Initial CMake and project setup
#

# Required CMake version (ExternalProject)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
INCLUDE(ExternalProject)
SET(PROJECT_NAME Khronos)
PROJECT(${PROJECT_NAME})

# Set software version
SET(APPLICATION_VERSION_MAJOR "0")
SET(APPLICATION_VERSION_MINOR "4")
SET(APPLICATION_VERSION_PATCH "0")
SET(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/library-build)

#
# Setup compiler flags
#

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE Release FORCE)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu11 -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu11 -O3")

if (GCC_VERSION VERSION_GREATER "4.8")
elsif (GCC_VERSION VERSION_GREATER "4.1.2")
	SET (GCC_COMMON_WARNING_FLAGS "-pedantic -Wall -Wextra -Wconversion -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstack-protector -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum")
	SET (GCC_COMMON_WARNING_FLAGS "${GCC_COMMON_WARNING_FLAGS} -Wno-unused-parameter")
	SET (GCC_CXX_WARNING_FLAGS "-Wctor-dtor-privacy")
else ()
	SET(GCC_COMMON_WARNING_FLAGS "-pedantic -Wall -Wextra -Wconversion -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstack-protector -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum")
	SET(GCC_COMMON_WARNING_FLAGS "${GCC_COMMON_WARNING_FLAGS} -Wno-unused-parameter")
	SET(GCC_CXX_WARNING_FLAGS "-Wctor-dtor-privacy")
endif ()

#
# Find Doxygen for documentation
#

# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
	ADD_CUSTOM_TARGET(DOC
		${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
endif ()

#
# Find/install dependencies
#

IF(WIN32)
    MESSAGE(FATAL_ERROR "Windows support in development")
    INCLUDE(${CMAKE_MODULE_PATH}/Win32-ExternalProjects.txt)
ELSE(WIN32)
    IF(APPLE)
        INCLUDE(${CMAKE_MODULE_PATH}/Apple-ExternalProjects.txt)
    ELSEIF(UNIX)
        MESSAGE(FATAL_ERROR "Unix support in development")
        INCLUDE(${CMAKE_MODULE_PATH}/Unix-ExternalProjects.txt)
    ENDIF(APPLE)
ENDIF(WIN32)

#
# Final project settings
#

INCLUDE_DIRECTORIES(include)
ADD_SUBDIRECTORY(src)

ADD_EXECUTABLE(${PROJECT_NAME}
        $<TARGET_OBJECTS:audio>
        $<TARGET_OBJECTS:util>
        $<TARGET_OBJECTS:main>
        )
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
if (DEPENDENCIES)
    MESSAGE (STATUS "Any missing dependencies will be downloaded upon running \"make\"")
    ADD_DEPENDENCIES(${PROJECT_NAME} ${DEPENDENCIES})
endif ()
