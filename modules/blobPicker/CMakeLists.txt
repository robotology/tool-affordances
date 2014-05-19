cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME blobPicker)

project(${PROJECTNAME})

find_package(YARP)
find_package(OpenCV)

include_directories(${ICUB_INCLUDE_DIRS}
                    ${YARP_INCLUDE_DIRS}
					${OpenCV_INCLUDE_DIRS} )
					
add_executable(${PROJECTNAME} blobPicker.cpp blobPicker_starter.cpp blobPicker.h)
target_link_libraries(${PROJECTNAME} ${OpenCV_LIBRARIES}  ${YARP_LIBRARIES})

#add_executable(startAffEnv startAffEnv.cpp)
#target_link_libraries(startAffEnv ${YARP_LIBRARIES})