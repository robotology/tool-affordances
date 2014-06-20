# Copyright: (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
# Author: Matej Hoffmann
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

SET(PROJECTNAME objectFinder)

PROJECT(${PROJECTNAME})

SET(folder_source objectFinder.cpp)

find_package(YARP)
find_package(ICUB)
find_package(OpenCV)


include_directories(${ICUB_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

ADD_EXECUTABLE(${PROJECTNAME} ${folder_source})
TARGET_LINK_LIBRARIES(${PROJECTNAME} ${YARP_LIBRARIES} ${OpenCV_LIBRARIES} icubmod)                              
install(TARGETS ${PROJECTNAME} DESTINATION bin)

yarp_install(FILES ${PROJECTNAME}.xml DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})
