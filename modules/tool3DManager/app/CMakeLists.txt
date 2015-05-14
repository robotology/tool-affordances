# Copyright: (C) 2013 VisLab - Institute for Systems and Robotics - Istituto Superior Técnico
# Authors: Afonso Gonçalves
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname tool3DManager)

file(GLOB conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
file(GLOB scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.template)
file(GLOB cloudsReal ${CMAKE_CURRENT_SOURCE_DIR}/clouds/real/*.ply
                     ${CMAKE_CURRENT_SOURCE_DIR}/clouds/real/*.pcd)
                 
file(GLOB cloudsSim ${CMAKE_CURRENT_SOURCE_DIR}/clouds/sim/*.ply
                    ${CMAKE_CURRENT_SOURCE_DIR}/clouds/sim/*.pcd)

         
### create a single target that installs all applications at once
yarp_install(FILES ${conf} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install( FILES ${cloudsReal} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname}/clouds/real)
yarp_install( FILES ${cloudsSim} DESTINATION ${ICUBCONTRIB_CONTEXTS_INSTALL_DIR}/${appname}/clouds/sim)
