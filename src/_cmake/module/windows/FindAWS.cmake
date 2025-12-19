
# Project Net
# Copyright 2025 Tony McCall

set(AWS_FOUND TRUE)

set(AWS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/aws/windows/include)

set(AWS_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/aws/windows/lib)
set(AWS_LINK_LIBRARIES
	${AWS_LIB_DIR}/aws-c-auth.lib
	${AWS_LIB_DIR}/aws-c-cal.lib
	${AWS_LIB_DIR}/aws-c-common.lib
	${AWS_LIB_DIR}/aws-c-compression.lib
	${AWS_LIB_DIR}/aws-c-event-stream.lib
	${AWS_LIB_DIR}/aws-c-http.lib
	${AWS_LIB_DIR}/aws-c-io.lib
	${AWS_LIB_DIR}/aws-c-mqtt.lib
	${AWS_LIB_DIR}/aws-c-s3.lib
	${AWS_LIB_DIR}/aws-c-sdkutils.lib
	${AWS_LIB_DIR}/aws-checksums.lib
	${AWS_LIB_DIR}/aws-cpp-sdk-core.lib
	${AWS_LIB_DIR}/aws-cpp-sdk-ec2.lib
	${AWS_LIB_DIR}/aws-cpp-sdk-s3.lib
	${AWS_LIB_DIR}/aws-crt-cpp.lib
)

set(AWS_BINS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/aws/windows/bin)

add_library (AWS SHARED IMPORTED)
set_property (TARGET AWS PROPERTY IMPORTED_IMPLIB ${AWS_LINK_LIBRARIES})
