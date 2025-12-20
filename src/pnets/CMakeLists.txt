
# pnet server
# Copyright 2025 Tony McCall


# targets
set (TARGET_NAME pnets)

# files
set (SRCS
	stdafx.h
	stdafx.cpp
	main.h
	main.cpp
)


#######################################################################
# executable
#######################################################################

# add in the executable target
add_executable (${TARGET_NAME} ${SRCS})

# use a pch in windows
if (MSVC)
	set_target_properties (${TARGET_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
	set_source_files_properties (stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif (MSVC)


#######################################################################
# libraries
#######################################################################

target_link_libraries(${TARGET_NAME} pnet aws-cpp-sdk-core aws-cpp-sdk-s3 ${SSL_LINK_LIBRARIES} ${CRYPTO_LINK_LIBRARIES} ${ZLIB_LINK_LIBRARIES})

# copy exe to local server
if(WINDOWS_BUILD)
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
				$<TARGET_FILE:${TARGET_NAME}>        # Source file (the built executable)
				"C:/Apache24/cgi-bin/pnets" 			 # Destination directory
		COMMENT "Copying server to server."
	)
endif()

# copy resources
#file(COPY ${CMAKE_SOURCE_DIR}/Edit/icon_container.gif DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_SOURCE_DIR}/Edit/icon_obj.gif DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_SOURCE_DIR}/Edit/icon_sub.gif DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# copy dlls
if(WINDOWS_BUILD)
	file(COPY ${SSL_BINS}/libcrypto-3-x64.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${SSL_BINS}/libssl-3-x64.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${ZLIB_BINS}/zlib.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${CURL_BINS}/libcurl-x64.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(
	TARGET ${TARGET_NAME} POST_BUILD # Or pre-build
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${AWS_BINS}"
		"${CMAKE_CURRENT_BINARY_DIR}"
	COMMENT "Copying aws bin directory to build target location"
)
endif()