cmake_minimum_required(VERSION 2.8.7)
include(ExternalProject)
project(Khronos)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/library-build)
set(CMAKE_C_FLAGS "-std=gnu11 -O3")

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

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
endif(DOXYGEN_FOUND)

find_package(PortAudio)
if(${PORTAUDIO_FOUND})
else(${PORTAUDIO_FOUND})
	message(STATUS "Could not find PortAudio.  This dependency will be downloaded.")
	ExternalProject_Add(
		PortAudio
		SVN_REPOSITORY "https://subversion.assembla.com/svn/portaudio/portaudio/trunk/"
		SVN_TRUST_CERT 1
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio
		CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/configure --prefix=<INSTALL_DIR>
		BUILD_COMMAND ${MAKE}
		UPDATE_COMMAND ""
		INSTALL_COMMAND ""
		LOG_DOWNLOAD ON
		LOG_UPDATE ON
		LOG_CONFIGURE ON
		LOG_BUILD ON
		LOG_TEST ON
		LOG_INSTALL ON
	)
	ExternalProject_Get_Property(PortAudio source_dir)
	ExternalProject_Get_Property(PortAudio binary_dir)
	set(PORTAUDIO_SOURCE_DIR ${source_dir})
	set(PORTAUDIO_BINARY_DIR ${binary_dir})
	set(PORTAUDIO_LIBRARIES ${PORTAUDIO_BINARY_DIR}/lib/.libs/libportaudio.dylib)
	include_directories(${PORTAUDIO_SOURCE_DIR})
	set(DEPENDENCIES ${DEPENDENCIES} PortAudio)
endif(${PORTAUDIO_FOUND})

message(STATUS "Could not find parcel.  This dependency will be downloaded.")
ExternalProject_Add(
	parcel
	GIT_REPOSITORY "git://github.com/syb0rg/parcel.git"
	GIT_TAG "c2fd447cd2af552021304e64b6bd66c88c170241"
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/parcel
    CMAKE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/parcel
	UPDATE_COMMAND ""
	INSTALL_COMMAND ""
	LOG_DOWNLOAD ON
	LOG_UPDATE ON
	LOG_CONFIGURE ON
	LOG_BUILD ON
	LOG_TEST ON
	LOG_INSTALL ON
)
ExternalProject_Get_Property(parcel source_dir)
ExternalProject_Get_Property(parcel binary_dir)
set(PARCEL_SOURCE_DIR ${source_dir})
set(PARCEL_BINARY_DIR ${binary_dir})
set(PARCEL_LIBRARIES ${PARCEL_BINARY_DIR}/libparcel.a)
set(DEPENDENCIES ${DEPENDENCIES} parcel)

find_package(CURL)
if(${CURL_FOUND})
else(${CURL_FOUND})
	message(STATUS "Could not find libcURL.  This dependency will be downloaded.")
	ExternalProject_Add(
		libcurl
		GIT_REPOSITORY "git://github.com/bagder/curl.git"
		GIT_TAG "1b6bc02fb926403f04061721f9159e9887202a96"
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl
        PATCH_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl/buildconf
        CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl/configure --prefix=<INSTALL_DIR>
		BUILD_COMMAND ${MAKE}
		UPDATE_COMMAND ""
		INSTALL_COMMAND ""
		LOG_DOWNLOAD ON
		LOG_UPDATE ON
		LOG_CONFIGURE ON
		LOG_BUILD ON
		LOG_TEST ON
		LOG_INSTALL ON
	)
	ExternalProject_Get_Property(libcurl source_dir)
	ExternalProject_Get_Property(libcurl binary_dir)
	set(CURL_SOURCE_DIR ${source_dir})
	set(CURL_BINARY_DIR ${binary_dir})
	set(CURL_LIBRARIES ${CURL_BINARY_DIR}/lib/.libs/libcurl.dylib)
	include_directories(${CURL_SOURCE_DIR})
	set(DEPENDENCIES ${DEPENDENCIES} libcurl)
endif(${CURL_FOUND})

#find_package(LibOgg)

#find_package(LibVorbis)

find_package(FLAC) # test if FLAC is installed on the system
if(${FLAC_FOUND}) # do something if it is found, maybe tell the user
else(${FLAC_FOUND}) # FLAC isn't installed on the system and needs to be downloaded
    ExternalProject_Add(
    FLAC
    GIT_REPOSITORY "git://github.com/xiph/flac.git"
    GIT_TAG "0e11f73eabd3544f59937d0a0d8e076d7c9c2d1d"
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac
    PATCH_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac/autogen.sh
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac/configure --prefix=<INSTALL_DIR>
    BUILD_COMMAND ${MAKE}
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_TEST ON
    LOG_INSTALL ON
    )
endif(${FLAC_FOUND})

find_package(LibSndFile)
if(${LIBSNDFILE_FOUND})
else(${LIBSNDFILE_FOUND})
	ExternalProject_Add(
		LibSndFile
		DEPENDS FLAC
		URL "http://www.mega-nerd.com/libsndfile/files/libsndfile-1.0.25.tar.gz"
		CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/LibSndFile/configure --prefix=<INSTALL_DIR>
		BUILD_COMMAND ${MAKE}
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/LibSndFile
		UPDATE_COMMAND ""
		INSTALL_COMMAND ""
		LOG_DOWNLOAD ON
		LOG_UPDATE ON
		LOG_CONFIGURE ON
		LOG_BUILD ON
		LOG_TEST ON
		LOG_INSTALL ON
	)
	ExternalProject_Get_Property(LibSndFile source_dir)
	ExternalProject_Get_Property(LibSndFile binary_dir)
	set(LIBSNDFILE_SOURCE_DIR ${source_dir})
	set(LIBSNDFILE_BINARY_DIR ${binary_dir})
	set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_BINARY_DIR}/)
	include_directories(${LIBSNDFILE_SOURCE_DIR})
	set(DEPENDENCIES ${DEPENDENCIES} LibSndFile)
endif(${LIBSNDFILE_FOUND})

message(STATUS "Could not find tritium.  This dependency will be downloaded.")
ExternalProject_Add(
tritium
GIT_REPOSITORY "https://github.com/syb0rg/tritium.git"
GIT_TAG "4a198455f49807f7cd8d486082a740f04650075b"
SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/tritium
CMAKE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/tritium
UPDATE_COMMAND ""
INSTALL_COMMAND ""
LOG_DOWNLOAD ON
LOG_UPDATE ON
LOG_CONFIGURE ON
LOG_BUILD ON
LOG_TEST ON
LOG_INSTALL ON
)
ExternalProject_Get_Property(tritium source_dir)
ExternalProject_Get_Property(tritium binary_dir)
set(TRITIUM_SOURCE_DIR ${source_dir})
set(TRITIUM_BINARY_DIR ${binary_dir})
set(TRITIUM_LIBRARIES ${PARCEL_BINARY_DIR}/libtritium.a)
set(DEPENDENCIES ${DEPENDENCIES} tritium)

include_directories(src/audio src/web ${PARCEL_SOURCE_DIR})
set(LIBS ${LIBS} ${CURL_LIBRARIES} ${PARCEL_LIBRARIES} ${PORTAUDIO_LIBRARIES} ${FLITE_LIBRARIES} ${LIBSNDFILE_LIBRARIES} ${CURL_LIBRARIES})
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(Khronos ${sources})
add_dependencies(Khronos ${DEPENDENCIES})
target_link_libraries(Khronos ${LIBS})
