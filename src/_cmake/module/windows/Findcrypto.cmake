
# Project Net
# Copyright 2025 Tony McCall

set(CRYPTO_FOUND TRUE)
set(CRYPTO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ssl/windows/include)
set(CRYPTO_LINK_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ssl/windows/lib/libcrypto.lib)
set(CRYPTO_BINS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ssl/windows/bin)

add_library (CRYPTO SHARED IMPORTED)
set_property (TARGET CRYPTO PROPERTY IMPORTED_IMPLIB ${CRYPTO_LINK_LIBRARIES})
