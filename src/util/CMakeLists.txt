set(UTIL_SOURCE_FILES
	util.c
)

add_library(util OBJECT ${UTIL_SOURCE_FILES})

if (DEPENDENCIES)
    add_dependencies(util ${DEPENDENCIES})
endif ()