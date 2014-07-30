# Copyright: (C) 2013 VisLab - Institute for Systems and Robotics - Istituto Superior Técnico
# Authors: Afonso Gonçalves
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME simtoolloader)

# Start a project.
project(${PROJECTNAME})

# add include directories
include_directories(${YARP_INCLUDE_DIRS})
include_directories(${ICUB_INCLUDE_DIRS})

# add required linker flags
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ICUB_LINK_FLAGS}")

file(GLOB folder_source src/*.cpp)
file(GLOB folder_header src/*.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

add_executable(${PROJECTNAME} ${folder_source} ${folder_header})

# we now add the YARP and iCub libraries to our project.
target_link_libraries(${PROJECTNAME} ${YARP_LIBRARIES} icubmod)
install(TARGETS ${PROJECTNAME} DESTINATION bin)

add_subdirectory(app)
