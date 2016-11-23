# Copyright: (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname AffordancesProject)

file(GLOB modules ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.xml)
file(GLOB conf    ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
file(GLOB conf    ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.txt)
file(GLOB temps ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.xml.template)
##file(GLOB apps ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.xml)
file(GLOB monitors ${CMAKE_CURRENT_SOURCE_DIR}/scripts/monitors/*.lua)         

### create a single target that installs all applications at once
yarp_install(FILES ${modules} DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})
yarp_install(FILES ${conf} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(FILES ${temps} DESTINATION ${ICUBCONTRIB_APPLICATIONS_TEMPLATES_INSTALL_DIR})
##yarp_install(FILES ${apps} DESTINATION ${ICUBCONTRIB_APPLICATIONS_INSTALL_DIR})
yarp_install(FILES ${monitors} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})
