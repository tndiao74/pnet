
# Project Net
# Copyright 2025 Tony McCall

set(AWS_FOUND TRUE)

set(AWS_INCLUDE_DIR /usr/local/include)
set(AWS_LINK_LIBRARIES
	aws-cpp-sdk-core
)

#set(AWS_BINS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/aws/windows/bin)

#add_library (AWS SHARED IMPORTED)
#set_property (TARGET AWS PROPERTY IMPORTED_IMPLIB ${AWS_LINK_LIBRARIES})
