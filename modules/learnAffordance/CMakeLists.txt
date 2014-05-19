cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME LEARNAFFORDANCE)

project(${PROJECTNAME})

find_package(YARP)
find_package(ICUB)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${YARP_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ICUB_MODULE_PATH})
include(iCubHelpers)


#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ICUB_LINK_FLAGS}")
include_directories(${ICUB_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS})
					
add_executable(learnAffordance learnAffordance.cpp learnAffordance_starter.cpp learnAffordance.h) 
target_link_libraries(learnAffordance ${YARP_LIBRARIES})

