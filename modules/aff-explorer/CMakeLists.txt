# Copyright: (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname AffordancesProject)

file(GLOB lua_bin ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer.lua)
file(GLOB modules ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer.xml)
file(GLOB helpers ${CMAKE_CURRENT_SOURCE_DIR}/aff-explorer_helper.lua   
                  ${CMAKE_CURRENT_SOURCE_DIR}/yarp_helper.lua     )

install(PROGRAMS ${modules} DESTINATION bin)
         
### create a single target that installs all applications at once
yarp_install(FILES ${modules} DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})

yarp_install(FILES ${helpers} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})

