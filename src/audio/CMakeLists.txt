cmake_minimum_required(VERSION 3.0.2)
project(Khronos_Audio)

set(AUDIO_SOURCE_FILES
	record.c
)

add_library(audio OBJECT ${AUDIO_SOURCE_FILES})

if (DEPENDENCIES)
    ADD_DEPENDENCIES(audio ${DEPENDENCIES})
endif ()