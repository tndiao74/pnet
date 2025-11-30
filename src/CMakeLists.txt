
# Project Net
# Copyright 2025 Tony McCall

cmake_minimum_required (VERSION 3.5.1)

# set project name and compilers
project (pnet C CXX)

# setup the env
include (./_cmake/env.cmake)


#######################################################################
# sub projects
#######################################################################

#add_subdirectory (action)
add_subdirectory (pnetc)
