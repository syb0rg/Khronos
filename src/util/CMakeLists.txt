set(UTIL_SOURCE_FILES
	util.c
)

add_library(util OBJECT ${UTIL_SOURCE_FILES})
target_compile_features(util PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(util ${DEPENDENCIES})
endif ()