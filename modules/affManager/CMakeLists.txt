# Copyright: 2014 iCub Facility, IIT
# Author: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(PROJECTNAME affManager)
PROJECT(${PROJECTNAME})

include(YarpIDL)
yarp_idl_to_dir(${PROJECTNAME}.thrift ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB source src/*.cpp)
file(GLOB header include/*.h)
set(idl ${PROJECTNAME}.thrift)

source_group("Source Files" FILES ${source})
source_group("Header Files" FILES ${header})
source_group("IDL Files" FILES ${idl})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${YARP_INCLUDE_DIRS}
                    ${ICUB_INCLUDE_DIRS} )

                    
#message ("Message opencvLIBS " ${OpenCV_LIBRARIES} )
                    
add_executable(${PROJECTNAME} ${source} ${header} ${idl} )
target_link_libraries(${PROJECTNAME} ${YARP_LIBRARIES}  icubmod ctrlLib)

add_executable(startAffEnv startAffEnv.cpp)
target_link_libraries(startAffEnv ${YARP_LIBRARIES})

install(TARGETS ${PROJECTNAME} DESTINATION bin)

yarp_install(FILES ${PROJECTNAME}.xml DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})


#icubcontrib_finalize_export (${PROJECT_NAME})
#icubcontrib_add_uninstall_target()
