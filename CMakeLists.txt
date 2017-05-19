#
# Initial CMake and project setup
#

# Required CMake version (ExternalProject)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(PROJECT_NAME Khronos)
project(${PROJECT_NAME})

include(ExternalProject)

# Set software version
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "5")
set(APPLICATION_VERSION_PATCH "0")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/build-data)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#
# Options
#

option(BUILD_TESTS "Build tests" OFF)

#
# Setup compiler flags
#

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE CACHE Release FORCE)
endif()


set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 --coverage -pedantic -Wall -Wextra -Wconversion -Wfloat-equal -Wformat=2 -Winit-self -Winline -Winvalid-pch -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstack-protector -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wno-unused-parameter -Wctor-dtor-privacy"
)

#
# Find Doxygen for documentation
#

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_custom_target(DOC
		${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
		COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif ()

#
# Find/install dependencies
#

if (WIN32)
    include(${CMAKE_MODULE_PATH}/Win32-ExternalProjects.txt)
else (UNIX)
    include(${CMAKE_MODULE_PATH}/Unix-ExternalProjects.txt)
endif ()

#
# Final project settings
#

include_directories(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME}
    $<TARGET_OBJECTS:audio>
    $<TARGET_OBJECTS:util>
    $<TARGET_OBJECTS:main>
)

target_compile_features(${PROJECT_NAME} PRIVATE c_restrict)
target_link_libraries(${PROJECT_NAME} ${LIBS})

if (DEPENDENCIES)
    message(STATUS "Any missing dependencies will be downloaded upon running \"make\"")
    add_dependencies(${PROJECT_NAME} ${DEPENDENCIES})
endif ()

#
# Testing
#

if(BUILD_TESTS)
    include(CTest)
    enable_testing()

    set(util_test_SOURCES test/test_util)
    add_executable(util_test ${util_test_SOURCES}
        $<TARGET_OBJECTS:util>
    )
    target_compile_features(util_test PRIVATE c_restrict)
    add_test (util_test util_test)

    set(audio_test_SOURCES test/test_audio)
    add_executable(audio_test ${audio_test_SOURCES}
        $<TARGET_OBJECTS:audio>
    )
    target_compile_features(audio_test PRIVATE c_restrict)
    target_link_libraries(audio_test ${LIBS})
    add_test (audio_test audio_test)
endif(BUILD_TESTS)
