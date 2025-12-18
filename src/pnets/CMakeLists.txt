
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

target_link_libraries(${TARGET_NAME} pnet ${SSL_LINK_LIBRARIES} ${CRYPTO_LINK_LIBRARIES} ${ZLIB_LINK_LIBRARIES})

# static build
#target_link_libraries(${TARGET_NAME} -static)


# use a pch in windows
if (MSVC)
	set_target_properties (${TARGET_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
	set_source_files_properties (stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif (MSVC)


#######################################################################
# libraries
#######################################################################

# link dependencies

# internal libraries
# target_link_libraries (${TARGET_NAME} Core Engine Render Physics Audio)

# external libraries
#target_link_libraries (${TARGET_NAME} ${OPENGL_LIBRARIES} LUA SDL2 SDL2main GLEW OpenAL ${FREETYPE_LIBRARIES} MIDIFILE RTMIDI)

# windows link dependencies
#if(WINDOWS_BUILD)
#	target_link_libraries (${TARGET_NAME} ${OPENGL_LIBRARIES} winmm.lib imm32.lib version.lib)
#endif()

#target_link_libraries (${TARGET_NAME} ${CGICC_LINK_LIBRARIES})


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

# linux link dependencies
#if(LINUX_BUILD)
#endif()

# copy dlls
if(WINDOWS_BUILD)
	#file(COPY ${CGICC_BINS}/cgicc.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	#file(COPY ${FREETYPE_LIBRARIE}/freetype.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	#file(COPY ${RTMIDI_LIBRARY}/rtmidi.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
