# Copyright: (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
set(appname AffordancesProject)

file(GLOB lua_bin ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer.lua
#                  ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer-TCDS.lua)
                   ${CMAKE_CURRENT_SOURCE_DIR}/aff-xperience.lua)
file(GLOB modules ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer.xml)
file(GLOB helpers ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer_helper.lua
#                  ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer-TCDS_helper.lua      
                  ${CMAKE_CURRENT_SOURCE_DIR}/yarp_helper.lua     )

source_group("Source Files" FILES ${lua_bin})

include_directories(${YARP_INCLUDE_DIRS})

install(PROGRAMS ${lua_bin} DESTINATION bin)
         
### create a single target that installs all applications at once
yarp_install(FILES ${modules} DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})

yarp_install(FILES ${helpers} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})

