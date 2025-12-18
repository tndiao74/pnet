
# Project Net
# Copyright 2025 Tony McCall

set(SSL_FOUND TRUE)
set(SSL_INCLUDE_DIR /usr/include)
set(SSL_LINK_LIBRARIES ssl)

add_library (SSL SHARED IMPORTED)
set_property (TARGET SSL PROPERTY IMPORTED_IMPLIB ${SSL_LINK_LIBRARIES})
