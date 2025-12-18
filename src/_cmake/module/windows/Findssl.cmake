
# Project Net
# Copyright 2025 Tony McCall

set(SSL_FOUND TRUE)
set(SSL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ssl/windows/include)
set(SSL_LINK_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ssl/windows/lib/libssl.lib)
set(SSL_BINS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ssl/windows/bin)

add_library (SSL SHARED IMPORTED)
set_property (TARGET SSL PROPERTY IMPORTED_IMPLIB ${SSL_LINK_LIBRARIES})
