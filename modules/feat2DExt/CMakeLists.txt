cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME feat2DExt)

project(${PROJECTNAME})

find_package(YARP REQUIRED)
find_package(OpenCV)

include(YarpIDL)
# Thrift compile definition file to generate source code into the desired directory
set(generated_libs_dir "${CMAKE_CURRENT_SOURCE_DIR}")
yarp_idl_to_dir(vecvec.thrift ${generated_libs_dir})

file(GLOB source src/*.cpp)
file(GLOB header include/*.h)
set(idl vecvec.thrift)

source_group("Source Files" FILES ${source})
source_group("Header Files" FILES ${header})
source_group("IDL Files" FILES ${idl})


include_directories(${PROJECT_SOURCE_DIR}/include
                    ${YARP_INCLUDE_DIRS}
                    ${ICUB_INCLUDE_DIRS}
					${OpenCV_INCLUDE_DIRS} )

	
# CMake has compiled the file during the configuration process, so now we can include the generated .cmake file that lists all sources into "headers" and "sources" variables
include(vecvec_thrift.cmake)
include_directories(${generated_libs_dir}/include)

#message ("Message opencvLIBS" ${OpenCV_LIBRARIES} )
        
add_executable(${PROJECTNAME} ${source} ${header}  ${idl})
target_link_libraries(${PROJECTNAME} ${OpenCV_LIBRARIES}  ${YARP_LIBRARIES} ${ICUB_LIBRARIES})

INSTALL(TARGETS ${PROJECTNAME} DESTINATION bin)

yarp_install(FILES ${PROJECTNAME}.xml DESTINATION ${ICUBCONTRIB_MODULES_INSTALL_DIR})


