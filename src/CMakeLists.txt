
# ProjectCGI
# Copyright 2025 Tony McCall

cmake_minimum_required (VERSION 3.5.1)

# set project name and compilers
project (ProjectCGI C CXX)

# setup the env
include(./_cmake/env.cmake)


#######################################################################
# sub projects
#######################################################################

#add_subdirectory (Core)
#add_subdirectory (Engine)
#add_subdirectory (Render)
#add_subdirectory (Physics)
#add_subdirectory (Audio)
add_subdirectory (action)
