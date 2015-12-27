cmake_minimum_required(VERSION 3.0.2)
project(Khronos_Source)

add_subdirectory(audio)
add_subdirectory(util)

set(MAIN_SOURCE_FILES
	command.c
	main.c
)

add_library(main OBJECT ${MAIN_SOURCE_FILES})