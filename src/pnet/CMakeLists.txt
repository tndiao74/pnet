
# pnet library
# Copyright 2025 Tony McCall


# targets
set (TARGET_NAME pnet)

# files
set (SRCS
	stdafx.h
	stdafx.cpp
	base64.h
	base64.cpp
	helpers.h
	helpers.cpp
	log.h
	log.cpp
	core.h
	core.cpp
	system.h
	system.cpp
)


#######################################################################
# executable
#######################################################################

# add in the executable target
add_library (${TARGET_NAME} ${SRCS})

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
