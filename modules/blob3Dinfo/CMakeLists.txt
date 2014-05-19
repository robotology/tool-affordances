# Copyright: 2014 iCub Facility, IIT
# Author: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.8.7)
set(PROJECTNAME blob3Dinfo)
project(${PROJECTNAME})

find_package(YARP REQUIRED)
find_package(ICUB REQUIRED)
find_package(ICUBcontrib)

find_package(PCL 1.6 REQUIRED)
find_package(thrift3D REQUIRED)
find_package(minimumBoundingBox REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH ${ICUB_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH ${ICUBCONTRIB_MODULE_PATH})

include(YarpInstallationHelpers)
include(ICUBcontribHelpers)
include(ICUBcontribOptions)
icubcontrib_set_default_prefix()

include(YarpIDL)
yarp_idl_to_dir(${PROJECTNAME}.thrift ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB source src/*.cpp)
file(GLOB header include/*.h)
set(idl ${PROJECTNAME}.thrift)

source_group("Source Files" FILES ${source})
source_group("Header Files" FILES ${header})
source_group("IDL Files" FILES ${idl})

include_directories(${PCL_INCLUDE_DIRS} 
                    ${CMAKE_SOURCE_DIR} 
                    ${PROJECT_SOURCE_DIR}/include 
                    ${thrift3D_INCLUDE_DIRS} 
                    ${minimumBoundingBox_INCLUDE_DIRS}
                    ${YARP_INCLUDE_DIRS}
                    ${ICUB_INCLUDE_DIRS}
                    )                    
                    
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (${PROJECTNAME} ${source} ${header}  ${idl})
target_link_libraries (${PROJECTNAME} ${PCL_LIBRARIES} ${ICUB_LIBRARIES} ${YARP_LIBRARIES} minimumBoundingBox thrift3D)
install(TARGETS ${PROJECTNAME} DESTINATION bin)