add_subdirectory(audio)
add_subdirectory(util)

set(MAIN_SOURCE_FILES
	command.c
	main.c
)

add_library(main OBJECT ${MAIN_SOURCE_FILES})
target_compile_features(main PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(main ${DEPENDENCIES})
endif ()