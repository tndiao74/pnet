
# Project Net
# Copyright 2025 Tony McCall

set(ZLIB_FOUND TRUE)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/zlib/windows/include)
set(ZLIB_LINK_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/zlib/windows/lib/zlib.lib)
set(ZLIB_BINS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/curl/windows/bin)

add_library (ZLIB SHARED IMPORTED)
set_property (TARGET ZLIB PROPERTY IMPORTED_IMPLIB ${ZLIB_LINK_LIBRARIES})
