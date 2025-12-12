
# Project Net
# Copyright 2025 Tony McCall


#######################################################################
# Platform
#######################################################################

# x32 or x64 build
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(EX_PLATFORM 64)
	set(EX_PLATFORM_NAME "x64")
else (CMAKE_SIZEOF_VOID_P EQUAL 8) 
	set(EX_PLATFORM 32)
	set(EX_PLATFORM_NAME "x86")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

# OS
if(WIN32)
	add_definitions(-DWINDOWS)
	set(WINDOWS_BUILD ON)
	set(OS Windows)
elseif( UNIX )
	add_definitions( -DLINUX )
	set(LINUX_BUILD ON)
	set(OS Linux)
endif()


#######################################################################
# Debug
#######################################################################

if("${CMAKE_BUILD_TYPE}" STREQUAL "" AND WINDOWS_BUILD)
	message(WARNING "Default CMAKE_BUILD_TYPE=Debug")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	# debug define
	add_definitions(-D_DEBUG)
	set(DEBUG_BUILD ON)
endif()


#######################################################################
# Compilers
#######################################################################

set(CMAKE_CXX_STANDARD 17)


#######################################################################
# Settings
#######################################################################

set_property(GLOBAL PROPERTY USE_FOLDERS ON)



#######################################################################
# Libraries
#######################################################################

# Make project-provided Find modules available
if (WINDOWS_BUILD)
	set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/_cmake/module/windows)
elseif (LINUX_BUILD)
	set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/_cmake/module/linux)
endif()

# nlohmann-json
find_package(nlohmann_json REQUIRED)
include_directories (${NLOHMANN_JSON_INCLUDE_DIR})

## crpyto
#find_package(crypto REQUIRED)
#include_directories (${CRYPTO_INCLUDE_DIR})
#
## OpenSSL
#find_package(OpenSSL REQUIRED)
#include_directories (${OPENSSL_INCLUDE_DIR})
#
## psl
#find_package(psl REQUIRED)
#include_directories (${PSL_INCLUDE_DIR})
#
## libzip
#find_package(zlib REQUIRED)
#include_directories (${ZLIB_INCLUDE_DIR})
#
## CURL
find_package(CURL REQUIRED)
include_directories (${CURL_INCLUDE_DIR})

# sudo apt install openssl libssl-dev
# sudo apt install libcurl4-openssl-dev