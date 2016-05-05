# Copyright: (C) 2016 iCub-Facility - Istituto Italiano di Tecnologia
# Authors: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)
set(PROJECTNAME affMotor)
project(${PROJECTNAME})

include_directories(${ICUB_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS})

set(folder_source main.cpp)
source_group("Source Files" FILES ${folder_source})

add_executable(${PROJECTNAME} ${folder_source})
target_link_libraries(${PROJECTNAME} ${YARP_LIBRARIES} ctrlLib)
install(TARGETS ${PROJECTNAME} DESTINATION bin)

yarp_install(FILES ${PROJECTNAME}.xml DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})

