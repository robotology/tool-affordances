# Copyright: (C) 2014-2017 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname AffordancesProject)

file(GLOB modules ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.xml)
file(GLOB conf    ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
file(GLOB conf    ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.txt)
file(GLOB temps ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.xml.template)
##file(GLOB apps ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.xml)
file(GLOB monitors ${CMAKE_CURRENT_SOURCE_DIR}/scripts/monitors/*.lua) 

set(lua_fsm        ${CMAKE_CURRENT_SOURCE_DIR}/LUA/xp_demo/xp_funcs.lua
                   ${CMAKE_CURRENT_SOURCE_DIR}/LUA/xp_demo/xp_interact_fsm.lua
                   ${CMAKE_CURRENT_SOURCE_DIR}/LUA/xp_demo/xp_root_fsm.lua)
set(lua_bin        ${CMAKE_CURRENT_SOURCE_DIR}/LUA/xp_demo/xp_main.lua)
        

### create a single target that installs all applications at once
yarp_install(FILES ${modules} DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})
yarp_install(FILES ${conf} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(FILES ${temps} DESTINATION ${ICUBCONTRIB_APPLICATIONS_TEMPLATES_INSTALL_DIR})
##yarp_install(FILES ${apps} DESTINATION ${ICUBCONTRIB_APPLICATIONS_INSTALL_DIR})
yarp_install(FILES ${monitors} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(PROGRAMS ${lua_fsm} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${PROJECTNAME}/LUA)

# We cannot go for "yarp_install(PROGRAMS ${lua_bin} DESTINATION bin)", since on Windows, only
# at build time (i.e. not during installation), we have to handle the Debug|Release subfolders.
# Requirements: the cmake "-E copy" command does not change file permissions, hence the file needs
# to be stored in the versioning database with *executable* rights.
add_custom_target(copy_lua_in_build ALL)
add_custom_command(TARGET copy_lua_in_build POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${lua_bin} ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}
                   COMMENT "Copying ${lua_bin} to ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/")
install(PROGRAMS ${lua_bin} DESTINATION bin)
