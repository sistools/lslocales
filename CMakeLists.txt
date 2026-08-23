
# ######################################################################## #
# File:     /CMakeLists.txt
#
# Purpose:  Top-level CMake lists file for lslocales
#
# Created:  16th August 2026
# Updated:  16th August 2026
#
# ######################################################################## #


# ##########################################################
# CMake

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# require out-of-source builds
file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")

	message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# directory for CMake specific extensions and source files.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


# ##########################################################
# project

project(lslocales
	DESCRIPTION "lslocales is a small, standalone utility program that lists installed system locales."
	HOMEPAGE_URL "https://github.com/sistools/lslocales"
	LANGUAGES C CXX
)

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

# handle version number
set(RX_PROJ_TAG "${PROJECT_NAME_UPPER}")
set(RX_WS "[ \t]")
file(READ "${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}.h" _header_file)
string(REGEX MATCH "#${RX_WS}*define${RX_WS}+SISTOOL_?${RX_PROJ_TAG}_VER_MAJOR${RX_WS}+([0-9]+)" MAJOR_DUMMY "${_header_file}")
set(_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "#${RX_WS}*define${RX_WS}+SISTOOL_?${RX_PROJ_TAG}_VER_MINOR${RX_WS}+([0-9]+)" MINOR_DUMMY "${_header_file}")
set(_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "#${RX_WS}*define${RX_WS}+SISTOOL_?${RX_PROJ_TAG}_VER_PATCH${RX_WS}+([0-9]+)" PATCH_DUMMY "${_header_file}")
set(_VERSION_PATCH ${CMAKE_MATCH_1})

# set project version number here
set(PROJECT_VERSION_MAJOR ${_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${_VERSION_PATCH})
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Default C standard is C17; override via prepare_cmake.sh --c-standard {99|11|17|23}
# (passes -DCMAKE_C_STANDARD=N) or by setting CMAKE_C_STANDARD on the cmake command line.
if(NOT CMAKE_C_STANDARD)

	set(CMAKE_C_STANDARD 17)
endif()
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON) # GNU extensions and POSIX standard
if(NOT CMAKE_CXX_STANDARD)

	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(MSVC)

	if(MSVC_VERSION GREATER_EQUAL 1914)

		add_compile_options("/Zc:__cplusplus")

		add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
	endif()

	if(MSVC_VERSION LESS 1930)

		set(CMAKE_C_STANDARD 90)
		set(CMAKE_CXX_STANDARD 98)
	endif()

	if(MSVC_USE_MT)

		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif(MSVC_USE_MT)
else(MSVC)

	if(MSVC_USE_MT)

		# this here just to absorb warning about not using `MSVC_USE_MT` (to
		# enable **prepare_cmake.sh** to be simple)
	endif(MSVC_USE_MT)
endif(MSVC)


# ##########################################################
# dependencies, includes, options

# ################################################
# includes - 1

include(BuildType)
#[====[
include(LanguageFullVersion)
include(TargetMacros)
#]====]


# ################################################
# dependencies, features, and options


# ######################################
# options

option(BUILD_EXAMPLES "Build examples" ON)

option(BUILD_TESTING "Build tests" ON)


# ######################################
# features


# ######################################
# dependencies
#
# required:
# - STLSoft;
# - CLASP;
# - Diagnosticism;
# - sistools-common-c;
#
# required if testing enabled:
# - xTests - required for testing;
#
# optional:


# ############################
# STLSoft
#
# We provide flexibility here, as in:
# 1. if a CMake variable STLSOFT is provided then it is used; otherwise
# 2. if an environment variable STLSOFT is provided then it is used;
#    otherwise
# 3. if the package STLSoft (via find_package()) is found then it is used;
#    otherwise
# 4. we abort
#
# This must run before other find_package() calls: CLASP, Diagnosticism,
# sistools-common-c, and xTests include STLSoft headers, and a path-based
# STLSOFT (variable or environment) only becomes visible via
# include_directories() after this block.

if(DEFINED STLSOFT)

	message("-- STLSOFT provided as CMake variable with value '${STLSOFT}'")

	set(STLSOFT_INCLUDE_DIR ${STLSOFT}/include)
elseif(DEFINED ENV{STLSOFT})

	message("-- STLSOFT provided as environment variable with value '$ENV{STLSOFT}'")

	set(STLSOFT_INCLUDE_DIR $ENV{STLSOFT}/include)
else()

	set(STLSoft_REQUIRED_VERSION_ 1.11.1)

	find_package(STLSoft ${STLSoft_REQUIRED_VERSION_} REQUIRED)

	message("-- CMake package STLSoft found (version ${STLSoft_VERSION}; ${STLSoft_REQUIRED_VERSION_} requested)")
endif()

if(DEFINED STLSOFT_INCLUDE_DIR)

	include_directories(${STLSOFT_INCLUDE_DIR})
endif()


# ############################
# CLASP

set(CLASP_REQUIRED_VERSION_ 0.15)

find_package(CLASP ${CLASP_REQUIRED_VERSION_} REQUIRED)

message("-- CMake package CLASP found (version ${CLASP_VERSION}; ${CLASP_REQUIRED_VERSION_} requested)")


# ############################
# Diagnosticism

set(DIAGNOSTICISM_REQUIRED_VERSION_ 0.3)

find_package(Diagnosticism ${DIAGNOSTICISM_REQUIRED_VERSION_} REQUIRED)

message("-- CMake package Diagnosticism found (version ${DIAGNOSTICISM_VERSION}; ${DIAGNOSTICISM_REQUIRED_VERSION_} requested)")


# ############################
# sistools-common-c

set(sistools-common-c_REQUIRED_VERSION_ 0.1)

find_package(sistools-common-c ${sistools-common-c_REQUIRED_VERSION_} REQUIRED)

message("-- CMake package sistools-common-c found (version ${sistools-common-c_VERSION}; ${sistools-common-c_REQUIRED_VERSION_} requested)")


# ############################
# xTests

if(BUILD_TESTING)

	set(xTests_REQUIRED_VERSION_ 0.26.4)

	find_package(xTests ${xTests_REQUIRED_VERSION_} REQUIRED)

	message("-- CMake package xTests found (version ${xTests_VERSION}; ${xTests_REQUIRED_VERSION_} requested)")
endif(BUILD_TESTING)


# ################################################
# includes - 2

include(CMakePackageConfigHelpers)
if(BUILD_TESTING)

	include(CTest)
endif(BUILD_TESTING)
include(GNUInstallDirs)


# ##########################################################
# tool

add_executable(${PROJECT_NAME_LOWER}
	entry.c
	${PROJECT_NAME_LOWER}.c
)

target_compile_options(${PROJECT_NAME_LOWER}
	PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
			-Werror -Wall -Wextra -pedantic

			-Wno-ignored-qualifiers
			-Wno-incompatible-pointer-types-discards-qualifiers
			-Wno-unused-parameter
		>
		$<$<CXX_COMPILER_ID:MSVC>:
			/WX /W4

			/wd4996
		>
)

target_link_libraries(${PROJECT_NAME_LOWER}
	PRIVATE
		CLASP::core
		sistools-common-c::core
		Diagnosticism::core
		$<$<STREQUAL:${STLSOFT_INCLUDE_DIR},>:STLSoft::STLSoft>
)

install(TARGETS ${PROJECT_NAME_LOWER}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES
	doc/${PROJECT_NAME_LOWER}.1
	DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
)


# ################################################
# examples


# ################################################
# tests

if(BUILD_TESTING)

	message("-- enabled building of tests ...")

	add_executable(${PROJECT_NAME_LOWER}_test
		${PROJECT_NAME_LOWER}_test.c
		${PROJECT_NAME_LOWER}.c
	)

	target_compile_options(${PROJECT_NAME_LOWER}_test
		PRIVATE
			$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
				-Werror -Wall -Wextra -pedantic

				-Wno-ignored-qualifiers
				-Wno-incompatible-pointer-types-discards-qualifiers
				-Wno-unused-parameter
			>
			$<$<CXX_COMPILER_ID:MSVC>:
				/WX /W4

				/wd4996
			>
	)

	target_link_libraries(${PROJECT_NAME_LOWER}_test
		PRIVATE
			xTests::core
			$<$<STREQUAL:${STLSOFT_INCLUDE_DIR},>:STLSoft::STLSoft>
	)

	add_test(NAME ${PROJECT_NAME_LOWER}_test COMMAND ${PROJECT_NAME_LOWER}_test)
else(BUILD_TESTING)

	message("-- disabled building of tests - define BUILD_TESTING to enable")
endif(BUILD_TESTING)


# ##########################################################
# completion

message(NOTICE "Generating CMake build scripts for ${PROJECT_NAME} ${PROJECT_VERSION}, for C${CMAKE_C_STANDARD} C++${CMAKE_CXX_STANDARD}, with build type ${CMAKE_BUILD_TYPE}")


# ############################## end of file ############################# #
