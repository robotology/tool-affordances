# Copyright: (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

#set(appname AffordancesProject)
set(lua_bin ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer.lua)

#file(GLOB lua_bin ${CMAKE_CURRENT_SOURCE_DIR}/*.lua)


install(PROGRAMS ${lua_bin} DESTINATION bin)
         
### create a single target that installs all applications at once
#yarp_install(FILES ${modules} DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})


