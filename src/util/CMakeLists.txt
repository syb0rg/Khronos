cmake_minimum_required(VERSION 3.0.2)
project(Khronos_Util)

set(UTIL_SOURCE_FILES
	util.c
)

add_library(util OBJECT ${UTIL_SOURCE_FILES})

if (DEPENDENCIES)
    ADD_DEPENDENCIES(util ${DEPENDENCIES})
endif ()