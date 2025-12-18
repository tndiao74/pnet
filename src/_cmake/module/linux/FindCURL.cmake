
# Project Net
# Copyright 2025 Tony McCall

set(CURL_FOUND TRUE)
set(CURL_INCLUDE_DIR /usr/include)
set(CURL_LINK_LIBRARIES curl)

add_library (CURL SHARED IMPORTED)
set_property (TARGET CURL PROPERTY IMPORTED_IMPLIB ${CURL_LINK_LIBRARIES})
