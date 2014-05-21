# Copyright: 2014 iCub Facility, IIT
# Author: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(PROJECTNAME affStateMachine)
PROJECT(${PROJECTNAME})

include_directories(${PROJECT_SOURCE_DIR}
                    ${YARP_INCLUDE_DIRS}
)

                    
#message ("Message opencvLIBS " ${OpenCV_LIBRARIES} )
                    
#target_link_libraries(${PROJECTNAME} ${YARP_LIBRARIES} )

#install(TARGETS ${PROJECTNAME} DESTINATION bin)

yarp_install(FILES ${PROJECTNAME}.xml DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})


#icubcontrib_finalize_export (${PROJECT_NAME})
#icubcontrib_add_uninstall_target()