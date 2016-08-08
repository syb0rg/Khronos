set(AUDIO_SOURCE_FILES
	record.c
)

add_library(audio OBJECT ${AUDIO_SOURCE_FILES})

if (DEPENDENCIES)
    add_dependencies(audio ${DEPENDENCIES})
endif ()