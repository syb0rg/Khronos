set(AUDIO_SOURCE_FILES
	record.c
)

add_library(audio OBJECT ${AUDIO_SOURCE_FILES})
target_compile_features(audio PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(audio ${DEPENDENCIES})
endif ()