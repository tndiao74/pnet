
# Project Net
# Copyright 2025 Tony McCall

set(CURL_FOUND TRUE)
set(CURL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/curl/windows/include)
set(CURL_LINK_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/curl/windows/lib/libcurl.dll.a)
set(CURL_BINS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/curl/windows/bin)

add_library (CURL SHARED IMPORTED)
set_property (TARGET CURL PROPERTY IMPORTED_IMPLIB ${CURL_LINK_LIBRARIES})
