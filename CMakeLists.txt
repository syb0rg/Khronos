CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
INCLUDE(ExternalProject)
SET(PROJECT_NAME Khronos)
PROJECT(${PROJECT_NAME})
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/library-build)
SET(CMAKE_C_FLAGS "-std=gnu11 -O3 -fprofile-arcs -ftest-coverage")

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
FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
	ADD_CUSTOM_TARGET(DOC
		${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
endif ()

FIND_PACKAGE(sphinxbase)
if (${SPHINXBASE_FOUND})
    INCLUDE_DIRECTORIES(${SPHINXBASE_INCLUDE_DIR}/sphinxbase/)
else ()
    message(STATUS "Could not find sphinxbase.  This dependency will be downloaded.")
    ExternalProject_Add(
        sphinxbase
        GIT_REPOSITORY      "https://github.com/cmusphinx/sphinxbase.git"
        GIT_TAG             "e34b1c632392276101ed16e8a05862e43f038a7c"
        SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/sphinxbase
        CONFIGURE_COMMAND   ${CMAKE_CURRENT_SOURCE_DIR}/lib/sphinxbase/autogen.sh
        BUILD_COMMAND       ${MAKE}
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     ""
        BUILD_IN_SOURCE     ON
        LOG_DOWNLOAD        ON
        LOG_UPDATE          ON
        LOG_CONFIGURE       ON
        LOG_BUILD           ON
        LOG_TEST            ON
        LOG_INSTALL         ON
    )
    ExternalProject_Get_Property(sphinxbase SOURCE_DIR)
    ExternalProject_Get_Property(sphinxbase BINARY_DIR)
    SET(SPHINXBASE_SOURCE_DIR ${SOURCE_DIR})
    SET(SPHINXBASE_BINARY_DIR ${BINARY_DIR})
    SET(SPHINXBASE_LIBRARIES ${SPHINXBASE_BINARY_DIR}/src/libsphinxbase/.libs/libsphinxbase.a)
    INCLUDE_DIRECTORIES(${SPHINXBASE_SOURCE_DIR}/include)
    SET(DEPENDENCIES ${DEPENDENCIES} sphinxbase)
endif ()
SET(LIBS ${LIBS} ${SPHINXBASE_LIBRARIES})

FIND_PACKAGE(pocketsphinx)
if (${POCKETSPHINX_FOUND})
    INCLUDE_DIRECTORIES(${POCKETSPHINX_INCLUDE_DIR}/pocketsphinx/)
    add_definitions( -DMODELDIR="/usr/local/share/pocketsphinx/model")  # more portable?
else ()
    message(STATUS "Could not find pocketsphinx.  This dependency will be downloaded.")
    ExternalProject_Add(
        pocketsphinx
        DEPENDS             sphinxbase
        GIT_REPOSITORY      "https://github.com/cmusphinx/pocketsphinx.git"
        GIT_TAG             "0d41af2efe3f77a3e7f22a54e42684bb1562876d"
        SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/pocketsphinx
        CONFIGURE_COMMAND   ${CMAKE_CURRENT_SOURCE_DIR}/lib/pocketsphinx/autogen.sh --prefix=<INSTALL_DIR> --with-sphinxbase=${SPHINXBASE_BINARY_DIR}
        BUILD_COMMAND       ${MAKE}
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     ""
        BUILD_IN_SOURCE     ON
        LOG_DOWNLOAD        ON
        LOG_UPDATE          ON
        LOG_CONFIGURE       ON
        LOG_BUILD           ON
        LOG_TEST            ON
        LOG_INSTALL         ON
        )
    ExternalProject_Get_Property(pocketsphinx SOURCE_DIR)
    ExternalProject_Get_Property(pocketsphinx BINARY_DIR)
    SET(POCKETSPHINX_SOURCE_DIR ${SOURCE_DIR})
    SET(POCKETSPHINX_BINARY_DIR ${BINARY_DIR})
    SET(POCKETSPHINX_LIBRARIES ${POCKETSPHINX_BINARY_DIR}/src/libpocketsphinx/.libs/libpocketsphinx.a)
    INCLUDE_DIRECTORIES(${POCKETSPHINX_SOURCE_DIR}/include/)
    SET(DEPENDENCIES ${DEPENDENCIES} pocketsphinx)
    add_definitions( -DMODELDIR="${POCKETSPHINX_BINARY_DIR}/model")
endif ()
SET(LIBS ${LIBS} ${POCKETSPHINX_LIBRARIES})

FIND_PACKAGE(PortAudio)
if (${PORTAUDIO_FOUND})
else ()
	MESSAGE(STATUS "Could not find PortAudio.  This dependency will be downloaded.")
	ExternalProject_Add(
		PortAudio
		SVN_REPOSITORY      "https://subversion.assembla.com/svn/portaudio/portaudio/trunk/"
		SVN_TRUST_CERT      1
		SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio
		CONFIGURE_COMMAND   ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/configure --prefix=<INSTALL_DIR>
		BUILD_COMMAND       ${MAKE}
		UPDATE_COMMAND      ""
		INSTALL_COMMAND     ""
        BUILD_IN_SOURCE     ON
		LOG_DOWNLOAD        ON
		LOG_UPDATE          ON
		LOG_CONFIGURE       ON
		LOG_BUILD           ON
		LOG_TEST            ON
		LOG_INSTALL         ON
	)
	ExternalProject_Get_Property(PortAudio SOURCE_DIR)
	ExternalProject_Get_Property(PortAudio BINARY_DIR)
	SET(PORTAUDIO_SOURCE_DIR ${SOURCE_DIR})
	SET(PORTAUDIO_BINARY_DIR ${BINARY_DIR})
	SET(PORTAUDIO_LIBRARIES ${PORTAUDIO_BINARY_DIR}/lib/.libs/libportaudio.dylib)
	INCLUDE_DIRECTORIES(${PORTAUDIO_SOURCE_DIR})
	SET(DEPENDENCIES ${DEPENDENCIES} PortAudio)
endif ()
SET(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})

#FIND_PACKAGE(LibOgg)

#FIND_PACKAGE(LibVorbis)

FIND_PACKAGE(FLAC)
if (${FLAC_FOUND})
else ()
    ExternalProject_Add(
        FLAC
        GIT_REPOSITORY      "git://github.com/xiph/flac.git"
        GIT_TAG             "0e11f73eabd3544f59937d0a0d8e076d7c9c2d1d"
        SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac
        PATCH_COMMAND       ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac/autogen.sh
        CONFIGURE_COMMAND   ${CMAKE_CURRENT_SOURCE_DIR}/lib/flac/configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND       ${MAKE}
        UPDATE_COMMAND      ""
        INSTALL_COMMAND     ""
        BUILD_IN_SOURCE     ON
        LOG_DOWNLOAD        ON
        LOG_UPDATE          ON
        LOG_CONFIGURE       ON
        LOG_BUILD           ON
        LOG_TEST            ON
        LOG_INSTALL         ON
    ExternalProject_Get_Property(FLAC SOURCE_DIR)
    ExternalProject_Get_Property(FLAC BINARY_DIR)
    SET(FLAC_SOURCE_DIR ${SOURCE_DIR})
    SET(FLAC_BINARY_DIR ${BINARY_DIR})
    SET(FLAC_LIBRARIES ${FLAC_BINARY_DIR}/lib/.libs/libFLAC.dylib)
    INCLUDE_DIRECTORIES(${FLAC_SOURCE_DIR})
    SET(DEPENDENCIES ${DEPENDENCIES} libFLAC)
    )
endif ()
SET(LIBS ${LIBS} ${FLAC_LIBRARIES})

FIND_PACKAGE(LibSndFile)
if(${LIBSNDFILE_FOUND})
else ()
	ExternalProject_Add(
		LibSndFile
		DEPENDS             FLAC
		URL                 "http://www.mega-nerd.com/libsndfile/files/libsndfile-1.0.25.tar.gz"
		CONFIGURE_COMMAND   ${CMAKE_CURRENT_SOURCE_DIR}/lib/LibSndFile/configure --prefix=<INSTALL_DIR>
		BUILD_COMMAND       ${MAKE}
		SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/LibSndFile
		UPDATE_COMMAND      ""
		INSTALL_COMMAND     ""
        BUILD_IN_SOURCE     ON
		LOG_DOWNLOAD        ON
		LOG_UPDATE          ON
		LOG_CONFIGURE       ON
		LOG_BUILD           ON
		LOG_TEST            ON
		LOG_INSTALL         ON
	)
    SET(DEPENDENCIES ${DEPENDENCIES} LibSndFile)
endif ()
SET(LIBS ${LIBS} ${LIBSNDFILE_LIBRARIES})
INCLUDE_DIRECTORIES(${LIBSNDFILE_INCLUDE_DIRS})

# TODO: create FindTritium.cmake
message(STATUS "Could not find tritium.  This dependency will be downloaded.")
ExternalProject_Add(
    tritium
    GIT_REPOSITORY  "https://github.com/syb0rg/tritium.git"
    GIT_TAG         "9eb6b9884f3b8d46c9f638833fe2ffc2489955ca"
    SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/lib/tritium
    UPDATE_COMMAND  ""
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE ON
    LOG_DOWNLOAD    ON
    LOG_UPDATE      ON
    LOG_CONFIGURE   ON
    LOG_BUILD       ON
    LOG_TEST        ON
    LOG_INSTALL     ON
)
ExternalProject_Get_Property(tritium SOURCE_DIR)
ExternalProject_Get_Property(tritium BINARY_DIR)
SET(TRITIUM_SOURCE_DIR ${SOURCE_DIR})
SET(TRITIUM_BINARY_DIR ${BINARY_DIR})
INCLUDE_DIRECTORIES(${SOURCE_DIR}/include)
SET(TRITIUM_LIBRARIES ${TRITIUM_BINARY_DIR}/libtritium.a)
SET(LIBS ${LIBS} ${TRITIUM_LIBRARIES})
SET(DEPENDENCIES ${DEPENDENCIES} tritium)

INCLUDE_DIRECTORIES(include)
ADD_SUBDIRECTORY(src)

ADD_EXECUTABLE(${PROJECT_NAME}
        $<TARGET_OBJECTS:audio>
        $<TARGET_OBJECTS:util>
        $<TARGET_OBJECTS:main>
        )
ADD_DEPENDENCIES(${PROJECT_NAME} ${DEPENDENCIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBS})
